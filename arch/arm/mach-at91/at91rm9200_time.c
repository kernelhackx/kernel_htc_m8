/*
 * linux/arch/arm/mach-at91/at91rm9200_time.c
 *
 *  Copyright (C) 2003 SAN People
 *  Copyright (C) 2003 ATMEL
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/clockchips.h>
#include <linux/export.h>

#include <asm/mach/time.h>

#include <mach/at91_st.h>

static unsigned long last_crtr;
static u32 irqmask;
static struct clock_event_device clkevt;

#define RM9200_TIMER_LATCH	((AT91_SLOW_CLOCK + HZ/2) / HZ)

static inline unsigned long read_CRTR(void)
{
	unsigned long x1, x2;

	x1 = at91_st_read(AT91_ST_CRTR);
	do {
		x2 = at91_st_read(AT91_ST_CRTR);
		if (x1 == x2)
			break;
		x1 = x2;
	} while (1);
	return x1;
}

static irqreturn_t at91rm9200_timer_interrupt(int irq, void *dev_id)
{
	u32	sr = at91_st_read(AT91_ST_SR) & irqmask;

	WARN_ON_ONCE(!irqs_disabled());

	
	if (sr & AT91_ST_ALMS) {
		clkevt.event_handler(&clkevt);
		return IRQ_HANDLED;
	}

	
	if (sr & AT91_ST_PITS) {
		u32	crtr = read_CRTR();

		while (((crtr - last_crtr) & AT91_ST_CRTV) >= RM9200_TIMER_LATCH) {
			last_crtr += RM9200_TIMER_LATCH;
			clkevt.event_handler(&clkevt);
		}
		return IRQ_HANDLED;
	}

	
	return IRQ_NONE;
}

static struct irqaction at91rm9200_timer_irq = {
	.name		= "at91_tick",
	.flags		= IRQF_SHARED | IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= at91rm9200_timer_interrupt
};

static cycle_t read_clk32k(struct clocksource *cs)
{
	return read_CRTR();
}

static struct clocksource clk32k = {
	.name		= "32k_counter",
	.rating		= 150,
	.read		= read_clk32k,
	.mask		= CLOCKSOURCE_MASK(20),
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

static void
clkevt32k_mode(enum clock_event_mode mode, struct clock_event_device *dev)
{
	
	at91_st_write(AT91_ST_IDR, AT91_ST_PITS | AT91_ST_ALMS);
	at91_st_read(AT91_ST_SR);

	last_crtr = read_CRTR();
	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		
		irqmask = AT91_ST_PITS;
		at91_st_write(AT91_ST_PIMR, RM9200_TIMER_LATCH);
		break;
	case CLOCK_EVT_MODE_ONESHOT:
		irqmask = AT91_ST_ALMS;
		at91_st_write(AT91_ST_RTAR, last_crtr);
		break;
	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_RESUME:
		irqmask = 0;
		break;
	}
	at91_st_write(AT91_ST_IER, irqmask);
}

static int
clkevt32k_next_event(unsigned long delta, struct clock_event_device *dev)
{
	u32		alm;
	int		status = 0;

	BUG_ON(delta < 2);

	alm = read_CRTR();

	
	at91_st_write(AT91_ST_RTAR, alm);
	at91_st_read(AT91_ST_SR);

	
	alm += delta;
	at91_st_write(AT91_ST_RTAR, alm);

	return status;
}

static struct clock_event_device clkevt = {
	.name		= "at91_tick",
	.features	= CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.shift		= 32,
	.rating		= 150,
	.set_next_event	= clkevt32k_next_event,
	.set_mode	= clkevt32k_mode,
};

void __iomem *at91_st_base;
EXPORT_SYMBOL_GPL(at91_st_base);

void __init at91rm9200_ioremap_st(u32 addr)
{
	at91_st_base = ioremap(addr, 256);
	if (!at91_st_base)
		panic("Impossible to ioremap ST\n");
}

void __init at91rm9200_timer_init(void)
{
	
	at91_st_write(AT91_ST_IDR,
		AT91_ST_PITS | AT91_ST_WDOVF | AT91_ST_RTTINC | AT91_ST_ALMS);
	at91_st_read(AT91_ST_SR);

	
	setup_irq(AT91_ID_SYS, &at91rm9200_timer_irq);

	at91_st_write(AT91_ST_RTMR, 1);

	
	clkevt.mult = div_sc(AT91_SLOW_CLOCK, NSEC_PER_SEC, clkevt.shift);
	clkevt.max_delta_ns = clockevent_delta2ns(AT91_ST_ALMV, &clkevt);
	clkevt.min_delta_ns = clockevent_delta2ns(2, &clkevt) + 1;
	clkevt.cpumask = cpumask_of(0);
	clockevents_register_device(&clkevt);

	
	clocksource_register_hz(&clk32k, AT91_SLOW_CLOCK);
}

struct sys_timer at91rm9200_timer = {
	.init		= at91rm9200_timer_init,
};
