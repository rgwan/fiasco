#ifndef _ASM_X86_MACH_DEFAULT_MACH_MPSPEC_H
#define _ASM_X86_MACH_DEFAULT_MACH_MPSPEC_H

#define MAX_IRQ_SOURCES 256

#if CONFIG_BASE_SMALL == 0
#define MAX_MP_BUSSES 256
#else
#define MAX_MP_BUSSES 32
#endif

#endif /* _ASM_X86_MACH_DEFAULT_MACH_MPSPEC_H */
