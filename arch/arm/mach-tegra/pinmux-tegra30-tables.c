/*
 * linux/arch/arm/mach-tegra/pinmux-tegra30-tables.c
 *
 * Common pinmux configurations for Tegra30 SoCs
 *
 * Copyright (C) 2010,2011 NVIDIA Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/init.h>
#include <linux/string.h>

#include <mach/iomap.h>
#include <mach/pinmux.h>
#include <mach/pinmux-tegra30.h>
#include <mach/suspend.h>

#define PINGROUP_REG_A	0x868
#define MUXCTL_REG_A	0x3000

#define DRIVE_PINGROUP(pg_name, r)		\
	[TEGRA_DRIVE_PINGROUP_ ## pg_name] = {	\
		.name = #pg_name,		\
		.reg_bank = 0,			\
		.reg = ((r) - PINGROUP_REG_A)	\
	}

static const struct tegra_drive_pingroup_desc tegra_soc_drive_pingroups[TEGRA_MAX_DRIVE_PINGROUP] = {
	DRIVE_PINGROUP(AO1,		0x868),
	DRIVE_PINGROUP(AO2,		0x86c),
	DRIVE_PINGROUP(AT1,		0x870),
	DRIVE_PINGROUP(AT2,		0x874),
	DRIVE_PINGROUP(AT3,		0x878),
	DRIVE_PINGROUP(AT4,		0x87c),
	DRIVE_PINGROUP(AT5,		0x880),
	DRIVE_PINGROUP(CDEV1,		0x884),
	DRIVE_PINGROUP(CDEV2,		0x888),
	DRIVE_PINGROUP(CSUS,		0x88c),
	DRIVE_PINGROUP(DAP1,		0x890),
	DRIVE_PINGROUP(DAP2,		0x894),
	DRIVE_PINGROUP(DAP3,		0x898),
	DRIVE_PINGROUP(DAP4,		0x89c),
	DRIVE_PINGROUP(DBG,		0x8a0),
	DRIVE_PINGROUP(LCD1,		0x8a4),
	DRIVE_PINGROUP(LCD2,		0x8a8),
	DRIVE_PINGROUP(SDIO2,		0x8ac),
	DRIVE_PINGROUP(SDIO3,		0x8b0),
	DRIVE_PINGROUP(SPI,		0x8b4),
	DRIVE_PINGROUP(UAA,		0x8b8),
	DRIVE_PINGROUP(UAB,		0x8bc),
	DRIVE_PINGROUP(UART2,		0x8c0),
	DRIVE_PINGROUP(UART3,		0x8c4),
	DRIVE_PINGROUP(VI1,		0x8c8),
	DRIVE_PINGROUP(SDIO1,		0x8ec),
	DRIVE_PINGROUP(CRT,		0x8f8),
	DRIVE_PINGROUP(DDC,		0x8fc),
	DRIVE_PINGROUP(GMA,		0x900),
	DRIVE_PINGROUP(GMB,		0x904),
	DRIVE_PINGROUP(GMC,		0x908),
	DRIVE_PINGROUP(GMD,		0x90c),
	DRIVE_PINGROUP(GME,		0x910),
	DRIVE_PINGROUP(GMF,		0x914),
	DRIVE_PINGROUP(GMG,		0x918),
	DRIVE_PINGROUP(GMH,		0x91c),
	DRIVE_PINGROUP(OWR,		0x920),
	DRIVE_PINGROUP(UAD,		0x924),
	DRIVE_PINGROUP(GPV,		0x928),
	DRIVE_PINGROUP(DEV3,		0x92c),
	DRIVE_PINGROUP(CEC,		0x938),
};

#define PINGROUP(pg_name, vdd, f0, f1, f2, f3, fs, iod, reg)	\
	[TEGRA_PINGROUP_ ## pg_name] = {			\
		.name = #pg_name,				\
		.vddio = TEGRA_VDDIO_ ## vdd,			\
		.funcs = {					\
			TEGRA_MUX_ ## f0,			\
			TEGRA_MUX_ ## f1,			\
			TEGRA_MUX_ ## f2,			\
			TEGRA_MUX_ ## f3,			\
		},						\
		.func_safe = TEGRA_MUX_ ## fs,			\
		.tri_bank = 1,					\
		.tri_reg = ((reg) - MUXCTL_REG_A),		\
		.tri_bit = 4,					\
		.mux_bank = 1,					\
		.mux_reg = ((reg) - MUXCTL_REG_A),		\
		.mux_bit = 0,					\
		.pupd_bank = 1,					\
		.pupd_reg = ((reg) - MUXCTL_REG_A),		\
		.pupd_bit = 2,					\
		.io_default = TEGRA_PIN_ ## iod,		\
		.od_bit = 6,					\
		.lock_bit = 7,					\
		.ioreset_bit = 8,				\
	}

static const struct tegra_pingroup_desc tegra_soc_pingroups[TEGRA_MAX_PINGROUP] = {
	
	PINGROUP(ULPI_DATA0,	  BB,	    SPI3,	HSI,	    UARTA,	ULPI,	    RSVD,	INPUT,	0x3000),
	PINGROUP(ULPI_DATA1,	  BB,	    SPI3,	HSI,	    UARTA,	ULPI,	    RSVD,	INPUT,	0x3004),
	PINGROUP(ULPI_DATA2,	  BB,	    SPI3,	HSI,	    UARTA,	ULPI,	    RSVD,	INPUT,	0x3008),
	PINGROUP(ULPI_DATA3,	  BB,	    SPI3,	HSI,	    UARTA,	ULPI,	    RSVD,	INPUT,	0x300c),
	PINGROUP(ULPI_DATA4,	  BB,	    SPI2,	HSI,	    UARTA,	ULPI,	    RSVD,	INPUT,	0x3010),
	PINGROUP(ULPI_DATA5,	  BB,	    SPI2,	HSI,	    UARTA,	ULPI,	    RSVD,	INPUT,	0x3014),
	PINGROUP(ULPI_DATA6,	  BB,	    SPI2,	HSI,	    UARTA,	ULPI,	    RSVD,	INPUT,	0x3018),
	PINGROUP(ULPI_DATA7,	  BB,	    SPI2,	HSI,	    UARTA,	ULPI,	    RSVD,	INPUT,	0x301c),
	PINGROUP(ULPI_CLK,	  BB,	    SPI1,	RSVD,	    UARTD,	ULPI,	    RSVD,	INPUT,	0x3020),
	PINGROUP(ULPI_DIR,	  BB,	    SPI1,	RSVD,	    UARTD,	ULPI,	    RSVD,	INPUT,	0x3024),
	PINGROUP(ULPI_NXT,	  BB,	    SPI1,	RSVD,	    UARTD,	ULPI,	    RSVD,	INPUT,	0x3028),
	PINGROUP(ULPI_STP,	  BB,	    SPI1,	RSVD,	    UARTD,	ULPI,	    RSVD,	INPUT,	0x302c),
	PINGROUP(DAP3_FS,	  BB,	    I2S2,	RSVD1,	    DISPLAYA,	DISPLAYB,   RSVD,	INPUT,	0x3030),
	PINGROUP(DAP3_DIN,	  BB,	    I2S2,	RSVD1,	    DISPLAYA,	DISPLAYB,   RSVD,	INPUT,	0x3034),
	PINGROUP(DAP3_DOUT,	  BB,	    I2S2,	RSVD1,	    DISPLAYA,	DISPLAYB,   RSVD,	INPUT,	0x3038),
	PINGROUP(DAP3_SCLK,	  BB,	    I2S2,	RSVD1,	    DISPLAYA,	DISPLAYB,   RSVD,	INPUT,	0x303c),
	PINGROUP(GPIO_PV0,	  BB,	    RSVD,	RSVD,	    RSVD,	RSVD,	    RSVD,	INPUT,	0x3040),
	PINGROUP(GPIO_PV1,	  BB,	    RSVD,	RSVD,	    RSVD,	RSVD,	    RSVD,	INPUT,	0x3044),
	PINGROUP(SDMMC1_CLK,	  SDMMC1,   SDIO1,	RSVD1,	    RSVD2,	INVALID,    RSVD,	INPUT,	0x3048),
	PINGROUP(SDMMC1_CMD,	  SDMMC1,   SDIO1,	RSVD1,	    RSVD2,	INVALID,    RSVD,	INPUT,	0x304c),
	PINGROUP(SDMMC1_DAT3,	  SDMMC1,   SDIO1,	RSVD1,	    UARTE,	INVALID,    RSVD,	INPUT,	0x3050),
	PINGROUP(SDMMC1_DAT2,	  SDMMC1,   SDIO1,	RSVD1,	    UARTE,	INVALID,    RSVD,	INPUT,	0x3054),
	PINGROUP(SDMMC1_DAT1,	  SDMMC1,   SDIO1,	RSVD1,	    UARTE,	INVALID,    RSVD,	INPUT,	0x3058),
	PINGROUP(SDMMC1_DAT0,	  SDMMC1,   SDIO1,	RSVD1,	    UARTE,	INVALID,    RSVD,	INPUT,	0x305c),
	PINGROUP(GPIO_PV2,	  SDMMC1,   OWR,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x3060),
	PINGROUP(GPIO_PV3,	  SDMMC1,   INVALID,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x3064),
	PINGROUP(CLK2_OUT,	  SDMMC1,   EXTPERIPH2,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x3068),
	PINGROUP(CLK2_REQ,	  SDMMC1,   DAP,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x306c),
	PINGROUP(LCD_PWR1,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x3070),
	PINGROUP(LCD_PWR2,	  LCD,	    DISPLAYA,	DISPLAYB,   SPI5,	INVALID,    RSVD,	OUTPUT,	0x3074),
	PINGROUP(LCD_SDIN,	  LCD,	    DISPLAYA,	DISPLAYB,   SPI5,	RSVD,	    RSVD,	OUTPUT,	0x3078),
	PINGROUP(LCD_SDOUT,	  LCD,	    DISPLAYA,	DISPLAYB,   SPI5,	INVALID,    RSVD,	OUTPUT,	0x307c),
	PINGROUP(LCD_WR_N,	  LCD,	    DISPLAYA,	DISPLAYB,   SPI5,	INVALID,    RSVD,	OUTPUT,	0x3080),
	PINGROUP(LCD_CS0_N,	  LCD,	    DISPLAYA,	DISPLAYB,   SPI5,	RSVD,	    RSVD,	OUTPUT,	0x3084),
	PINGROUP(LCD_DC0,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x3088),
	PINGROUP(LCD_SCK,	  LCD,	    DISPLAYA,	DISPLAYB,   SPI5,	INVALID,    RSVD,	OUTPUT,	0x308c),
	PINGROUP(LCD_PWR0,	  LCD,	    DISPLAYA,	DISPLAYB,   SPI5,	INVALID,    RSVD,	OUTPUT,	0x3090),
	PINGROUP(LCD_PCLK,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x3094),
	PINGROUP(LCD_DE,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x3098),
	PINGROUP(LCD_HSYNC,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x309c),
	PINGROUP(LCD_VSYNC,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30a0),
	PINGROUP(LCD_D0,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30a4),
	PINGROUP(LCD_D1,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30a8),
	PINGROUP(LCD_D2,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30ac),
	PINGROUP(LCD_D3,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30b0),
	PINGROUP(LCD_D4,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30b4),
	PINGROUP(LCD_D5,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30b8),
	PINGROUP(LCD_D6,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30bc),
	PINGROUP(LCD_D7,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30c0),
	PINGROUP(LCD_D8,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30c4),
	PINGROUP(LCD_D9,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30c8),
	PINGROUP(LCD_D10,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30cc),
	PINGROUP(LCD_D11,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30d0),
	PINGROUP(LCD_D12,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30d4),
	PINGROUP(LCD_D13,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30d8),
	PINGROUP(LCD_D14,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30dc),
	PINGROUP(LCD_D15,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30e0),
	PINGROUP(LCD_D16,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30e4),
	PINGROUP(LCD_D17,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30e8),
	PINGROUP(LCD_D18,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30ec),
	PINGROUP(LCD_D19,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30f0),
	PINGROUP(LCD_D20,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30f4),
	PINGROUP(LCD_D21,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30f8),
	PINGROUP(LCD_D22,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x30fc),
	PINGROUP(LCD_D23,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x3100),
	PINGROUP(LCD_CS1_N,	  LCD,	    DISPLAYA,	DISPLAYB,   SPI5,	RSVD2,	    RSVD,	OUTPUT,	0x3104),
	PINGROUP(LCD_M1,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x3108),
	PINGROUP(LCD_DC1,	  LCD,	    DISPLAYA,	DISPLAYB,   RSVD1,	RSVD2,	    RSVD,	OUTPUT,	0x310c),
	PINGROUP(HDMI_INT,	  LCD,	    RSVD,	RSVD,	    RSVD,	RSVD,	    RSVD,	INPUT,	0x3110),
	PINGROUP(DDC_SCL,	  LCD,	    I2C4,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x3114),
	PINGROUP(DDC_SDA,	  LCD,	    I2C4,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x3118),
	PINGROUP(CRT_HSYNC,	  LCD,	    CRT,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x311c),
	PINGROUP(CRT_VSYNC,	  LCD,	    CRT,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x3120),
	PINGROUP(VI_D0,		  VI,	    INVALID,	RSVD1,	    VI,		RSVD2,	    RSVD,	INPUT,	0x3124),
	PINGROUP(VI_D1,		  VI,	    INVALID,	SDIO2,	    VI,		RSVD1,	    RSVD,	INPUT,	0x3128),
	PINGROUP(VI_D2,		  VI,	    INVALID,	SDIO2,	    VI,		RSVD1,	    RSVD,	INPUT,	0x312c),
	PINGROUP(VI_D3,		  VI,	    INVALID,	SDIO2,	    VI,		RSVD1,	    RSVD,	INPUT,	0x3130),
	PINGROUP(VI_D4,		  VI,	    INVALID,	SDIO2,	    VI,		RSVD1,	    RSVD,	INPUT,	0x3134),
	PINGROUP(VI_D5,		  VI,	    INVALID,	SDIO2,	    VI,		RSVD1,	    RSVD,	INPUT,	0x3138),
	PINGROUP(VI_D6,		  VI,	    INVALID,	SDIO2,	    VI,		RSVD1,	    RSVD,	INPUT,	0x313c),
	PINGROUP(VI_D7,		  VI,	    INVALID,	SDIO2,	    VI,		RSVD1,	    RSVD,	INPUT,	0x3140),
	PINGROUP(VI_D8,		  VI,	    INVALID,	SDIO2,	    VI,		RSVD1,	    RSVD,	INPUT,	0x3144),
	PINGROUP(VI_D9,		  VI,	    INVALID,	SDIO2,	    VI,		RSVD1,	    RSVD,	INPUT,	0x3148),
	PINGROUP(VI_D10,	  VI,	    INVALID,	RSVD1,	    VI,		RSVD2,	    RSVD,	INPUT,	0x314c),
	PINGROUP(VI_D11,	  VI,	    INVALID,	RSVD1,	    VI,		RSVD2,	    RSVD,	INPUT,	0x3150),
	PINGROUP(VI_PCLK,	  VI,	    RSVD1,	SDIO2,	    VI,		RSVD2,	    RSVD,	INPUT,	0x3154),
	PINGROUP(VI_MCLK,	  VI,	    VI,		INVALID,    INVALID,	INVALID,    RSVD,	INPUT,	0x3158),
	PINGROUP(VI_VSYNC,	  VI,	    INVALID,	RSVD1,	    VI,		RSVD2,	    RSVD,	INPUT,	0x315c),
	PINGROUP(VI_HSYNC,	  VI,	    INVALID,	RSVD1,	    VI,		RSVD2,	    RSVD,	INPUT,	0x3160),
	PINGROUP(UART2_RXD,	  UART,	    IRDA,	SPDIF,	    UARTA,	SPI4,	    RSVD,	INPUT,	0x3164),
	PINGROUP(UART2_TXD,	  UART,	    IRDA,	SPDIF,	    UARTA,	SPI4,	    RSVD,	INPUT,	0x3168),
	PINGROUP(UART2_RTS_N,	  UART,	    UARTA,	UARTB,	    GMI,	SPI4,	    RSVD,	INPUT,	0x316c),
	PINGROUP(UART2_CTS_N,	  UART,	    UARTA,	UARTB,	    GMI,	SPI4,	    RSVD,	INPUT,	0x3170),
	PINGROUP(UART3_TXD,	  UART,	    UARTC,	RSVD1,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3174),
	PINGROUP(UART3_RXD,	  UART,	    UARTC,	RSVD1,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3178),
	PINGROUP(UART3_CTS_N,	  UART,	    UARTC,	RSVD1,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x317c),
	PINGROUP(UART3_RTS_N,	  UART,	    UARTC,	PWM0,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3180),
	PINGROUP(GPIO_PU0,	  UART,	    OWR,	UARTA,	    GMI,	RSVD1,	    RSVD,	INPUT,	0x3184),
	PINGROUP(GPIO_PU1,	  UART,	    RSVD1,	UARTA,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3188),
	PINGROUP(GPIO_PU2,	  UART,	    RSVD1,	UARTA,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x318c),
	PINGROUP(GPIO_PU3,	  UART,	    PWM0,	UARTA,	    GMI,	RSVD1,	    RSVD,	INPUT,	0x3190),
	PINGROUP(GPIO_PU4,	  UART,	    PWM1,	UARTA,	    GMI,	RSVD1,	    RSVD,	INPUT,	0x3194),
	PINGROUP(GPIO_PU5,	  UART,	    PWM2,	UARTA,	    GMI,	RSVD1,	    RSVD,	INPUT,	0x3198),
	PINGROUP(GPIO_PU6,	  UART,	    PWM3,	UARTA,	    GMI,	RSVD1,	    RSVD,	INPUT,	0x319c),
	PINGROUP(GEN1_I2C_SDA,	  UART,	    I2C,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x31a0),
	PINGROUP(GEN1_I2C_SCL,	  UART,	    I2C,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x31a4),
	PINGROUP(DAP4_FS,	  UART,	    I2S3,	RSVD1,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31a8),
	PINGROUP(DAP4_DIN,	  UART,	    I2S3,	RSVD1,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31ac),
	PINGROUP(DAP4_DOUT,	  UART,	    I2S3,	RSVD1,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31b0),
	PINGROUP(DAP4_SCLK,	  UART,	    I2S3,	RSVD1,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31b4),
	PINGROUP(CLK3_OUT,	  UART,	    EXTPERIPH3,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x31b8),
	PINGROUP(CLK3_REQ,	  UART,	    DEV3,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x31bc),
	PINGROUP(GMI_WP_N,	  GMI,	    RSVD1,	NAND,	    GMI,	GMI_ALT,    RSVD,	INPUT,	0x31c0),
	PINGROUP(GMI_IORDY,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31c4),
	PINGROUP(GMI_WAIT,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31c8),
	PINGROUP(GMI_ADV_N,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31cc),
	PINGROUP(GMI_CLK,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31d0),
	PINGROUP(GMI_CS0_N,	  GMI,	    RSVD1,	NAND,	    GMI,	INVALID,    RSVD,	INPUT,	0x31d4),
	PINGROUP(GMI_CS1_N,	  GMI,	    RSVD1,	NAND,	    GMI,	DTV,	    RSVD,	INPUT,	0x31d8),
	PINGROUP(GMI_CS2_N,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31dc),
	PINGROUP(GMI_CS3_N,	  GMI,	    RSVD1,	NAND,	    GMI,	GMI_ALT,    RSVD,	INPUT,	0x31e0),
	PINGROUP(GMI_CS4_N,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31e4),
	PINGROUP(GMI_CS6_N,	  GMI,	    NAND,	NAND_ALT,   GMI,	SATA,	    RSVD,	INPUT,	0x31e8),
	PINGROUP(GMI_CS7_N,	  GMI,	    NAND,	NAND_ALT,   GMI,	GMI_ALT,    RSVD,	INPUT,	0x31ec),
	PINGROUP(GMI_AD0,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31f0),
	PINGROUP(GMI_AD1,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31f4),
	PINGROUP(GMI_AD2,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31f8),
	PINGROUP(GMI_AD3,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x31fc),
	PINGROUP(GMI_AD4,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3200),
	PINGROUP(GMI_AD5,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3204),
	PINGROUP(GMI_AD6,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3208),
	PINGROUP(GMI_AD7,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x320c),
	PINGROUP(GMI_AD8,	  GMI,	    PWM0,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3210),
	PINGROUP(GMI_AD9,	  GMI,	    PWM1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3214),
	PINGROUP(GMI_AD10,	  GMI,	    PWM2,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3218),
	PINGROUP(GMI_AD11,	  GMI,	    PWM3,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x321c),
	PINGROUP(GMI_AD12,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3220),
	PINGROUP(GMI_AD13,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3224),
	PINGROUP(GMI_AD14,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x3228),
	PINGROUP(GMI_AD15,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD2,	    RSVD,	INPUT,	0x322c),
	PINGROUP(GMI_A16,	  GMI,	    UARTD,	SPI4,	    GMI,	GMI_ALT,    RSVD,	INPUT,	0x3230),
	PINGROUP(GMI_A17,	  GMI,	    UARTD,	SPI4,	    GMI,	INVALID,    RSVD,	INPUT,	0x3234),
	PINGROUP(GMI_A18,	  GMI,	    UARTD,	SPI4,	    GMI,	INVALID,    RSVD,	INPUT,	0x3238),
	PINGROUP(GMI_A19,	  GMI,	    UARTD,	SPI4,	    GMI,	RSVD3,	    RSVD,	INPUT,	0x323c),
	PINGROUP(GMI_WR_N,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD3,	    RSVD,	INPUT,	0x3240),
	PINGROUP(GMI_OE_N,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD3,	    RSVD,	INPUT,	0x3244),
	PINGROUP(GMI_DQS,	  GMI,	    RSVD1,	NAND,	    GMI,	RSVD3,	    RSVD,	INPUT,	0x3248),
	PINGROUP(GMI_RST_N,	  GMI,	    NAND,	NAND_ALT,   GMI,	RSVD3,	    RSVD,	INPUT,	0x324c),
	PINGROUP(GEN2_I2C_SCL,	  GMI,	    I2C2,	INVALID,    GMI,	RSVD3,	    RSVD,	INPUT,	0x3250),
	PINGROUP(GEN2_I2C_SDA,	  GMI,	    I2C2,	INVALID,    GMI,	RSVD3,	    RSVD,	INPUT,	0x3254),
	PINGROUP(SDMMC4_CLK,	  SDMMC4,   INVALID,	NAND,	    GMI,	SDIO4,	    RSVD,	INPUT,	0x3258),
	PINGROUP(SDMMC4_CMD,	  SDMMC4,   I2C3,	NAND,	    GMI,	SDIO4,	    RSVD,	INPUT,	0x325c),
	PINGROUP(SDMMC4_DAT0,	  SDMMC4,   UARTE,	SPI3,	    GMI,	SDIO4,	    RSVD,	INPUT,	0x3260),
	PINGROUP(SDMMC4_DAT1,	  SDMMC4,   UARTE,	SPI3,	    GMI,	SDIO4,	    RSVD,	INPUT,	0x3264),
	PINGROUP(SDMMC4_DAT2,	  SDMMC4,   UARTE,	SPI3,	    GMI,	SDIO4,	    RSVD,	INPUT,	0x3268),
	PINGROUP(SDMMC4_DAT3,	  SDMMC4,   UARTE,	SPI3,	    GMI,	SDIO4,	    RSVD,	INPUT,	0x326c),
	PINGROUP(SDMMC4_DAT4,	  SDMMC4,   I2C3,	I2S4,	    GMI,	SDIO4,	    RSVD,	INPUT,	0x3270),
	PINGROUP(SDMMC4_DAT5,	  SDMMC4,   VGP3,	I2S4,	    GMI,	SDIO4,	    RSVD,	INPUT,	0x3274),
	PINGROUP(SDMMC4_DAT6,	  SDMMC4,   VGP4,	I2S4,	    GMI,	SDIO4,	    RSVD,	INPUT,	0x3278),
	PINGROUP(SDMMC4_DAT7,	  SDMMC4,   VGP5,	I2S4,	    GMI,	SDIO4,	    RSVD,	INPUT,	0x327c),
	PINGROUP(SDMMC4_RST_N,	  SDMMC4,   VGP6,	RSVD1,	    RSVD2,	POPSDMMC4,  RSVD,	INPUT,	0x3280),
	PINGROUP(CAM_MCLK,	  CAM,	    VI,		INVALID,    VI_ALT2,	POPSDMMC4,  RSVD,	INPUT,	0x3284),
	PINGROUP(GPIO_PCC1,	  CAM,	    I2S4,	RSVD1,	    RSVD2,	POPSDMMC4,  RSVD,	INPUT,	0x3288),
	PINGROUP(GPIO_PBB0,	  CAM,	    I2S4,	RSVD1,	    RSVD2,	POPSDMMC4,  RSVD,	INPUT,	0x328c),
	PINGROUP(CAM_I2C_SCL,	  CAM,	    INVALID,	I2C3,	    RSVD2,	POPSDMMC4,  RSVD,	INPUT,	0x3290),
	PINGROUP(CAM_I2C_SDA,	  CAM,	    INVALID,	I2C3,	    RSVD2,	POPSDMMC4,  RSVD,	INPUT,	0x3294),
	PINGROUP(GPIO_PBB3,	  CAM,	    VGP3,	DISPLAYA,   DISPLAYB,	POPSDMMC4,  RSVD,	INPUT,	0x3298),
	PINGROUP(GPIO_PBB4,	  CAM,	    VGP4,	DISPLAYA,   DISPLAYB,	POPSDMMC4,  RSVD,	INPUT,	0x329c),
	PINGROUP(GPIO_PBB5,	  CAM,	    VGP5,	DISPLAYA,   DISPLAYB,	POPSDMMC4,  RSVD,	INPUT,	0x32a0),
	PINGROUP(GPIO_PBB6,	  CAM,	    VGP6,	DISPLAYA,   DISPLAYB,	POPSDMMC4,  RSVD,	INPUT,	0x32a4),
	PINGROUP(GPIO_PBB7,	  CAM,	    I2S4,	RSVD1,	    RSVD2,	POPSDMMC4,  RSVD,	INPUT,	0x32a8),
	PINGROUP(GPIO_PCC2,	  CAM,	    I2S4,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x32ac),
	PINGROUP(JTAG_RTCK,	  SYS,	    RTCK,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x32b0),
	PINGROUP(PWR_I2C_SCL,	  SYS,	    I2CPWR,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x32b4),
	PINGROUP(PWR_I2C_SDA,	  SYS,	    I2CPWR,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x32b8),
	PINGROUP(KB_ROW0,	  SYS,	    KBC,	INVALID,    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x32bc),
	PINGROUP(KB_ROW1,	  SYS,	    KBC,	INVALID,    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x32c0),
	PINGROUP(KB_ROW2,	  SYS,	    KBC,	INVALID,    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x32c4),
	PINGROUP(KB_ROW3,	  SYS,	    KBC,	INVALID,    RSVD2,	INVALID,    RSVD,	INPUT,	0x32c8),
	PINGROUP(KB_ROW4,	  SYS,	    KBC,	INVALID,    TRACE,	RSVD3,	    RSVD,	INPUT,	0x32cc),
	PINGROUP(KB_ROW5,	  SYS,	    KBC,	INVALID,    TRACE,	OWR,	    RSVD,	INPUT,	0x32d0),
	PINGROUP(KB_ROW6,	  SYS,	    KBC,	INVALID,    SDIO2,	INVALID,    RSVD,	INPUT,	0x32d4),
	PINGROUP(KB_ROW7,	  SYS,	    KBC,	INVALID,    SDIO2,	INVALID,    RSVD,	INPUT,	0x32d8),
	PINGROUP(KB_ROW8,	  SYS,	    KBC,	INVALID,    SDIO2,	INVALID,    RSVD,	INPUT,	0x32dc),
	PINGROUP(KB_ROW9,	  SYS,	    KBC,	INVALID,    SDIO2,	INVALID,    RSVD,	INPUT,	0x32e0),
	PINGROUP(KB_ROW10,	  SYS,	    KBC,	INVALID,    SDIO2,	INVALID,    RSVD,	INPUT,	0x32e4),
	PINGROUP(KB_ROW11,	  SYS,	    KBC,	INVALID,    SDIO2,	INVALID,    RSVD,	INPUT,	0x32e8),
	PINGROUP(KB_ROW12,	  SYS,	    KBC,	INVALID,    SDIO2,	INVALID,    RSVD,	INPUT,	0x32ec),
	PINGROUP(KB_ROW13,	  SYS,	    KBC,	INVALID,    SDIO2,	INVALID,    RSVD,	INPUT,	0x32f0),
	PINGROUP(KB_ROW14,	  SYS,	    KBC,	INVALID,    SDIO2,	INVALID,    RSVD,	INPUT,	0x32f4),
	PINGROUP(KB_ROW15,	  SYS,	    KBC,	INVALID,    SDIO2,	INVALID,    RSVD,	INPUT,	0x32f8),
	PINGROUP(KB_COL0,	  SYS,	    KBC,	INVALID,    TRACE,	INVALID,    RSVD,	INPUT,	0x32fc),
	PINGROUP(KB_COL1,	  SYS,	    KBC,	INVALID,    TRACE,	INVALID,    RSVD,	INPUT,	0x3300),
	PINGROUP(KB_COL2,	  SYS,	    KBC,	INVALID,    TRACE,	RSVD,	    RSVD,	INPUT,	0x3304),
	PINGROUP(KB_COL3,	  SYS,	    KBC,	INVALID,    TRACE,	RSVD,	    RSVD,	INPUT,	0x3308),
	PINGROUP(KB_COL4,	  SYS,	    KBC,	INVALID,    TRACE,	RSVD,	    RSVD,	INPUT,	0x330c),
	PINGROUP(KB_COL5,	  SYS,	    KBC,	INVALID,    TRACE,	RSVD,	    RSVD,	INPUT,	0x3310),
	PINGROUP(KB_COL6,	  SYS,	    KBC,	INVALID,    TRACE,	INVALID,    RSVD,	INPUT,	0x3314),
	PINGROUP(KB_COL7,	  SYS,	    KBC,	INVALID,    TRACE,	INVALID,    RSVD,	INPUT,	0x3318),
	PINGROUP(CLK_32K_OUT,	  SYS,	    BLINK,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x331c),
	PINGROUP(SYS_CLK_REQ,	  SYS,	    SYSCLK,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x3320),
	PINGROUP(CORE_PWR_REQ,	  SYS,	    RSVD,	RSVD,	    RSVD,	RSVD,	    RSVD,	INPUT,	0x3324),
	PINGROUP(CPU_PWR_REQ,	  SYS,	    RSVD,	RSVD,	    RSVD,	RSVD,	    RSVD,	INPUT,	0x3328),
	PINGROUP(PWR_INT_N,	  SYS,	    RSVD,	RSVD,	    RSVD,	RSVD,	    RSVD,	INPUT,	0x332c),
	PINGROUP(CLK_32K_IN,	  SYS,	    RSVD,	RSVD,	    RSVD,	RSVD,	    RSVD,	INPUT,	0x3330),
	PINGROUP(OWR,		  SYS,	    OWR,	RSVD,	    RSVD,	RSVD,	    RSVD,	INPUT,	0x3334),
	PINGROUP(DAP1_FS,	  AUDIO,    I2S0,	HDA,	    GMI,	SDIO2,	    RSVD,	INPUT,	0x3338),
	PINGROUP(DAP1_DIN,	  AUDIO,    I2S0,	HDA,	    GMI,	SDIO2,	    RSVD,	INPUT,	0x333c),
	PINGROUP(DAP1_DOUT,	  AUDIO,    I2S0,	HDA,	    GMI,	SDIO2,	    RSVD,	INPUT,	0x3340),
	PINGROUP(DAP1_SCLK,	  AUDIO,    I2S0,	HDA,	    GMI,	SDIO2,	    RSVD,	INPUT,	0x3344),
	PINGROUP(CLK1_REQ,	  AUDIO,    DAP,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x3348),
	PINGROUP(CLK1_OUT,	  AUDIO,    EXTPERIPH1,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x334c),
	PINGROUP(SPDIF_IN,	  AUDIO,    SPDIF,	HDA,	    INVALID,	DAPSDMMC2,  RSVD,	INPUT,	0x3350),
	PINGROUP(SPDIF_OUT,	  AUDIO,    SPDIF,	RSVD1,	    INVALID,	DAPSDMMC2,  RSVD,	INPUT,	0x3354),
	PINGROUP(DAP2_FS,	  AUDIO,    I2S1,	HDA,	    RSVD2,	GMI,	    RSVD,	INPUT,	0x3358),
	PINGROUP(DAP2_DIN,	  AUDIO,    I2S1,	HDA,	    RSVD2,	GMI,	    RSVD,	INPUT,	0x335c),
	PINGROUP(DAP2_DOUT,	  AUDIO,    I2S1,	HDA,	    RSVD2,	GMI,	    RSVD,	INPUT,	0x3360),
	PINGROUP(DAP2_SCLK,	  AUDIO,    I2S1,	HDA,	    RSVD2,	GMI,	    RSVD,	INPUT,	0x3364),
	PINGROUP(SPI2_MOSI,	  AUDIO,    SPI6,	SPI2,	    INVALID,	GMI,	    RSVD,	INPUT,	0x3368),
	PINGROUP(SPI2_MISO,	  AUDIO,    SPI6,	SPI2,	    INVALID,	GMI,	    RSVD,	INPUT,	0x336c),
	PINGROUP(SPI2_CS0_N,	  AUDIO,    SPI6,	SPI2,	    INVALID,	GMI,	    RSVD,	INPUT,	0x3370),
	PINGROUP(SPI2_SCK,	  AUDIO,    SPI6,	SPI2,	    INVALID,	GMI,	    RSVD,	INPUT,	0x3374),
	PINGROUP(SPI1_MOSI,	  AUDIO,    SPI2,	SPI1,	    INVALID,	GMI,	    RSVD,	INPUT,	0x3378),
	PINGROUP(SPI1_SCK,	  AUDIO,    SPI2,	SPI1,	    INVALID,	GMI,	    RSVD,	INPUT,	0x337c),
	PINGROUP(SPI1_CS0_N,	  AUDIO,    SPI2,	SPI1,	    INVALID,	GMI,	    RSVD,	INPUT,	0x3380),
	PINGROUP(SPI1_MISO,	  AUDIO,    INVALID,	SPI1,	    INVALID,	RSVD3,	    RSVD,	INPUT,	0x3384),
	PINGROUP(SPI2_CS1_N,	  AUDIO,    INVALID,	SPI2,	    INVALID,	INVALID,    RSVD,	INPUT,	0x3388),
	PINGROUP(SPI2_CS2_N,	  AUDIO,    INVALID,	SPI2,	    INVALID,	INVALID,    RSVD,	INPUT,	0x338c),
	PINGROUP(SDMMC3_CLK,	  SDMMC3,   UARTA,	PWM2,	    SDIO3,	INVALID,    RSVD,	INPUT,	0x3390),
	PINGROUP(SDMMC3_CMD,	  SDMMC3,   UARTA,	PWM3,	    SDIO3,	INVALID,    RSVD,	INPUT,	0x3394),
	PINGROUP(SDMMC3_DAT0,	  SDMMC3,   RSVD,	RSVD1,	    SDIO3,	INVALID,    RSVD,	INPUT,	0x3398),
	PINGROUP(SDMMC3_DAT1,	  SDMMC3,   RSVD,	RSVD1,	    SDIO3,	INVALID,    RSVD,	INPUT,	0x339c),
	PINGROUP(SDMMC3_DAT2,	  SDMMC3,   RSVD,	PWM1,	    SDIO3,	INVALID,    RSVD,	INPUT,	0x33a0),
	PINGROUP(SDMMC3_DAT3,	  SDMMC3,   RSVD,	PWM0,	    SDIO3,	INVALID,    RSVD,	INPUT,	0x33a4),
	PINGROUP(SDMMC3_DAT4,	  SDMMC3,   PWM1,	INVALID,    SDIO3,	INVALID,    RSVD,	INPUT,	0x33a8),
	PINGROUP(SDMMC3_DAT5,	  SDMMC3,   PWM0,	INVALID,    SDIO3,	INVALID,    RSVD,	INPUT,	0x33ac),
	PINGROUP(SDMMC3_DAT6,	  SDMMC3,   SPDIF,	INVALID,    SDIO3,	INVALID,    RSVD,	INPUT,	0x33b0),
	PINGROUP(SDMMC3_DAT7,	  SDMMC3,   SPDIF,	INVALID,    SDIO3,	INVALID,    RSVD,	INPUT,	0x33b4),
	PINGROUP(PEX_L0_PRSNT_N,  PEXCTL,   PCIE,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33b8),
	PINGROUP(PEX_L0_RST_N,	  PEXCTL,   PCIE,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33bc),
	PINGROUP(PEX_L0_CLKREQ_N, PEXCTL,   PCIE,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33c0),
	PINGROUP(PEX_WAKE_N,	  PEXCTL,   PCIE,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33c4),
	PINGROUP(PEX_L1_PRSNT_N,  PEXCTL,   PCIE,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33c8),
	PINGROUP(PEX_L1_RST_N,	  PEXCTL,   PCIE,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33cc),
	PINGROUP(PEX_L1_CLKREQ_N, PEXCTL,   PCIE,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33d0),
	PINGROUP(PEX_L2_PRSNT_N,  PEXCTL,   PCIE,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33d4),
	PINGROUP(PEX_L2_RST_N,	  PEXCTL,   PCIE,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33d8),
	PINGROUP(PEX_L2_CLKREQ_N, PEXCTL,   PCIE,	HDA,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33dc),
	PINGROUP(HDMI_CEC,	  SYS,      CEC,	RSVD1,	    RSVD2,	RSVD3,	    RSVD,	INPUT,	0x33e0),
};

void __devinit tegra30_pinmux_init(const struct tegra_pingroup_desc **pg,
		int *pg_max, const struct tegra_drive_pingroup_desc **pgdrive,
		int *pgdrive_max)
{
	*pg = tegra_soc_pingroups;
	*pg_max = TEGRA_MAX_PINGROUP;
	*pgdrive = tegra_soc_drive_pingroups;
	*pgdrive_max = TEGRA_MAX_DRIVE_PINGROUP;
}

