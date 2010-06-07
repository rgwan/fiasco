/*
 * (c) 2010 Technische Universität Dresden
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 */
#pragma once

#include <l4/l4re_vfs/backend>
#include <l4/sys/capability>
#include <l4/re/namespace>
#include "simple_store.h"

namespace L4Re { namespace Core {

using cxx::Ref_ptr;

class Ns_base_dir : public L4Re::Vfs::Be_file
{
public:
  enum { Size = sizeof(L4Re::Vfs::Be_file) + sizeof(l4_addr_t) };

  void *operator new(size_t s) throw();
  void operator delete(void *b) throw();

protected:
  static Simple_store_sz<Size> store;
};

class Env_dir : public Ns_base_dir
{
public:
  explicit Env_dir(L4Re::Env const *env) : _env(env) {}

  ssize_t readv(const struct iovec*, int) throw() { return -EISDIR; }
  ssize_t writev(const struct iovec*, int) throw() { return -EISDIR; }
  int fstat64(struct stat64 *) const throw() { return -EINVAL; }
  int faccessat(const char *path, int mode, int flags) throw();
  int get_entry(const char *path, int flags, mode_t mode,
                Ref_ptr<L4Re::Vfs::File> *) throw();

  ~Env_dir() throw() {}


private:
  int get_ds(const char *path, L4Re::Auto_cap<L4Re::Dataspace>::Cap *ds) throw();

  L4Re::Env const *_env;

};

class Ns_dir : public Ns_base_dir
{
public:
  explicit Ns_dir(L4::Cap<L4Re::Namespace> ns) : _ns(ns) {}

  ssize_t readv(const struct iovec*, int) throw() { return -EISDIR; }
  ssize_t writev(const struct iovec*, int) throw() { return -EISDIR; }
  int fstat64(struct stat64 *) const throw() { return -EINVAL; }
  int faccessat(const char *path, int mode, int flags) throw();
  int get_entry(const char *path, int flags, mode_t mode,
                Ref_ptr<L4Re::Vfs::File> *) throw();

  ~Ns_dir() throw() {}

private:
  int get_ds(const char *path, L4Re::Auto_cap<L4Re::Dataspace>::Cap *ds) throw();

  L4::Cap<L4Re::Namespace> _ns;

};

}}
