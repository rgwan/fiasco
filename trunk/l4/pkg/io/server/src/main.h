/*
 * (c) 2010 Technische Universität Dresden
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include <l4/sys/capability>
#include <l4/sys/icu>

namespace Hw {
class Device;
}

class Hw_icu
{
public:
  L4::Cap<L4::Icu> icu;
  L4::Icu::Info info;

  Hw_icu();
};

Hw::Device *system_bus();
Hw_icu *system_icu();

