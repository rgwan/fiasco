/*
 * (c) 2008-2009 Alexander Warg <warg@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include <l4/sys/types.h>
#include <l4/re/l4aux.h>
#include <cstddef>
#include <l4/libloader/remote_mem>

class Region_map;

namespace Moe {

class Dataspace;

class Stack : public Ldr::Remote_stack<>
{
public:
  explicit Stack(char *p = 0) : Ldr::Remote_stack<>(p) {}
  l4_addr_t add(l4_addr_t start, l4_umword_t size, Region_map *rm,
                Moe::Dataspace const *m, unsigned long offs,
                unsigned flags, unsigned char align,
                char const *what);
};

}
