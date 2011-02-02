INTERFACE [arm]:

#include "io.h"
#include "mem_layout.h"
#include "mem_unit.h"
#include "types.h"
#include "per_cpu_data.h"
#include "processor.h"

EXTENSION
class Cpu
{
public:
  void init(bool is_boot_cpu = false);

  static void early_init();

  static Per_cpu<Cpu> cpus;
  static Cpu *boot_cpu() { return _boot_cpu; }

  enum {
    Cp15_c1_mmu             = 1 << 0,
    Cp15_c1_alignment_check = 1 << 1,
    Cp15_c1_cache           = 1 << 2,
    Cp15_c1_branch_predict  = 1 << 11,
    Cp15_c1_insn_cache      = 1 << 12,
    Cp15_c1_high_vector     = 1 << 13,
  };

  Cpu(unsigned id) { set_id(id); }

private:
  static Cpu *_boot_cpu;

  unsigned _phys_id;
};

// ------------------------------------------------------------------------
INTERFACE [arm && armv5]:

EXTENSION class Cpu
{
public:
  enum {
    Cp15_c1_write_buffer    = 1 << 3,
    Cp15_c1_prog32          = 1 << 4,
    Cp15_c1_data32          = 1 << 5,
    Cp15_c1_late_abort      = 1 << 6,
    Cp15_c1_big_endian      = 1 << 7,
    Cp15_c1_system_protect  = 1 << 8,
    Cp15_c1_rom_protect     = 1 << 9,
    Cp15_c1_f               = 1 << 10,
    Cp15_c1_rr              = 1 << 14,
    Cp15_c1_l4              = 1 << 15,

    Cp15_c1_generic         = Cp15_c1_mmu
                              | (Config::Cp15_c1_use_alignment_check ?  Cp15_c1_alignment_check : 0)
                              | Cp15_c1_write_buffer
                              | Cp15_c1_prog32
                              | Cp15_c1_data32
                              | Cp15_c1_late_abort
                              | Cp15_c1_rom_protect
                              | Cp15_c1_high_vector,

    Cp15_c1_cache_bits      = Cp15_c1_cache
                              | Cp15_c1_insn_cache
                              | Cp15_c1_write_buffer,

  };
};

INTERFACE [arm && armv6]:

EXTENSION class Cpu
{
public:
  enum {
    Cp15_c1_l4              = 1 << 15,
    Cp15_c1_u               = 1 << 22,
    Cp15_c1_xp              = 1 << 23,
    Cp15_c1_ee              = 1 << 25,
    Cp15_c1_nmfi            = 1 << 27,
    Cp15_c1_tex             = 1 << 28,
    Cp15_c1_force_ap        = 1 << 29,

    Cp15_c1_generic         = Cp15_c1_mmu
                              | (Config::Cp15_c1_use_alignment_check ?  Cp15_c1_alignment_check : 0)
			      | Cp15_c1_branch_predict
			      | Cp15_c1_high_vector
                              | Cp15_c1_u
			      | Cp15_c1_xp,

    Cp15_c1_cache_bits      = Cp15_c1_cache
                              | Cp15_c1_insn_cache,

    Smp_enable              = 0x20,
  };
};

INTERFACE [arm && armv7 && armca8]:

EXTENSION class Cpu
{
public:
  enum {
    Cp15_c1_ee              = 1 << 25,
    Cp15_c1_nmfi            = 1 << 27,
    Cp15_c1_tre             = 1 << 28,
    Cp15_c1_afe             = 1 << 29,
    Cp15_c1_te              = 1 << 30,

    Cp15_c1_cache_bits      = Cp15_c1_cache
                              | Cp15_c1_insn_cache,

    Cp15_c1_generic         = Cp15_c1_mmu
                              | (Config::Cp15_c1_use_alignment_check ?  Cp15_c1_alignment_check : 0)
			      | Cp15_c1_branch_predict
			      | Cp15_c1_high_vector,
  };
};

INTERFACE [arm && armv7 && armca9]:

EXTENSION class Cpu
{
public:
  enum {
    Cp15_c1_sw              = 1 << 10,
    Cp15_c1_ha              = 1 << 17,
    Cp15_c1_ee              = 1 << 25,
    Cp15_c1_nmfi            = 1 << 27,
    Cp15_c1_tre             = 1 << 28,
    Cp15_c1_afe             = 1 << 29,
    Cp15_c1_te              = 1 << 30,

    Cp15_c1_cache_bits      = Cp15_c1_cache
                              | Cp15_c1_insn_cache,

    Cp15_c1_generic         = Cp15_c1_mmu
                              | (Config::Cp15_c1_use_alignment_check ?  Cp15_c1_alignment_check : 0)
			      | Cp15_c1_branch_predict
			      | Cp15_c1_high_vector
			      | (Config::Cp15_c1_use_a9_swp_enable ?  Cp15_c1_sw : 0),

    Smp_enable              = 0x41,
  };
};

INTERFACE [arm]:

EXTENSION class Cpu
{
public:
  enum {
    Cp15_c1_cache_enabled  = Cp15_c1_generic | Cp15_c1_cache_bits,
    Cp15_c1_cache_disabled = Cp15_c1_generic,
  };
};

//---------------------------------------------------------------------------
IMPLEMENTATION [arm && (mpcore || armca9)]:

PUBLIC static inline void
Cpu::early_init_platform()
{
  Io::write<Mword>(0xffffffff, Mem_layout::Mp_scu_map_base + 0xc);
  Io::write<Mword>(1,          Mem_layout::Mp_scu_map_base + 0);

  Io::write<Mword>(Io::read<Mword>(Mem_layout::Gic_cpu_map_base + 0) | 1,
                   Mem_layout::Gic_cpu_map_base + 0);
  Io::write<Mword>(Io::read<Mword>(Mem_layout::Gic_dist_map_base + 0) | 1,
                   Mem_layout::Gic_dist_map_base + 0);

  Mem_unit::clean_dcache();

  Mword tmp = 0;
  __asm__ __volatile__("mrc p15, 0, %0, c1, c0, 1   \n"
                       "orr %0, %1                  \n"
                       "mcr p15, 0, %0, c1, c0, 1   \n"
                       : : "r" (tmp), "i" (Smp_enable));
}

//---------------------------------------------------------------------------
IMPLEMENTATION [arm && !(mpcore || armca9)]:

PUBLIC static inline void Cpu::early_init_platform()
{}

//---------------------------------------------------------------------------
IMPLEMENTATION [arm]:

#include <cstdio>
#include <cstring>
#include <panic.h>

#include "io.h"
#include "pagetable.h"
#include "kmem_space.h"
#include "mem_unit.h"
#include "processor.h"
#include "ram_quota.h"

Per_cpu<Cpu> DEFINE_PER_CPU_P(0) Cpu::cpus(true);
Cpu *Cpu::_boot_cpu;

PUBLIC static inline
Mword
Cpu::stack_align(Mword stack)
{ return stack & ~0x3; }


IMPLEMENT
void Cpu::early_init()
{
  // switch to supervisor mode and intialize the memory system
  asm volatile ( " mov  r2, r13             \n"
                 " mov  r3, r14             \n"
                 " msr  cpsr_c, %1          \n"
                 " mov  r13, r2             \n"
                 " mov  r14, r3             \n"

                 " mcr  p15, 0, %0, c1, c0  \n"
                 :
                 : "r" (Config::cache_enabled
                        ? Cp15_c1_cache_enabled : Cp15_c1_cache_disabled),
                   "I" (0x0d3)
                 : "r2", "r3");

  early_init_platform();

  Mem_unit::flush_cache();

  print_infos();
}


PUBLIC static inline
bool
Cpu::have_superpages()
{ return true; }

PUBLIC static inline
void
Cpu::debugctl_enable()
{}

PUBLIC static inline
void
Cpu::debugctl_disable()
{}

PUBLIC static inline NEEDS["types.h"]
Unsigned32
Cpu::get_scaler_tsc_to_ns()
{ return 0; }

PUBLIC static inline NEEDS["types.h"]
Unsigned32
Cpu::get_scaler_tsc_to_us()
{ return 0; }

PUBLIC static inline NEEDS["types.h"]
Unsigned32
Cpu::get_scaler_ns_to_tsc()
{ return 0; }

PUBLIC static inline
bool
Cpu::tsc()
{ return 0; }

PUBLIC static inline
Unsigned64
Cpu::rdtsc (void)
{ return 0; }

PUBLIC static
void Cpu::init_mmu()
{
  extern char ivt_start;
  // map the interrupt vector table to 0xffff0000
  Pte pte = Kmem_space::kdir()->walk((void*)Kmem_space::Ivt_base, 4096,
      true, Ram_quota::root);

  pte.set((unsigned long)&ivt_start, 4096, 
      Mem_page_attr(Page::KERN_RW | Page::CACHEABLE),
      true);

  Mem_unit::tlb_flush();
}

IMPLEMENT inline
unsigned
Cpu::phys_id() const
{ return _phys_id; }

IMPLEMENT
void
Cpu::init(bool is_boot_cpu)
{
  if (is_boot_cpu)
    {
      _boot_cpu = this;
      set_online(1);
    }

  _phys_id = Proc::cpu_id();

  init_tz();
}

//---------------------------------------------------------------------------
IMPLEMENTATION [arm && !tz]:

PRIVATE static inline
void
Cpu::init_tz()
{}

//---------------------------------------------------------------------------
INTERFACE [arm && tz]:

EXTENSION class Cpu
{
public:

  static char monitor_vector_base asm ("monitor_vector_base");
};

//---------------------------------------------------------------------------
IMPLEMENTATION [arm && tz]:

PRIVATE inline
void
Cpu::init_tz()
{
  // set monitor vector base address
  assert(!((Mword)&monitor_vector_base & 31));
  tz_mvbar((Mword)&monitor_vector_base);

  // enable nonsecure access to vfp coprocessor
  asm volatile("mov r0, #0xc00;"
               "mcr p15, 0, r0, c1, c1, 2;"
               : : : "r0"
              );

  enable_irq_ovrr();
}

PUBLIC inline
void
Cpu::tz_switch_to_ns(Mword *nonsecure_state)
{
  volatile register Mword r0 asm("r0") = (Mword)nonsecure_state;
  extern char go_nonsecure;

  asm volatile("stmdb sp!, {fp}   \n"
               "stmdb sp!, {r0}   \n"
               "mov    r2, sp     \n" // copy sp_svc to sp_mon
               "cps    #0x16      \n" // switch to monitor mode
               "mov    sp, r2     \n"
               "adr    r3, 1f     \n" // save return eip
               "mrs    r4, cpsr   \n" // save return psr
               "mov    pc, r1     \n" // go nonsecure!
               "1:                \n"
               "mov    r0, sp     \n" // copy sp_mon to sp_svc
               "cps    #0x13      \n" // switch to svc mode
               "mov    sp, r0     \n"
               "ldmia  sp!, {r0}  \n"
               "ldmia  sp!, {fp}  \n"
               : : "r" (r0), "r" (&go_nonsecure)
               : "r2", "r3", "r4", "r5", "r6", "r7",
                 "r8", "r9", "r10", "r12", "r14", "memory");
}

PUBLIC static inline
Mword
Cpu::tz_scr()
{
  Mword r;
  asm volatile ("mrc p15, 0, %0, c1, c1, 0" : "=r" (r));
  return r;
}

PUBLIC static inline
void
Cpu::tz_scr(Mword val)
{
  asm volatile ("mcr p15, 0, %0, c1, c1, 0" : : "r" (val));
}

PUBLIC static inline
Mword
Cpu::tz_mvbar()
{
  Mword r;
  asm volatile ("mrc p15, 0, %0, c12, c0, 1" : "=r" (r));
  return r;
}

PUBLIC static inline
void
Cpu::tz_mvbar(Mword val)
{
  asm volatile ("mcr p15, 0, %0, c12, c0, 1" : : "r" (val));
}

// ------------------------------------------------------------------------
IMPLEMENTATION [arm && tz && armca9]:

PUBLIC static inline
void
Cpu::enable_irq_ovrr()
{
  // set IRQ/FIQ/Abort override bits
  asm volatile("mov r0, #0x1c0            \n"
               "mcr p15, 0, r0, c1, c1, 3 \n"
               : : : "r0");
}

IMPLEMENTATION [!tz || !armca9]:

PUBLIC static inline
void
Cpu::enable_irq_ovrr()
{}

// ------------------------------------------------------------------------
IMPLEMENTATION [!debug]:

PRIVATE static inline
void
Cpu::print_infos()
{}

// ------------------------------------------------------------------------
IMPLEMENTATION [debug]:

PRIVATE static
void
Cpu::print_infos()
{
  printf("Cache config: %s\n", Config::cache_enabled ? "ON" : "OFF");
}