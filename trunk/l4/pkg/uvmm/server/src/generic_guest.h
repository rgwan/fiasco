/*
 * Copyright (C) 2015 Kernkonzept GmbH.
 * Author(s): Sarah Hoffmann <sarah.hoffmann@kernkonzept.com>
 *
 * This file is distributed under the terms of the GNU General Public
 * License, version 2.  Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include <l4/cxx/ref_ptr>
#include <l4/re/dataspace>
#include <l4/re/util/br_manager>
#include <l4/re/util/object_registry>
#include <l4/l4virtio/l4virtio>

#include "debug.h"
#include "device_tree.h"
#include "ds_mmio_mapper.h"
#include "irq.h"
#include "ram_ds.h"
#include "vm_memmap.h"
#include "vcpu.h"

namespace Vmm {

class Generic_guest
{
public:
    explicit Generic_guest(L4::Cap<L4Re::Dataspace> ram,
                           l4_addr_t vm_base = ~0UL, l4_addr_t boot_offset = 0);

    virtual ~Generic_guest() = default;

    Cpu create_cpu();

    Vdev::Device_tree device_tree() const
    { return Vdev::Device_tree(_ram.access(_device_tree)); }

    bool has_device_tree() const
    { return _device_tree.is_valid(); }

    Ram_ds &ram()
    { return _ram; }

    virtual void show_state_registers() = 0;
    virtual void show_state_interrupts() = 0;

    L4Re::Util::Object_registry *registry() { return &_registry; }

    void set_fallback_mmio_ds(L4::Cap<L4Re::Dataspace> ds)
    { _mmio_fallback = ds; }

    void register_mmio_device(cxx::Ref_ptr<Vmm::Mmio_device> &&dev,
                              Vdev::Dt_node const &node, int index = 0);

    l4_size_t load_ramdisk_at(char const *ram_disk, l4_addr_t offset);

protected:
    void load_device_tree_at(char const *src, l4_addr_t base, l4_size_t padding);

    void handle_ipc(l4_msgtag_t tag, l4_umword_t label, l4_utcb_t *utcb)
    {
      l4_msgtag_t r = _registry.dispatch(tag, label, utcb);
      if (r.label() != -L4_ENOREPLY)
        l4_ipc_send(L4_INVALID_CAP | L4_SYSF_REPLY, utcb, r,
                    L4_IPC_SEND_TIMEOUT_0);
    }

    void process_pending_ipc(Cpu vcpu, l4_utcb_t *utcb)
    {
      while (vcpu->sticky_flags & L4_VCPU_SF_IRQ_PENDING)
        {
          l4_umword_t src;
          _bm.setup_wait(utcb, L4::Ipc_svr::Reply_separate);
          l4_msgtag_t res = l4_ipc_wait(utcb, &src, L4_IPC_BOTH_TIMEOUT_0);
          if (!res.has_error())
            handle_ipc(res, src, utcb);
        }
    }

    bool handle_mmio(l4_addr_t pfa, Cpu vcpu)
    {
      Vm_mem::const_iterator f = _memmap.find(pfa);

      if (f != _memmap.end())
        return f->second->access(pfa, pfa - f->first.start,
                                 vcpu, _task.get(),
                                 f->first.start, f->first.end);

      if (!_mmio_fallback)
         return false;

      // Use the MMIO fallback dataspace to serve a 1:1 mapping.
      // This is necessary in some cases when guest use hardcoded
      // addresses to access devices instead of respecting
      // settings from the device tree.
      long res;
#if MAP_OTHER
      res = _mmio_fallback->map(pfa, L4Re::Dataspace::Map_rw, pfa,
                                l4_trunc_page(pfa), l4_round_page(pfa + 1),
                                _task.get());
#else
      l4_addr_t local_addr = 0;
      auto *e = L4Re::Env::env();
      res = e->rm()->reserve_area(&local_addr, L4_PAGESIZE,
                                  L4Re::Rm::Search_addr);
      if (res < 0)
        {
          Err().printf("VM memory fallback: VM allocation failure)\n");
          return false;
        }

      res = _mmio_fallback->map(pfa, L4Re::Dataspace::Map_rw, local_addr,
                                l4_trunc_page(local_addr),
                                l4_round_page(local_addr + 1));
      if (res < 0)
        {
          Err().printf("VM memory fallback: failure mapping into VMM\n");
          return false;
        }

      res = l4_error(_task->map(e->task(),
                     l4_fpage(local_addr, L4_PAGESHIFT, L4_FPAGE_RW),
                     l4_trunc_page(pfa)));
#endif /* MAP_OTHER */

      if (res < 0)
        Err().printf("VM memory fallback: map to VM failure\n");

      return res >= 0;
    }

    void wait_for_ipc(l4_utcb_t *utcb, l4_timeout_t to)
    {
      l4_umword_t src;
      l4_msgtag_t tag = l4_ipc_wait(utcb, &src, to);
      if (!tag.has_error())
        handle_ipc(tag, src, utcb);
    }

    void __attribute__((noreturn)) halt_vm()
    {
      // XXX Only halts the current CPU. For the SMP case some
      // further signaling might be required.
      Err().printf("VM entered a fatal state. Halting.\n");
      for(;;)
        wait_for_ipc(l4_utcb(), L4_IPC_NEVER);
    }

    L4Re::Util::Br_manager _bm;
    L4Re::Util::Object_registry _registry;
    Vm_mem _memmap;
    Ram_ds _ram;
    L4Re::Util::Auto_cap<L4::Task>::Cap _task;
    L4virtio::Ptr<void> _device_tree;

protected:
    enum { Nr_cpus = 1 };
    Cpu *_vcpu[Nr_cpus];
    L4::Cap<L4Re::Dataspace> _mmio_fallback;
};

} // namespace
