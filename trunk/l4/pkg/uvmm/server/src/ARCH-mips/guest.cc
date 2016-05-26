/*
 * Copyright (C) 2015 Kernkonzept GmbH.
 * Author(s): Sarah Hoffmann <sarah.hoffmann@kernkonzept.com>
 *
 * This file is distributed under the terms of the GNU General Public
 * License, version 2.  Please see the COPYING-GPL-2 file for details.
 */

#include <l4/cxx/static_container>

#include "guest.h"
#include "binary_loader.h"

static cxx::Static_container<Vmm::Guest> guest;

static void
handler(l4_vcpu_state_t *vcpu)
{
  guest->handle_entry(Vmm::Cpu(vcpu));
}

namespace Vmm {

Guest::Guest(L4::Cap<L4Re::Dataspace> ram)
: Guest::Generic_guest(ram, 0, 0x80000000),
  _core_ic(Vdev::make_device<Gic::Mips_core_ic>())
{
  // TODO Fiasco should be exporting the proc ID for us. For the
  //      moment just derive it from the platform.
  auto *platform = l4re_kip()->platform_info.name;
  if (strcmp(platform, "baikal_t") == 0)
    _proc_id = 0x0001a82c; // P5600
  else
    _proc_id = 0x0001a700; // M5150
}

Guest *
Guest::create_instance(L4::Cap<L4Re::Dataspace> ram)
{
  guest.construct(ram);
  return guest;
}

void
Guest::load_device_tree(char const *name)
{
  load_device_tree_at(name, 0x100, 0x200);

  // advertise CPU core timer frequency in DTS
  auto node = device_tree().path_offset("/cpus");
  node.setprop_u32("mips-hpt-frequency", l4re_kip()->frequency_cpu * 1000);
}

int
Guest::config_as_core_device(L4::Cap<L4Re::Dataspace>,
                             Vdev::Dt_node const &node,
                             cxx::Ref_ptr<Vdev::Device> *dev)
{
  if (node.is_compatible("mti,cpu-interrupt-controller") == 0)
    {
      Dbg().printf("Core interrupt controller enabled\n");
      *dev = _core_ic;
      return 1;
    }

  if (node.is_compatible("mti,gic") == 0)
    {
      auto *prop = node.check_prop<fdt32_t const>("reg", 2);

      uint32_t base = fdt32_to_cpu(prop[0]);
      uint32_t size = fdt32_to_cpu(prop[1]);

      auto gic = Vdev::make_device<Gic::Dist>(size);

      _memmap[Region::ss(base, size)] = gic;

      gic->set_cpu_ic(0, _core_ic.get());

      Dbg().printf("GIC configured @ 0x%x (0x%x)\n", base, size);
      *dev = gic;
      return 1;
    }

  return 0;
}

l4_addr_t
Guest::load_linux_kernel(char const *kernel, char const *cmdline, Cpu vcpu)
{
  Boot::Binary_ds kbin(kernel);

  l4_addr_t entry = _ram.boot_addr(0x100400);
  l4_addr_t start, end;
  if (kbin.is_elf_binary())
    {
      entry = kbin.load_as_elf(&_ram);

      kbin.elf_addr_bounds(&start, &end);

      start = _ram.local_start() + _ram.boot2ram(start);
      end = _ram.local_start() + _ram.boot2ram(end);
    }
  else
    {
      l4_size_t sz;
      _ram.load_file(kernel, 0x100000, &sz);

      start = _ram.local_start() + 0x100000;
      end = start + sz;
    }

  if (has_device_tree() && device_tree().overlaps(start, end))
    L4Re::chksys(-L4_EINVAL, "Linux binary overlaps with device tree in RAM.");

  // Initial register setup:
  //  a0 - number of kernel arguments
  //  a1 - address of kernel arguments
  //  a2 - unused
  //  a3 - address of DTB
  L4virtio::Ptr<l4_uint32_t> prom_tab(l4_round_size(device_tree().size(), 12));
  L4virtio::Ptr<char> prom_buf(prom_tab.get() + 2 * sizeof(l4_uint32_t));

  // Setup initial arguments
  // two arguments: kernel name and optionally cmdline
  vcpu->r.a0 = cmdline ? 2 : 1;
  vcpu->r.a1 = _ram.boot_addr(prom_tab);

  _ram.access(prom_tab)[0] = _ram.boot_addr(prom_buf.get());
  unsigned strpos = sprintf(_ram.access(prom_buf), "%s", kernel) + 1;

  if (cmdline)
    {
      prom_buf = L4virtio::Ptr<char>(prom_buf.get() + strpos);
      _ram.access(prom_tab)[1] = _ram.boot_addr(prom_buf.get());
      strcpy(_ram.access(prom_buf), cmdline);
    }

  vcpu->r.a2 = 0;
  vcpu->r.a3 = _ram.boot_addr(_device_tree);
  vcpu->r.status = 8;
  vcpu->r.ip = entry;

  // sync i-cache on binary
  unsigned step;
  asm volatile("rdhwr %0, $1" : "=r" (step));

  for (l4_addr_t a = start; a <= end; a += step)
    asm volatile("synci 0(%0)" : : "r" (a));

  asm volatile("sync");

  return end - _ram.local_start();
}

void
Guest::show_state_registers()
{
  for (int i = 0; i < 1; ++i)
    {
      //if (i != current_cpu)
      //  interrupt_vcpu(i);

      Cpu v = *_vcpu[i];
      printf("CPU %d\n", i);
      printf("EPC=%08lx SP=%08lx\n", v->r.ip, v->r.sp);
      printf("Status=%08lx  Cause=%08lx\n", v->r.status, v->r.cause);
      printf("ULR=%08lx  Hi=%08lx Lo=%08lx\n", v->r.ulr, v->r.hi, v->r.lo);
      printf("at/ 1=%08lx v0/ 2=%08lx v1/ 3=%08lx\n",
             v->r.r[1], v->r.r[2], v->r.r[3]);
      printf("a0/ 4=%08lx a1/ 5=%08lx a1/ 6=%08lx a4/ 7=%08lx\n",
             v->r.r[4], v->r.r[5], v->r.r[6], v->r.r[7]);
      printf("t0/ 8=%08lx t1/ 9=%08lx t2/10=%08lx t3/11=%08lx\n",
             v->r.r[8], v->r.r[9], v->r.r[10], v->r.r[11]);
      printf("t4/12=%08lx t5/13=%08lx t6/14=%08lx t7/15=%08lx\n",
             v->r.r[12], v->r.r[13], v->r.r[14], v->r.r[15]);
      printf("s0/16=%08lx s1/17=%08lx s2/18=%08lx s3/19=%08lx\n",
             v->r.r[16], v->r.r[17], v->r.r[18], v->r.r[19]);
      printf("s4/20=%08lx s5/21=%08lx s6/22=%08lx s7/23=%08lx\n",
             v->r.r[20], v->r.r[21], v->r.r[22], v->r.r[23]);
      printf("t8/24=%08lx t9/25=%08lx k0/26=%08lx k1/27=%08lx\n",
             v->r.r[24], v->r.r[25], v->r.r[26], v->r.r[27]);
      printf("gp/28=%08lx sp/29=%08lx s8/30=%08lx ra/31=%08lx\n",
             v->r.r[28], v->r.r[29], v->r.r[30], v->r.r[31]);

      auto *s = v.state();
      s->update_state(~0UL);
      printf("\nGuestCtl0=%08lx  Guestctl0_ext=%08lx\n",
             s->guest_ctl_0, s->guest_ctl_0_ext);
      printf("GuestCtl1=%08lx  Guestctl2    =%08lx\n",
             s->guest_ctl_1, s->guest_ctl_2);
      printf("\nGuest CP0:\n");

      printf("Status   =%08lx  Cause    =%08lx\n", s->g_status, s->g_cause);
      printf("Index    =%08lx  EBase    =%08lx\n", s->g_index, s->g_ebase);
      printf("EntryLo0 =%08lx  EntryLo1 =%08lx\n", s->g_entry_lo[0], s->g_entry_lo[1]);
      printf("Context  =%08lx  EntryHi  =%08lx\n", s->g_context, s->g_entry_hi);
      printf("PageMask =%08lx  PageGrain=%08lx\n", s->g_page_mask, s->g_page_grain);
      printf("ULR      =%08lx  Wired    =%08lx\n", s->g_ulr, s->g_wired);
      printf("SegCtl0  =%08lx  SegCtl1  =%08lx\n", s->g_seg_ctl[0], s->g_seg_ctl[1]);
      printf("SegCtl2  =%08lx  HWRena   =%08lx\n", s->g_seg_ctl[2], s->g_hwrena);
      printf("PWBase   =%08lx  PWField  =%08lx\n", s->g_pw_base, s->g_pw_field);
      printf("PWSize   =%08lx  PWCtl    =%08lx\n", s->g_pw_size, s->g_pw_ctl);
      printf("BadVAddr =%08lx  BadInstr =%08lx\n", s->g_bad_v_addr, s->g_bad_instr);
      printf("BadInstrP=%08lx  Compare  =%08lx\n", s->g_bad_instr_p, s->g_compare);
      printf("IntCtl   =%08lx  EPC      =%08lx\n", s->g_intctl, s->g_epc);
      printf("Config0  =%08lx  Config1  =%08lx\n", s->g_cfg[0], s->g_cfg[1]);
      printf("Config2  =%08lx  Config3  =%08lx\n", s->g_cfg[2], s->g_cfg[3]);
      printf("Config4  =%08lx  Config5  =%08lx\n", s->g_cfg[4], s->g_cfg[5]);
    }
}

void
Guest::show_state_interrupts()
{
  for (int i = 0; i < 1; ++i)
    {
      //if (i != current_cpu)
      //  interrupt_vcpu(i);

      Cpu v = *_vcpu[i];

      printf("\nCPU %d core IC:\n", i);
      _core_ic->show_state(v);
    }
}


void
Guest::run(Cpu vcpu)
{
  _vcpu[0] = &vcpu;

  l4_umword_t sp;
  asm ("move %0, $sp" : "=r" (sp));

  vcpu->saved_state = L4_VCPU_F_FPU_ENABLED
                      | L4_VCPU_F_USER_MODE
                      | L4_VCPU_F_IRQ
                      | L4_VCPU_F_PAGE_FAULTS
                      | L4_VCPU_F_EXCEPTIONS;
  vcpu->entry_ip = (l4_umword_t)&handler;
  vcpu->entry_sp = sp & ~0xfUL;

  auto *s = vcpu.state();
  // disable trapping of CF1&2, CG and GT, enable ctl2
  s->guest_ctl_0 |= 0x3000083;
  s->guest_ctl_0_ext |= 0x10; // CGI
  l4_umword_t cca = s->g_cfg[0] & 7UL;
  s->g_seg_ctl[0] = 0x00200010;
  s->g_seg_ctl[1] = 0x00000002 | (cca << 16);
  s->g_seg_ctl[2] = 0x04300030 | (cca << 16) | cca;
  s->set_modified(L4_VM_MOD_GUEST_CTL_0
                  | L4_VM_MOD_GUEST_CTL_0_EXT
                  | L4_VM_MOD_CFG
                  | L4_VM_MOD_XLAT);

  Dbg(Dbg::Info).printf("Starting vmm @ 0x%lx (handler @ %p with stack @ %lx)\n",
                        vcpu->r.ip, &handler, sp);

  L4::Cap<L4::Thread> myself;
  auto e = l4_error(myself->vcpu_resume_commit(myself->vcpu_resume_start()));

  Err().printf("VMM exited with %ld\n", e);
}

int
Guest::dispatch_hypcall(Hypcall_code hypcall_code, Cpu &vcpu)
{
  switch (hypcall_code)
  {
  case Hypcall_outchar:
    _hypcall_print.print_char(vcpu->r.a0);
    return Jump_instr;

  };

  return -L4_ENOSYS;
}

void
Guest::handle_entry(Cpu vcpu)
{
  if (!(vcpu->r.status & (1UL << 3)))
    {
      Err().printf("Exception in entry handler. Halting. IP = 0x%lx\n",
                   vcpu->r.ip);
      halt_vm();
    }

  auto *utcb = l4_utcb();
  unsigned cause = (vcpu->r.cause >> 2) & 0x1F;
  auto *s = vcpu.state();
  unsigned exccode = (s->guest_ctl_0 >> 2) & 0x1f;

  if (0 && (cause != 27 || exccode != 2))
    Dbg(Dbg::Info).printf("VMM Entry. IP = 0x%lx, cause: 0x%lx(%d), ctl0: 0x%lx\n",
                        vcpu->r.ip, vcpu->r.cause, cause, s->guest_ctl_0);

  switch (cause)
    {
    case 0:
      handle_ipc(vcpu->i.tag, vcpu->i.label, utcb);
      break;
    case 1: // TLB modify
    case 2: // TLB load/fetch
    case 3: // TLB store
      if (!handle_mmio(vcpu->r.pfa, vcpu))
        {
          Err().printf("Bad page fault (%s) 0x%lx (GExcCode=0x%x) @0x%lx. Halting.\n",
                       cause == 2 ? "read" : "write", vcpu->r.pfa, exccode, vcpu->r.ip);
          halt_vm();
        }
      break;
    case 27: // guest exception
      {
        Mips::Instruction insn(vcpu->r.bad_instr);
        if (!insn.raw)
          {
            Err().printf("Cannot decode faulting instruction @ IP 0x%lx\n",
                         vcpu->r.ip);
            halt_vm();
          }

        int ret = -L4_ENOSYS;
        switch (exccode)
          {
          case 0: // sensitive instruction
            if (insn.is_mfc0())
              ret = handle_gpsi_mfc0(vcpu, insn);
            else if (insn.is_mtc0())
              ret = handle_gpsi_mtc0(vcpu, insn);
            else if (insn.is_wait())
              ret = handle_wait(vcpu, utcb);
            break;

          case 1: // software field change
            if (insn.is_mtc0())
              ret = handle_software_field_change(vcpu, insn);
            break;

          case 2: // hypcall
            if (insn.is_hypcall())
              ret = dispatch_hypcall((Hypcall_code)(unsigned)insn.hypcall_code(), vcpu);
            break;

          case 9: // hardware field change
            Dbg().printf("Hardware change @ IP 0x%lx\n", vcpu->r.ip);
            ret = 0; // ignored
            break;
          case 10:
            Err().printf("Bad TLB root access 0x%lx @0x%lx. Halting.\n",
                         vcpu->r.pfa, vcpu->r.ip);
            break;
          }

        if (ret < 0)
          {
            Err().printf("Guest exception %d, error: %d, inst: 0x%x @ IP 0x%lx\n",
                         exccode, ret, insn.raw, vcpu->r.ip);
            halt_vm();
          }
        if (ret == Jump_instr)
          vcpu.jump_instruction();
        break;
      }
    default:
      Err().printf("Unknown cause of VMM entry: %d. Halting.\n", cause);
      halt_vm();
    }

  process_pending_ipc(vcpu, utcb);
  _core_ic->update_vcpu(vcpu);

  L4::Cap<L4::Thread> myself;
  auto e = l4_error(myself->vcpu_resume_commit(myself->vcpu_resume_start()));

  Err().printf("VM restart failed with %ld\n", e);
  halt_vm();
}

} // namespace
