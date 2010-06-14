/*
 * (c) 2010 Technische Universität Dresden
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */

#include <l4/sys/vhw.h>
#include <l4/re/env.h>

#include "ux.h"

#include "hw_device.h"

#include <cstdio>

void ux_setup(Hw::Device *bus)
{

  struct l4_vhw_descriptor *vhw;
  struct l4_vhw_entry *vhwe;

  if (!(vhw = l4_vhw_get(l4re_kip())))
    {
      printf("%s: No VHWs found.\n", __func__);
      return;
    }

  if ((vhwe = l4_vhw_get_entry_type(vhw, L4_TYPE_VHW_INPUT))
      && vhwe->irq_no)
    {
      Hw::Device *input = new Hw::Device();

       if (!input)
         printf("Failed to allocate device for 'L4UXinput'\n");
       else
         {
	   input->set_hid("L4UXinput");
           input->add_resource
             (new Adr_resource(Adr_resource::Mmio_res
                               | Resource::F_fixed_size | Resource::F_fixed_addr,
                               vhwe->mem_start,
                               vhwe->mem_start + vhwe->mem_size - 1));
           input->add_resource
             (new Adr_resource(Adr_resource::Irq_res,
                               vhwe->irq_no, vhwe->irq_no));
          bus->add_child(input);
         }
    }

  if ((vhwe = l4_vhw_get_entry_type(vhw, L4_TYPE_VHW_FRAMEBUFFER)))
    {
      Hw::Device *fb = new Hw::Device();

      if (!fb)
        printf("Failed to allocate device for 'L4UXfb'\n");
      else
        {
	  fb->set_hid("L4UXfb");
          fb->add_resource
            (new Adr_resource(Adr_resource::Mmio_res
                              | Resource::F_fixed_size | Resource::F_fixed_addr,
                              vhwe->mem_start,
                              vhwe->mem_start + vhwe->mem_size - 1));
          bus->add_child(fb);
        }
    }

  if ((vhwe = l4_vhw_get_entry_type(vhw, L4_TYPE_VHW_NET)))
    {
      Hw::Device *net = new Hw::Device();

      if (!net)
        printf("Failed to allocate device for 'L4UXnet'\n");
      else
        {
          net->set_hid("L4UXnet");
          net->add_resource
            (new Adr_resource(Adr_resource::Irq_res,
                              vhwe->irq_no, vhwe->irq_no));
          net->add_resource
            (new Adr_resource(Adr_resource::Io_res,
                              vhwe->fd, vhwe->provider_pid));
          bus->add_child(net);
        }
    }
}