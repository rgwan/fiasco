/*
 * (c) 2010 Technische Universität Dresden
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include <l4/sys/types.h>
#include <l4/re/video/goos>

void splash_display(L4Re::Video::Goos::Info *fb_info, l4_addr_t fbaddr);
