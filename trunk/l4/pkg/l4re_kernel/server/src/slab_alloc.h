/*
 * (c) 2008-2009 Technische Universität Dresden
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include <l4/cxx/slab_alloc>

#include "page_alloc.h"

template< typename Type >
class Slab_alloc 
: public cxx::Slab_static<Type, L4_PAGESIZE, 2, Single_page_alloc>
{};

