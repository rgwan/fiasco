/**
 * The functions regarding DDE/BSD initialization are found here.
 *
 * \author Thomas Friebel <tf13@os.inf.tu-dresden.de>
 */
#include <l4/dde/ddekit/panic.h>
#include <l4/dde/ddekit/thread.h>
#include <l4/dde/ddekit/memory.h>
#include <l4/dde/ddekit/interrupt.h>

#include <l4/dde/dde.h>
#include <l4/log/log.h>

#if 0
/* FIXME this must be initialized explicitly as some users may not need l4io,
 * e.g., l4io's own pcilib. */
static void ddekit_init_l4io(void)
{
	int err;
	l4io_info_t *ioip = NULL;

	LOGd(0, "mapping io info page to %p", ioip);
	err = l4io_init(&ioip, L4IO_DRV_INVALID);
	if ( err | !ioip ) {
		LOG("error initializing io lib: %s (err=%d, ioip=%p)", l4env_errstr(err), err, ioip);
		ddekit_panic("fatal error");
	}
}
#endif

extern void ddekit_pgtab_init(void);

void ddekit_init(void)
{
	ddekit_pgtab_init();
#if 0
	ddekit_init_l4io();
#endif
	ddekit_init_threads();
	ddekit_init_irqs();
}

