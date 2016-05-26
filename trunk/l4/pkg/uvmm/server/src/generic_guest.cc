/*
 * Copyright (C) 2015 Kernkonzept GmbH.
 * Author(s): Sarah Hoffmann <sarah.hoffmann@kernkonzept.com>
 *
 * This file is distributed under the terms of the GNU General Public
 * License, version 2.  Please see the COPYING-GPL-2 file for details.
 */

#include <l4/sys/debugger.h>

#include "generic_guest.h"
#include "dev_sysctl.h"
#include "virtio_console.h"
#include "virtio_proxy.h"

namespace Vmm {

Generic_guest::Generic_guest(L4::Cap<L4Re::Dataspace> ram,
                             l4_addr_t vm_base, l4_addr_t boot_offset)
: _registry(&_bm),
  _ram(ram, vm_base, boot_offset),
  _task(L4Re::chkcap(L4Re::Util::cap_alloc.alloc<L4::Task>()))
{
  // attach RAM to VM
  _memmap[Region::ss(_ram.vm_start(), _ram.size())]
    = Vdev::make_device<Ds_handler>(_ram.ram(), _ram.local_start());

  // create the VM task
  auto *e = L4Re::Env::env();
  L4Re::chksys(e->factory()->create(_task.get(), L4_PROTO_VM),
               "allocate vm");
  l4_debugger_set_object_name(_task.get().cap(), "vm-task");
}

Cpu
Generic_guest::create_cpu()
{
  auto *e = L4Re::Env::env();
  l4_addr_t vcpu_addr = 0x10000000;

  L4Re::chksys(e->rm()->reserve_area(&vcpu_addr, L4_PAGESIZE,
                                     L4Re::Rm::Search_addr));
  L4Re::chksys(e->task()->add_ku_mem(l4_fpage(vcpu_addr, L4_PAGESHIFT,
                                              L4_FPAGE_RWX)),
               "kumem alloc");

  Cpu vcpu = Cpu((l4_vcpu_state_t *)vcpu_addr);
  vcpu.thread_attach();
  vcpu->user_task = _task.get().cap();

  return vcpu;
}

void
Generic_guest::load_device_tree_at(char const *name, l4_addr_t base,
                                 l4_size_t padding)
{
  _device_tree = _ram.load_file(name, base);

  auto dt = device_tree();
  dt.check_tree();
  dt.add_to_size(padding);

  // fill in memory node in the device tree
  auto mem_nd = dt.path_offset("/memory");
  mem_nd.setprop_u32("reg", _ram.vm_start());
  mem_nd.appendprop_u32("reg", _ram.size());
}

l4_size_t
Generic_guest::load_ramdisk_at(char const *ram_disk, l4_addr_t offset)
{
  Dbg info(Dbg::Info);
  info.printf("load ramdisk image %s\n", ram_disk);

  l4_size_t size;
  auto initrd = _ram.load_file(ram_disk, offset, &size);

  if (offset + size > _ram.size())
    L4Re::chksys(-L4_EINVAL, "Ramdisk does not fit into RAM.");

  if (has_device_tree()
      && device_tree().overlaps(_ram.local_start() + offset,
                                _ram.local_start() + offset + size))
    L4Re::chksys(-L4_EINVAL, "Ramdisk overlaps with device tree.");


  if (has_device_tree())
    {
      auto node = device_tree().path_offset("/chosen");
      node.setprop_u32("linux,initrd-start", _ram.boot_addr(initrd));
      node.setprop_u32("linux,initrd-end", _ram.boot_addr(initrd) + size);
    }

  return size;
}

void
Generic_guest::register_mmio_device(cxx::Ref_ptr<Vmm::Mmio_device> &&dev,
                                    Vdev::Dt_node const &node, int index)
{
  // XXX need to check for address-cells and size-cells here
  auto *prop = node.check_prop<fdt32_t>("reg", 2 * index);
  uint32_t base = fdt32_to_cpu(prop[index * 2]);
  uint32_t size = fdt32_to_cpu(prop[index * 2 + 1]);

  _memmap[Region::ss(base, size)] = dev;

  Dbg().printf("New mmio mapping: @ %x %x\n", base, size);
}

} // namespace
