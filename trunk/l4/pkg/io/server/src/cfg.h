/*
 * (c) 2010 Technische Universität Dresden
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */

#pragma once

namespace Hw {
  class Device;
}

class Io_config
{
public:
  virtual bool transparent_msi(Hw::Device *) const = 0;
  virtual bool legacy_ide_resources(Hw::Device *) const = 0;
  virtual bool expansion_rom(Hw::Device *) const = 0;
  virtual int verbose() const = 0;

  static Io_config *cfg;
  virtual ~Io_config() {}
};
