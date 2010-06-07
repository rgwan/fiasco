/*
 * (c) 2008-2009 Technische Universität Dresden
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include <l4/sys/types.h>
#include <l4/sys/kip.h>

#include <l4/cxx/ipc_server>

#include "obj_reg.h"

namespace Moe { class Dataspace; };

extern L4::Cap<void> root_name_space_obj;
extern Moe::Dataspace *kip_ds;
extern l4_kernel_info_t *_current_kip;
inline l4_kernel_info_t const *kip() { return _current_kip; }

extern Object_pool object_pool;
extern char const *const PROG;
extern char log_buffer[1024];

namespace Moe {
  extern unsigned l4re_dbg;
  extern unsigned ldr_flags;
}

enum
{
  My_task_cap    = L4_BASE_TASK_CAP,
  My_factory_cap = L4_BASE_FACTORY_CAP,
  My_self_cap    = L4_BASE_THREAD_CAP,
  Sigma0_cap     = L4_BASE_PAGER_CAP,
};
