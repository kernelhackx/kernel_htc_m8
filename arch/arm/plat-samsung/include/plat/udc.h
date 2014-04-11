/* arch/arm/plat-samsung/include/plat/udc.h
 *
 * Copyright (c) 2005 Arnaud Patard <arnaud.patard@rtp-net.org>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 *  Changelog:
 *	14-Mar-2005	RTP	Created file
 *	02-Aug-2005	RTP	File rename
 *	07-Sep-2005	BJD	Minor cleanups, changed cmd to enum
 *	18-Jan-2007	HMW	Add per-platform vbus_draw function
*/

#ifndef __ASM_ARM_ARCH_UDC_H
#define __ASM_ARM_ARCH_UDC_H

enum s3c2410_udc_cmd_e {
	S3C2410_UDC_P_ENABLE	= 1,	
	S3C2410_UDC_P_DISABLE	= 2,	
	S3C2410_UDC_P_RESET	= 3,	
};

struct s3c2410_udc_mach_info {
	void	(*udc_command)(enum s3c2410_udc_cmd_e);
	void	(*vbus_draw)(unsigned int ma);

	unsigned int pullup_pin;
	unsigned int pullup_pin_inverted;

	unsigned int vbus_pin;
	unsigned char vbus_pin_inverted;
};

extern void __init s3c24xx_udc_set_platdata(struct s3c2410_udc_mach_info *);

struct s3c24xx_hsudc_platdata;

extern void __init s3c24xx_hsudc_set_platdata(struct s3c24xx_hsudc_platdata *pd);

#endif 