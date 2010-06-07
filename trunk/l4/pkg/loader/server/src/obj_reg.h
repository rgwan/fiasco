/*
 * (c) 2008-2009 Technische Universität Dresden
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include <l4/sys/task>
#include <l4/sys/factory>
#include <l4/sys/types.h>

#include <l4/re/util/cap_alloc>
#include <l4/re/env>
#include <l4/re/util/object_registry>

#include <l4/cxx/ipc_server>


namespace Gate_alloc {

  extern L4Re::Util::Object_registry registry;

}


