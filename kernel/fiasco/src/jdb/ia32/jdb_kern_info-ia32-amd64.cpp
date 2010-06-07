IMPLEMENTATION[ia32,amd64,ux]:

#include <cstdio>
#include <cstring>
#include "simpleio.h"

#include "config.h"
#include "cpu.h"
#include "gdt.h"
#include "idt.h"
#include "jdb_symbol.h"
#include "perf_cnt.h"
#include "pic.h"
#include "space.h"
#include "tss.h"


class Jdb_kern_info_idt : public Jdb_kern_info_module
{
};

static Jdb_kern_info_idt k_I INIT_PRIORITY(JDB_MODULE_INIT_PRIO+1);

PUBLIC
Jdb_kern_info_idt::Jdb_kern_info_idt()
  : Jdb_kern_info_module('I', "Interrupt Descriptor Table (IDT)")
{
  Jdb_kern_info::register_subcmd(this);
}

PUBLIC
void
Jdb_kern_info_idt::show()
{
  Pseudo_descriptor idt_pseudo;
  unsigned line = 0;

  Idt::get (&idt_pseudo);

  printf("idt base="L4_PTR_FMT"  limit=%04x (%04x bytes)\n",
	 idt_pseudo.base(), (idt_pseudo.limit()+1)/sizeof(Idt_entry), idt_pseudo.limit()+1);
  if (!Jdb_core::new_line(line))
    return;

  Idt_entry *ie = reinterpret_cast<Idt_entry*>(idt_pseudo.base());
  for (unsigned i=0; i<(idt_pseudo.limit()+1)/sizeof(Idt_entry); i++)
    {
      printf("%3x: ",i);
      ie[i].show();
      if (!Jdb_core::new_line(line))
	return;
    }
}


//---------------------------------------------------------------------------
IMPLEMENTATION[ia32,amd64]:

#include "io.h"


class Jdb_kern_info_pic_state : public Jdb_kern_info_module
{
};

static Jdb_kern_info_pic_state k_p INIT_PRIORITY(JDB_MODULE_INIT_PRIO+1);

PUBLIC
Jdb_kern_info_pic_state::Jdb_kern_info_pic_state()
  : Jdb_kern_info_module('p', "PIC ports")
{
  Jdb_kern_info::register_subcmd(this);
}

void
Jdb_kern_info_pic_state::show()
{
  int i;
  static char const hex[] = "0123456789ABCDEF";
	  
  // show important I/O ports
  Io::out8_p(Pic::OCW_TEMPLATE | Pic::READ_NEXT_RD | Pic::READ_IS_ONRD, 
	     Pic::MASTER_ICW );
  unsigned in_service = Io::in8(Pic::MASTER_ICW);
  Io::out8_p(Pic::OCW_TEMPLATE | Pic::READ_NEXT_RD | Pic::READ_IR_ONRD,
	     Pic::MASTER_ICW);
  unsigned requested = Io::in8(Pic::MASTER_ICW);
  unsigned mask = Jdb::pic_status & 0x0ff;
  printf("master PIC: in service:");
  for (i=7; i>=0; i--)
    putchar((in_service & (1<<i)) ? hex[i] : '-');
  printf(", request:");
  for (i=7; i>=0; i--)
    putchar((requested & (1<<i)) ? hex[i] : '-');
  printf(", mask:");
  for (i=7; i>=0; i--)
    putchar((mask & (1<<i)) ? hex[i] : '-');
  putchar('\n');
	  
  Io::out8_p( Pic::OCW_TEMPLATE | Pic::READ_NEXT_RD | Pic::READ_IS_ONRD, 
	      Pic::SLAVES_ICW);
  in_service = Io::in8(Pic::SLAVES_ICW);
  Io::out8_p( Pic::OCW_TEMPLATE | Pic::READ_NEXT_RD | Pic::READ_IR_ONRD, 
	      Pic::SLAVES_ICW);
  requested = Io::in8(Pic::SLAVES_ICW);
  mask = Jdb::pic_status >> 8;
  printf(" slave PIC: in service:");
  for (i=7; i>=0; i--)
    putchar((in_service & (1<<i)) ? hex[i+8] : '-');
  printf(", request:");
  for (i=7; i>=0; i--)
    putchar((requested & (1<<i)) ? hex[i+8] : '-');
  printf(", mask:");
  for (i=7; i>=0; i--)
    putchar((mask & (1<<i)) ? hex[i+8] : '-');
  putchar('\n');
}


class Jdb_kern_info_misc : public Jdb_kern_info_module
{
};

static Jdb_kern_info_misc k_i INIT_PRIORITY(JDB_MODULE_INIT_PRIO+1);

PUBLIC
Jdb_kern_info_misc::Jdb_kern_info_misc()
  : Jdb_kern_info_module('i', "Miscellaneous info")
{
  Jdb_kern_info::register_subcmd(this);
}

PUBLIC
void
Jdb_kern_info_misc::show()
{
  printf ("clck: %08x.%08x\n",
	  (unsigned) (Kip::k()->clock >> 32),
	  (unsigned) (Kip::k()->clock));

  show_pdir();

  Pseudo_descriptor gdt_pseudo, idt_pseudo;
  Gdt::get (&gdt_pseudo);
  Idt::get (&idt_pseudo);
  printf ("idt : base="L4_PTR_FMT"  limit=%04x\n"
	  "gdt : base="L4_PTR_FMT"  limit=%04x\n",
	  idt_pseudo.base(), (idt_pseudo.limit()+1)/8,
	  gdt_pseudo.base(), (gdt_pseudo.limit()+1)/8);

  // print LDT
  printf("ldt : %04x", Cpu::get_ldt());
  if (Cpu::get_ldt() != 0)
    {
      Gdt_entry *e = Cpu::boot_cpu()->get_gdt()->entries() + (Cpu::boot_cpu()->get_ldt() >> 3);
      printf(": "L4_PTR_FMT"-"L4_PTR_FMT,
	  e->base(), e->base()+ e->size());
    }

  // print TSS
  printf("\n"
	 "tr  : %04x", Cpu::boot_cpu()->get_tr());
  if(Cpu::get_tr() != 0)
    {
      Gdt_entry *e = Cpu::boot_cpu()->get_gdt()->entries() + (Cpu::boot_cpu()->get_tr() >> 3);
      printf(": "L4_PTR_FMT"-"L4_PTR_FMT", iobitmap at "L4_PTR_FMT, 
	  e->base(), e->base()+ e->size(),
	  e->base() + (reinterpret_cast<Tss *>(e->base())->_io_bit_map_offset));
    }
  printf("\n"
	 "cr0 : "L4_PTR_FMT"\n"
	 "cr4 : "L4_PTR_FMT"\n",
	 Cpu::get_cr0(), Cpu::get_cr4());
}

class Jdb_kern_info_cpu : public Jdb_kern_info_module
{
};

static Jdb_kern_info_cpu k_c INIT_PRIORITY(JDB_MODULE_INIT_PRIO+1);

PUBLIC
Jdb_kern_info_cpu::Jdb_kern_info_cpu()
  : Jdb_kern_info_module('c', "CPU features")
{
  Jdb_kern_info::register_subcmd(this);
}

PUBLIC
void
Jdb_kern_info_cpu::show()
{
  const char *perf_type = Perf_cnt::perf_type();
  char cpu_mhz[32];
  char time[32];
  unsigned hz;
  static char const * const scheduler_mode[] = { "PIT", "RTC", "APIC" };

  cpu_mhz[0] = '\0';
  if ((hz = Cpu::boot_cpu()->frequency()))
    {
      unsigned mhz = hz / 1000000;
      hz -= mhz * 1000000;
      unsigned khz = hz / 1000;
      snprintf(cpu_mhz, sizeof(cpu_mhz), "%d.%03d MHz", mhz, khz);
    }

  printf ("CPU: %s %s (%s)\n",
          Cpu::boot_cpu()->model_str(), cpu_mhz,
          Config::found_vmware ? "vmware" : "native");
  Cpu::boot_cpu()->show_cache_tlb_info("     ");
  show_features();

  if (Cpu::boot_cpu()->tsc())
    {
      Unsigned32 hour, min, sec, ns;
      Cpu::boot_cpu()->tsc_to_s_and_ns(Cpu::rdtsc(), &sec, &ns);
      hour = sec  / 3600;
      sec -= hour * 3600;
      min  = sec  / 60;
      sec -= min  * 60;
      snprintf(time, sizeof(time), "%02d:%02d:%02d.%06d", 
	       hour, min, sec, ns/1000);
    }
  else
    strcpy(time, "not available");

  printf("\nTimer interrupt source: %s (irq vector 0x%02x)"
	 "\nPerformance counters: %s"
	 "\nLast branch recording: %s"
	 "\nDebug store to memory: %s"
	 "\nTime stamp counter: %s"
	 "\n",
	 scheduler_mode[Config::scheduler_mode],
	 Config::scheduler_irq_vector,
	 perf_type ? perf_type : "no",
	 Cpu::boot_cpu()->lbr_type() != Cpu::Lbr_unsupported 
	    ? Cpu::boot_cpu()->lbr_type() == Cpu::Lbr_pentium_4 ? "P4" : "P6"
	    : "no",
	 Cpu::boot_cpu()->bts_type() != Cpu::Bts_unsupported
	    ? Cpu::boot_cpu()->bts_type() == Cpu::Bts_pentium_4 ? "P4" : "Pentium-M"
	    : "no",
	 time
	 );
}


class Jdb_kern_info_gdt : public Jdb_kern_info_module
{
private:
  static unsigned line;
};

static Jdb_kern_info_gdt k_g INIT_PRIORITY(JDB_MODULE_INIT_PRIO+1);

unsigned Jdb_kern_info_gdt::line;

PUBLIC
Jdb_kern_info_gdt::Jdb_kern_info_gdt()
  : Jdb_kern_info_module('g', "Global Descriptor Table (GDT)")
{
  Jdb_kern_info::register_subcmd(this);
}

PRIVATE static
void
Jdb_kern_info_gdt::show_gdt(unsigned cpu)
{
  Gdt *gdt = Cpu::cpus.cpu(cpu).get_gdt();
  unsigned entries = Gdt::gdt_max / 8;

  if (Config::Max_num_cpus > 1)
    printf("CPU%d: GDT base="L4_PTR_FMT"  limit=%04x (%04x bytes)\n",
           cpu, (Mword)gdt, entries, Gdt::gdt_max);
  else
    printf("GDT base="L4_PTR_FMT"  limit=%04x (%04x bytes)\n",
           (Mword)gdt, entries, Gdt::gdt_max);

  if (!Jdb_core::new_line(line))
    return;

  for (unsigned i = 0; i < entries; i++)
    {
      printf(" %02x: ", i * 8);
      (*gdt)[i].show();
      if (!Jdb_core::new_line(line))
	return;
    }
}

PUBLIC
void
Jdb_kern_info_gdt::show()
{
  line = 0;
  Jdb::foreach_cpu(&show_gdt);
}
