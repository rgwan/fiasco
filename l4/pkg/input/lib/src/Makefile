PKGDIR ?= ../..
L4DIR  ?= $(PKGDIR)/../..

TARGET = libinput.a
SRC_S  = jiffie_hack.S
SRC_C = emul_time.c emul_irq.c emul_wait.c emul_res.c \
        l4evdev.c init.c proxy.c l4drv.c l4drv_reg.c l4bus.c
REQUIRES_LIBS = libio libirq l4util libc

PRIVATE_INCDIR = $(PKGDIR)/lib/include

DEFINES  = -D__KERNEL__ -DL4INPUT
# for pthread locks in semaphore.h
DEFINES += -D_XOPEN_SOURCE=500
WARNINGS = -Wall -Wstrict-prototypes $(call checkcc,-Wno-pointer-sign)

# gather Linux sources
VPATH += $(PKGDIR)/lib/contrib

SRCALL = input/input.c input/keyboard/atkbd.c input/mouse/psmouse-base.c \
         input/mouse/logips2pp.c input/mouse/synaptics.c input/mouse/alps.c \
         input/serio/serio.c input/serio/libps2.c

SRC_C_x86-l4f   += $(SRCALL) input/serio/i8042.c input/misc/pcspkr.c ux.c
SRC_C_amd64-l4f += $(SRCALL) input/serio/i8042.c input/misc/pcspkr.c
SRC_C_arm-l4f   += $(SRCALL) input/serio/ambakmi.c
SRC_C_ppc32-l4f += $(SRCALL)

include $(L4DIR)/mk/lib.mk