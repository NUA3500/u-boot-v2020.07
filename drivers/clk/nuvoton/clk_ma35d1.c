// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 Nuvoton technology corporation
 * All rights reserved.
 */

#include <common.h>
#include <clk.h>
#include <clk-uclass.h>
#include <dm.h>
#include <log.h>
#include <linux/bitops.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <dt-bindings/clock/ma35d1-clk.h>
#include <misc.h>

#include "regs-ma35d1-clock.h"
#include "clk-ma35d1.h"

DECLARE_GLOBAL_DATA_PTR;

struct __ma35d1_clk_priv {
	void *base;
	const struct __ma35d1_ccf_ops *ops;
	struct __ma35d1_clk_gate_data *gate_data;
	struct __ma35d1_wrpll_data *pwd;
	struct __ma35d1_clk_data *data;
};

/* struct __ma35d1_ccf_ops - clock operations */
struct __ma35d1_ccf_ops {
	int (*set_rate)(struct __ma35d1_clk_priv *pc,unsigned long rate,
	                unsigned long parent_rate);
	unsigned long (*round_rate)(struct __ma35d1_clk_priv *pc,unsigned long rate,
	                            unsigned long *parent_rate);
	unsigned long (*recalc_rate)(struct __ma35d1_clk_priv *pc,
	                             unsigned long parent_rate);
	int (*enable_clk)(struct __ma35d1_clk_priv *pc, bool enable);
};

static const char * const clk_names[clk_max] = {
/*0 */ "capll", "syspll", "ddrpll", "apll","epll", "vpll",
/*6 */ "hxt_gate", "lxt_gate", "hirc_gate", "lirc_gate", "ddr0_gate", "ddr6_gate", "sdh0_gate", "sdh1_gate",
/*14 */ "nand_gate", "usbh_gate", "husbh0_gate", "husbh1_gate", "dcu_gate", "emac0_gate", "emac1_gate", "rtc_gate",
/*22 */ "ddr_gate", "i2c0_gate", "i2c1_gate", "i2c2_gate", "i2c3_gate", "i2c4_gate", "i2c5_gate", "qspi0_gate",
/*30 */ "qspi1_gate", "spi0_gate", "spi1_gate", "spi2_gate", "spi3_gate", "gpa_gate", "gpb_gate", "gpc_gate",
/*38 */ "gpd_gate", "gpe_gate", "gpf_gate", "gpg_gate", "gph_gate", "gpi_gate", "gpj_gate", "gpk_gate",
/*46 */ "gpl_gate", "gpm_gate", "gpn_gate", "ca35clk_mux", "sysclk0_mux", "sysclk1_mux", "sdh0_mux", "sdh1_mux",
/*54 */ "dcu_mux", "dcup_mux", "spi0_mux", "spi1_mux", "spi2_mux", "spi3_mux", "qspi0_mux", "qspi1_mux",
/*62 */ "dcup_div", "emac0_div", "emac1_div", "aclk0_div", "wdt1_gate"
};

ulong hxt_hz = 0;
ulong lxt_hz = 0;
ulong hirc_hz = 0;
ulong lirc_hz = 0;
ulong usbphy0_hz = 0, usbphy1_hz = 0;
ulong pllfreq[6] = {0};
u32 pllmode[6] = {0};
ulong caclk = 0;
ulong sysclk[2] = {0};

unsigned long CLK_CalPLLFreq_Mode0(unsigned long PllSrcClk,
                                   unsigned long u64PllFreq, u32 *u32Reg)
{
	u32 u32M, u32N, u32P;
	u32 u32Tmp, u32Min, u32MinN, u32MinM, u32MinP;

	unsigned long u64basFreq, u64PllClk;
	unsigned long u64Con1, u64Con2, u64Con3;

	/* Find best solution */
	u32Min = (u32) - 1;
	u32MinM = 0UL;
	u32MinN = 0UL;
	u32MinP = 0UL;
	u64basFreq = u64PllFreq;
	if((u64PllFreq <= 2400000000) && (u64PllFreq >= 85700000)) {
		for(u32M = 1UL; u32M < 64UL; u32M++) {
			u64Con1 = PllSrcClk/u32M;
			if(!((u64Con1 <= 40000000) && (u64Con1 >= 1000000)))  continue;

			for(u32N = 16UL; u32N < 2048UL; u32N++) {
				u64Con2 = u64Con1 * u32N;
				if(!((u64Con2 <= 2400000000) && (u64Con2 >= 600000000)))  continue;

				for(u32P = 1UL; u32P < 8UL; u32P++) {
					/* Break when get good results */
					if (u32Min == 0UL) {
						break;
					}

					u64Con3 = u64Con2 / u32P;
					if(!((u64Con3 <= 2400000000) && (u64Con3 >= 85700000)))	 continue;

					u32Tmp = (u64Con3 > u64PllFreq) ? u64Con3 - u64PllFreq : u64PllFreq - u64Con3;
					if(u32Tmp < u32Min) {
						u32Min = u32Tmp;
						u32MinM = u32M;
						u32MinN = u32N;
						u32MinP = u32P;

					} else {
						if (u64Con3 < u64PllFreq)  break;
					}
				}
			}
		}

		/* Enable and apply new PLL setting. */
		u32Reg[0] =	 (u32MinM << 12) | (u32MinN);
		u32Reg[1] =	 (u32MinP << 4);

		/* Actual PLL output clock frequency */
		u64PllClk = (PllSrcClk * u32MinN) / (u32MinP * (u32MinM));
	} else {
		u32Reg[0] = 0x30FA;
		u32Reg[1] = 0x20;

		/* Actual PLL output clock frequency */
		u64PllClk = 1000000000;
	}

	return u64PllClk;
}

unsigned long CLK_CalPLLFreq_Mode1(unsigned long PllSrcClk,
                                   unsigned long u64PllFreq, u32 *u32Reg)
{
	unsigned long u64X, u64N, u64M, u64P, u64tmp, u64tmpP, u64tmpM;
	unsigned long u64PllClk, u64FCLKO;
	u32 u32FRAC, i;

	// check condition 1
	if ((PllSrcClk > 200000000) || (PllSrcClk < 1000000)) {
		// Fref is incorrect, return fail
		return 0;
	}

	// check condition 4
	if(u64PllFreq < 85700000) {
		// Adjust u64FCLKO
		u64FCLKO = 0;

		for(i = 2; i < 100; i++) {
			u64tmp = (i * u64PllFreq);
			if (u64tmp > 85700000) {
				u64FCLKO = u64tmp;
				break;
			}
		}

		if (u64FCLKO == 0) return 0;
	} else if(u64PllFreq >= 2400000000) {
		u32Reg[0] = 0x30FA;
		u32Reg[1] = (0x2 << 4);
		u64PllClk = 1000000000;
		return u64PllClk;
	} else
		u64FCLKO = u64PllFreq;

	// Find P
	u64P = 0;
	for(i = 1; i < 8; i++) {
		u64tmpP =  i * u64FCLKO;
		// it should be condition 3
		if((u64tmpP <= 2400000000) && (u64tmpP >= 600000000)) {
			u64P = i;
			break;
		}
	}

	// No reasonable P is found, return fail.
	if (u64P == 0)	return 0;

	// Find M
	u64M = 0; // Initialize it, and use it to judge reasonable M is found or not
	for(i = 1; i < 64; i++) {
		u64tmpM = PllSrcClk / i;
		if((u64tmpM <= 40000000) && (u64tmpM >= 10000000)) { // condition 2
			u64M = i;
			break;
		}
	}

	if (u64M == 0) { // No reasonable M is found
		return 0;
	}

	u64tmp = (u64FCLKO * u64P * u64M * 1000)/ PllSrcClk;
	u64N = u64tmp / 1000;
	u64X = u64tmp % 1000;
	u32FRAC = ((u64X << 24) + 500) / 1000;

	u32Reg[0] = (u64M << 12) | (u64N);
	u32Reg[1] = (u64P << 4) | (u32FRAC << 8);

	/* Actual PLL output clock frequency */
	u64PllClk = (PllSrcClk * u64tmp) / u64P / u64M / 1000;

	return u64PllClk;
}

unsigned long CLK_CalPLLFreq_Mode2(unsigned long PllSrcClk,
                                   unsigned long u64PllFreq, u32 u32SR, u32 u32Fmod, u32 *u32Reg)
{

	unsigned long u64PllClk;

#if 0 // Spread Specrum mode PLL calculating

	unsigned long u64X, u64N, u64M, u64P, u64tmp, u64tmpP, u64tmpM;
	unsigned long u64PllClk, u64FCLKO;
	u32 u32FRAC, i;

	// check condition 1
	if ((PllSrcClk > 200000000) || (PllSrcClk < 1000000)) {
		// Fref is incorrect, return fail case
		return 0;
	}

	// check condition 4
	if(u64PllFreq < 85700000) {
		u64FCLKO = 0;
		for(i = 2; i < 8; i++) {
			u64tmp = (i * u64PllFreq);
			if (u64tmp > 85700000) {
				u64FCLKO = u64tmp;
			}
		}

		if (u64FCLKO == 0) return 0;
	} else if(u64PllFreq >= 2400000000) {
		u32Reg[0] = 0x30FA;
		u32Reg[1] = (0x2 << 4);
		u64PllClk = 1000000000;
		return u64PllClk;
	} else
		u64FCLKO = u64PllFreq;

	// Find P
	u64P = 0;
	for(i = 1; i < 8; i++) {
		u64tmpP =  i * u64FCLKO;
		if((u64tmpP <= 2400000000) && (u64tmpP >= 600000000)) {
			u64P = i;
			break;
		}
	}

	// No reasonable P is found, return fail.
	if (u64P == 0)	return 0;

	// Find M
	u64M = 0; // Initialize it, and use it to judge reasonable M is found or not
	for(i = 1; i < 64; i++) {
		u64tmpM = PllSrcClk / i;
		if((u64tmpM <= 40000000) && (u64tmpM >= 10000000)) { // condition 2
			u64M = i;
			break;
		}
	}

	if (u64M == 0) { // No reasonable M is found
		return 0;
	}

	u64tmp = (u64FCLKO * u64P * u64M * 1000)/ PllSrcClk;
	u64N = u64tmp / 1000;
	u64X = u64tmp % 1000;
	u32FRAC = ((u64X << 24) + 500) / 1000;

	u64SSRATE = ((PllSrcClk >> 1) / (u32Fmod*2)) -1;
	u64SLOPE = ((u64tmp * u32SR / u64SSRATE) << 24) / 100 / 1000;

	u32Reg[0] = (u64SSRATE << 20) | (u64M << 12) | (u64N);
	u32Reg[1] = (u64P << 4) | (u32FRAC << 8);
	u32Reg[2] = u64SLOPE;

	/* Actual PLL output clock frequency */
	u64PllClk = (PllSrcClk * u64tmp) / u64P / u64M / 1000;

#else

	// Workround :Slope bug(only 16bit)
	if(u64PllFreq <= 266000000) {
		u32Reg[0] = 0x07782085;
		u32Reg[1] = 0x60;
		u32Reg[2] = 0x58CF9;
		u64PllClk = 266000000;
	} else {
		u32Reg[0] = 0x12b81016;
		u32Reg[1] = 0x35532610;
		u32Reg[2] = 0x9208;
		u64PllClk = 533000000;
	}
#endif

	return u64PllClk;
}

unsigned long CLK_SetPLLFreq(struct __ma35d1_clk_priv *pc,
                             unsigned long PllSrcClk, unsigned long u64PllFreq)
{
	struct __ma35d1_clk_data *data = pc->data;
	struct __ma35d1_wrpll_data *pwd = pc->pwd;
	u32 u32Reg[3] = {0}, val_ctl0, val_ctl1, val_ctl2;
	unsigned long u64PllClk;

	pr_debug("%s\n", __func__);
	val_ctl0 = readl(data->va + pwd->ctl0_offs);
	val_ctl1 = readl(data->va + pwd->ctl1_offs);
	val_ctl2 = readl(data->va + pwd->ctl2_offs);

	// PLL Operation mode setting
	val_ctl0 = (val_ctl0 & ~(0xc0000)) | (pwd->pllmode << 18);

	if(pwd->pllmode == 0) {
		u64PllClk = CLK_CalPLLFreq_Mode0(PllSrcClk, u64PllFreq, u32Reg);
		val_ctl0 = (val_ctl0 & ~(0x3f000) & ~(0x7ff)) | u32Reg[0];
		val_ctl1 = (val_ctl1 & ~(0x7 << 4)) | u32Reg[1];
	} else if(pwd->pllmode == 1) {
		u64PllClk = CLK_CalPLLFreq_Mode1(PllSrcClk, u64PllFreq, u32Reg);
		val_ctl0 = (val_ctl0 & ~(0x3f<< 12) & ~(0x7ff)) | u32Reg[0];
		val_ctl1 = (val_ctl1 & ~(0x7 << 4) & ~(0xffffff << 8)) | u32Reg[1];
	} else { //pwd->pllmode == 2
		u64PllClk = CLK_CalPLLFreq_Mode2(PllSrcClk, u64PllFreq, 50000, 194,
		                                 u32Reg); //50khz, 1.94%
		val_ctl0 = (val_ctl0 & ~(0x7ff << 20) & ~(0x3f << 12) & ~(0x7ff)) | u32Reg[0];
		val_ctl1 = (val_ctl1 & ~(0x7 << 4) & ~(0xffffff << 8)) | u32Reg[1];
		val_ctl2 = u32Reg[2];
	}

	//__raw_writel(val_ctl0, pll->ctl0_base);
	//__raw_writel(val_ctl1, pll->ctl1_base);
	//__raw_writel(val_ctl2, pll->ctl2_base);

	return u64PllClk;
}

unsigned long CLK_GetPLLFreq(struct __ma35d1_clk_priv *pc,
                             unsigned long PllSrcClk)
{
	struct __ma35d1_clk_data *data = pc->data;
	struct __ma35d1_wrpll_data *pwd = pc->pwd;
	u32 u32M, u32N, u32P, u32X, u32SR, u32FMOD;
	u32 val_ctl0, val_ctl1, val_ctl2, val_mode;
	unsigned long u64PllClk, u64X;

	pr_debug("%s\n", __func__);
	val_ctl0 = readl(data->va + pwd->ctl0_offs);
	val_ctl1 = readl(data->va + pwd->ctl1_offs);
	val_ctl2 = readl(data->va + pwd->ctl2_offs);

	// PLL Operation mode setting
	val_mode = (val_ctl0 & (0xc0000)) >> 18;
	pwd->pllmode = val_mode;

	if(val_ctl0 == 0)
		u64PllClk = 0;
	else {

		if(pwd->pllmode == 0) {
			u32N = (val_ctl0 & (0x7ff));
			u32M = (val_ctl0 & (0x3f000)) >> 12;

			u32P = ((val_ctl1 & (0x70)) >> 4);

			/* Actual PLL output clock frequency */
			u64PllClk = (PllSrcClk * u32N) / (u32P * (u32M));

		} else if(pwd->pllmode == 1) {
			u32N = (val_ctl0 & (0x7ff));
			u32M = (val_ctl0 & (0x3f000)) >> 12;

			u32P = ((val_ctl1 & (0x70)) >> 4);
			u32X = (val_ctl1 & 0xffffff00) >> 8;

			/* Actual PLL output clock frequency */
			u64X = (u64)u32X;
			u64X = (((u64X * 1000)+500) >> 24);
			u64PllClk = (PllSrcClk * ((u32N*1000) + u64X)) / 1000 / u32P / u32M;

		} else { //pll->mode == 2
			u32N = (val_ctl0 & (0x7ff));
			u32M = (val_ctl0 & (0x3f000)) >> 12;
			u32SR = (val_ctl0 & (0x7ff00000)) >> 20;

			u32P = ((val_ctl1 & (0x70)) >> 4);
			u32X =	(val_ctl1 & 0xffffff00) >> 8;

			u32FMOD = (val_ctl2 & (0x00ffffff));

			/* Actual PLL output clock frequency */
			u64X = (u64)u32X;
			u64X = ((u64X * 1000) >> 24);

			//u64PllClk = (PllSrcClk * ((u32N*1000) + u64X)) / 1000 / u32P / u32M;
			u64PllClk = pwd->clko;
		}
	}

	return u64PllClk;
}

static unsigned long ma35d1_pll_recalc_rate(struct __ma35d1_clk_priv *pc,
        unsigned long parent_rate)
{
	unsigned long ll;

	pr_debug("%s\n", __func__);
	ll = CLK_GetPLLFreq(pc, parent_rate);
	pr_debug("%ld Hz\n", ll);

	return ll;
}

static int ma35d1_pll_set_rate(struct __ma35d1_clk_priv *pc,
                                unsigned long rate,
                                unsigned long parent_rate)
{
	struct __ma35d1_wrpll_data *pwd = pc->pwd;
	unsigned long ll;

	pr_debug("%s\n", __func__);

	ll = pllfreq[pwd->plltype];

	pr_debug("    PLL set rate: [%ld] Hz\n", ll);
	return ll;
}



static int ma35d1_pll_clock_enable(struct __ma35d1_clk_priv *pc, bool enable)
{
	struct __ma35d1_wrpll_data *pwd = pc->pwd;
	struct __ma35d1_clk_data *data = pc->data;
	u32 val;

	pr_debug("%s\n", __func__);
	if (enable) {
		val = readl(data->va+pwd->ctl1_offs);
		val &= ~0x1;/* PD = 0, power down mode disable */
		writel(val, data->va+pwd->ctl1_offs);
		pr_debug("%s Enable:[0x%x]\n", __func__, readl(data->va+pwd->ctl1_offs));
	} else {
		val = readl(data->va+pwd->ctl1_offs);
		val |= 0x1;/* PD = 1, power down mode enable */
		writel(val, data->va+pwd->ctl1_offs);
		pr_debug("%s Disnable:[0x%x]\n", __func__, readl(data->va+pwd->ctl1_offs));
	}

	return 0;
}


static const struct __ma35d1_ccf_ops ma35d1_pll_clk_ops = {
	.set_rate = ma35d1_pll_set_rate,
	.recalc_rate = ma35d1_pll_recalc_rate,
	.enable_clk = ma35d1_pll_clock_enable,
};

static struct __ma35d1_clk_priv __ma35d1_init_plls[] = {
	[capll] = {
		.ops = &ma35d1_pll_clk_ops,
		.gate_data = &__clk_id_gate_sets[capll],
		.pwd = &__ma35d1_capll_data,
	},
	[syspll] = {
		.ops = &ma35d1_pll_clk_ops,
		.gate_data = &__clk_id_gate_sets[syspll],
		.pwd = &__ma35d1_syspll_data,
	},
	[ddrpll] = {
		.ops = &ma35d1_pll_clk_ops,
		.gate_data = &__clk_id_gate_sets[ddrpll],
		.pwd = &__ma35d1_ddrpll_data,
	},
	[apll] = {
		.ops = &ma35d1_pll_clk_ops,
		.gate_data = &__clk_id_gate_sets[apll],
		.pwd = &__ma35d1_apll_data,
	},
	[epll] = {
		.ops = &ma35d1_pll_clk_ops,
		.gate_data = &__clk_id_gate_sets[epll],
		.pwd = &__ma35d1_epll_data,
	},
	[vpll] = {
		.ops = &ma35d1_pll_clk_ops,
		.gate_data = &__clk_id_gate_sets[vpll],
		.pwd = &__ma35d1_vpll_data,
	},
};

static ulong ma35d1_pll_parent_rate(struct __ma35d1_clk_priv *pc)
{
	ulong parent_rate;

	pr_debug("%s\n", __func__);

	parent_rate = clk_get_rate(&pc->data->parent_hxt);
	pr_debug(" PLL source clock: %ld Hz\n", parent_rate);
	return parent_rate;
}


ulong Assign_root_clk(u32 clk_sel_type)
{
	ulong ll = 0;
	switch(clk_sel_type) {
	case TYPE_HXT_HZ:
		ll = hxt_hz;
		break;
	case TYPE_LXT_HZ:
		ll = lxt_hz;
		break;
	case TYPE_HIRC_HZ:
		ll = hirc_hz;
		break;
	case TYPE_LIRC_HZ:
		ll = lirc_hz;
		break;
	case TYPE_CAPLL_HZ:
		ll = pllfreq[0];
		break;
	case TYPE_SYSPLL_HZ:
		ll = pllfreq[1];
		break;
	case TYPE_DDRPLL_HZ:
		ll = pllfreq[2];
		break;
	case TYPE_APLL_HZ:
		ll = pllfreq[3];
		break;
	case TYPE_EPLL_HZ:
		ll = pllfreq[4];
		break;
	case TYPE_VPLL_HZ:
		ll = pllfreq[5];
		break;
	case TYPE_CAPLL_DIV2_HZ:
		ll = pllfreq[0]/2;
		break;
	case TYPE_CAPLL_DIV4_HZ:
		ll = pllfreq[0]/4;
		break;
	case TYPE_EPLL_DIV2_HZ:
		ll = pllfreq[4]/2;
		break;
	case TYPE_EPLL_DIV4_HZ:
		ll = pllfreq[4]/4;
		break;
	case TYPE_VPLL_DIV2_HZ:
		ll =  pllfreq[5]/2;
		break;
	case TYPE_CA35_HZ:
		ll = caclk;
		break;
	case TYPE_AXI0_ACLK_HZ:
		ll = pllfreq[0]/2;
		break;
	case TYPE_SYSCLK0_HZ:
		ll = sysclk[0];
		break;
	case TYPE_SYSCLK1_HZ:
		ll = sysclk[1];
		break;
	case TYPE_SYSCLK1_DIV2_HZ:
		ll = sysclk[1]/2;
		break;
	case TYPE_HCLK0_HZ:
	case TYPE_HCLK1_HZ:
	case TYPE_HCLK2_HZ:
		ll = sysclk[1];
		break;
	case TYPE_HCLK3_HZ:
		ll = sysclk[1]/2;
		break;
	case TYPE_PCLK0_HZ:
	case TYPE_PCLK1_HZ:
	case TYPE_PCLK2_HZ:
		ll = sysclk[1];
		break;
	case TYPE_PCLK3_HZ:
	case TYPE_PCLK4_HZ:
		ll = sysclk[1]/2;
		break;
	case TYPE_PCLK3_DIV_4096_HZ:
	case TYPE_PCLK4_DIV_4096_HZ:
		ll = sysclk[1]/2/4096;
		break;
	case TYPE_USBPHY0_HZ:
		ll = usbphy0_hz;
		break;
	case TYPE_USBPHY1_HZ:
		ll = usbphy1_hz;
		break;
	default:
		ll = hxt_hz;
		break;

	}
	return ll;
}

ulong get_rate_reg(struct clk *clk, void *base)
{
	u32 maskMap[5] = {BITMASK_1, BITMASK_1, BITMASK_2, BITMASK_3, BITMASK_4};
	u32 regVal, gateId, muxId, divId, div_val;
	struct ___ma35d1_clk_reg_info reg_info;
	ulong ll = 0;

	pr_debug("%s\n", __func__);

	gateId = clk->id;
	if ((clk->id >= ca35clk_mux) && (clk->id < dcup_div))
		muxId  = clk->id - ca35clk_mux;
	else
		muxId = UNKNOWN_MUX_ID;

	if (clk->id >= dcup_div)
		divId  = clk->id - dcup_div;
	else
		divId = UNKNOWN_DIV_ID;

	reg_info.clkGate = __clk_id_gate_sets[gateId];
	reg_info.clkMux  = __clk_id_mux_sets[muxId];
	reg_info.clkDiv  = __clk_id_div_sets[divId];

	//Get root clock source
	reg_info.clkGate.rate = Assign_root_clk(__clk_id_gate_sets[gateId].type);

	//Assign_root_clk(u32 clk_sel_type)
	reg_info.clkMux.muxMap[0] = Assign_root_clk(
	                                __clk_sel_type_tbl[muxId].muxMap[0]);
	reg_info.clkMux.muxMap[1] = Assign_root_clk(
	                                __clk_sel_type_tbl[muxId].muxMap[1]);
	reg_info.clkMux.muxMap[2] = Assign_root_clk(
	                                __clk_sel_type_tbl[muxId].muxMap[2]);
	reg_info.clkMux.muxMap[3] = Assign_root_clk(
	                                __clk_sel_type_tbl[muxId].muxMap[3]);

	if(muxId == UNKNOWN_MUX_ID) {
		if(divId == UNKNOWN_DIV_ID) {
			return reg_info.clkGate.rate;
		} else {
			regVal = readl(base + reg_info.clkDiv.offst);
			div_val = (regVal & (maskMap[reg_info.clkDiv.bitWidth] <<
			                     reg_info.clkDiv.bitIdx)) >> reg_info.clkDiv.bitIdx;
			ll = reg_info.clkGate.rate/reg_info.clkDiv.divMap[div_val];
		}
	} else {
		regVal = readl(base + reg_info.clkMux.offst);
		regVal = regVal >> reg_info.clkMux.bitIdx;
		regVal = regVal & maskMap[reg_info.clkMux.bitWidth];
		if(divId == UNKNOWN_DIV_ID) {
			ll = reg_info.clkMux.muxMap[regVal];
			pr_debug("Get rate: %s, [sel_val = %d] @[%ld] Hz\n", clk_names[clk->id], regVal,
			         ll);
		} else {
			regVal = readl(base + reg_info.clkDiv.offst);
			div_val = (regVal & (maskMap[reg_info.clkDiv.bitWidth] <<
			                     reg_info.clkDiv.bitIdx)) >> reg_info.clkDiv.bitIdx;
			ll = reg_info.clkGate.rate/reg_info.clkDiv.divMap[div_val];
			pr_debug("Get rate: %s, regVal=0x%x, [div_val: %d](/%d) @[%ld]\n",
			         clk_names[clk->id], regVal, div_val, reg_info.clkDiv.divMap[div_val], ll);
		}
	}


	return ll;
}

static ulong ma35d1_clk_get_rate(struct clk *clk)
{
	struct __ma35d1_clk_priv *pc = dev_get_priv(clk->dev);
	ulong rate = 0;

	pr_debug("%s\n", __func__);
	if(clk->id <= vpll) {
		if (ARRAY_SIZE(__ma35d1_init_plls) <= clk->id)
			return -ENXIO;
		pc = &__ma35d1_init_plls[clk->id];
		if (!pc->data || !pc->ops->recalc_rate)
			return -ENXIO;
		rate = pc->ops->recalc_rate(pc, ma35d1_pll_parent_rate(pc));
	} else {
		rate = get_rate_reg(clk, pc->base);
	}

	return rate;
}

ulong set_rate_reg(struct clk *clk, ulong rate, void *base)
{
	u32 i, selectIdx = 0xFF, sel_val, div_val;
	u32 maskMap[5] = {BITMASK_1, BITMASK_1, BITMASK_2, BITMASK_3, BITMASK_4};
	u32 regVal, gateId, muxId, divId;
	struct ___ma35d1_clk_reg_info reg_info;
	ulong ll = 0;

	pr_debug("%s\n", __func__);

	gateId = clk->id;
	if ((clk->id >= ca35clk_mux) && (clk->id < dcup_div))
		muxId  = clk->id - ca35clk_mux;
	else
		muxId = UNKNOWN_MUX_ID;

	if (clk->id >= dcup_div)
		divId  = clk->id - dcup_div;
	else
		divId = UNKNOWN_DIV_ID;

	reg_info.clkGate = __clk_id_gate_sets[gateId];
	reg_info.clkMux  = __clk_id_mux_sets[muxId];
	reg_info.clkDiv  = __clk_id_div_sets[divId];

	//Get root clock source
	reg_info.clkGate.rate = Assign_root_clk(__clk_id_gate_sets[gateId].type);

	//Assign_root_clk(u32 clk_sel_type)
	reg_info.clkMux.muxMap[0] = Assign_root_clk(
	                                __clk_sel_type_tbl[muxId].muxMap[0]);
	reg_info.clkMux.muxMap[1] = Assign_root_clk(
	                                __clk_sel_type_tbl[muxId].muxMap[1]);
	reg_info.clkMux.muxMap[2] = Assign_root_clk(
	                                __clk_sel_type_tbl[muxId].muxMap[2]);
	reg_info.clkMux.muxMap[3] = Assign_root_clk(
	                                __clk_sel_type_tbl[muxId].muxMap[3]);

	if(muxId == UNKNOWN_MUX_ID) {
		if(divId == UNKNOWN_DIV_ID) {
			pr_debug("Get rate: %s parent_rate @%ld\n", clk_names[clk->id],
			         reg_info.clkGate.rate);
			return reg_info.clkGate.rate;
		} else {
			regVal = readl(base + reg_info.clkDiv.offst);
			div_val = (regVal & (maskMap[reg_info.clkDiv.bitWidth]
			                     <<reg_info.clkDiv.bitIdx)) >> reg_info.clkDiv.bitIdx;
			ll = reg_info.clkGate.rate/reg_info.clkDiv.divMap[div_val];
			pr_debug("parent rate=%ld, [div_val: %d](/%d), @%ld Hz\n",reg_info.clkGate.rate,
			         div_val, reg_info.clkDiv.divMap[div_val], ll);
		}
	} else {
		regVal = readl(base + reg_info.clkMux.offst);

		for (i=0; i < 16; i++) {
			if (reg_info.clkMux.muxMap[i] == rate) {
				selectIdx = i;
				break;
			}
		}
		if (selectIdx == 0xFF) {
			// not found, need to error handle, SET default value
			sel_val = selectIdx << reg_info.clkMux.bitIdx;
			sel_val = (regVal & ~
			           (maskMap[reg_info.clkMux.bitWidth]<<reg_info.clkMux.bitIdx)) | sel_val;
			writel(sel_val, base + reg_info.clkMux.offst);
			ll = reg_info.clkGate.rate;
			return ll;
		} else {
			sel_val = selectIdx << reg_info.clkMux.bitIdx;
			sel_val = (regVal & ~
			           (maskMap[reg_info.clkMux.bitWidth]<<reg_info.clkMux.bitIdx)) | sel_val;
			writel(sel_val, base + reg_info.clkMux.offst);
			ll = reg_info.clkMux.muxMap[selectIdx];

			// DCUP & EMAC DIV Read only
			if(divId == UNKNOWN_DIV_ID) {
				ll = reg_info.clkMux.muxMap[selectIdx];
			} else {
				regVal = readl(base + reg_info.clkDiv.offst);
				div_val = (regVal & (maskMap[reg_info.clkDiv.bitWidth]
				                     <<reg_info.clkDiv.bitIdx)) >> reg_info.clkDiv.bitIdx;
				ll = reg_info.clkGate.rate/reg_info.clkDiv.divMap[div_val];
				pr_debug("parent rate=%ld, [div_val: %d](/%d), @%ld Hz\n",
				         reg_info.clkGate.rate, div_val, reg_info.clkDiv.divMap[div_val], ll);
			}
		}

		pr_debug("Set rate: [0x%x] (val=0x%x) @%ld\n",
		         readl(base + reg_info.clkMux.offst), selectIdx, ll);
	}

	return ll;
}

static ulong ma35d1_clk_set_rate(struct clk *clk, ulong rate)
{
	ulong rate_val = 0;
	struct __ma35d1_clk_priv *pc = dev_get_priv(clk->dev);

	pr_debug("%s\n", __func__);
	if(clk->id <= vpll) {
		if (ARRAY_SIZE(__ma35d1_init_plls) <= clk->id)
			return -ENXIO;

		pc = &__ma35d1_init_plls[clk->id];
		if (!pc->data || !pc->ops->set_rate)
			return -ENXIO;

		rate_val = pc->ops->recalc_rate(pc, ma35d1_pll_parent_rate(pc));
		if (rate_val)
			return rate_val;
	} else {
		rate_val = set_rate_reg(clk, rate, pc->base);
	}

	return rate_val;
}

static int ma35d1_clk_enable(struct clk *clk)
{
	struct __ma35d1_clk_priv *pc = dev_get_priv(clk->dev);
	int ret = 0;
	u32 regVal, sel_val;

	pr_debug("%s\n", __func__);
	if(clk->id <= vpll) {
		if (ARRAY_SIZE(__ma35d1_init_plls) <= clk->id)
			return -ENXIO;
		pc = &__ma35d1_init_plls[clk->id];
		if (!pc->data)
			return -ENXIO;

		if (pc->ops->enable_clk)
			ret = pc->ops->enable_clk(pc, 1);
	} else {
		regVal = readl(pc->base+__clk_id_gate_sets[clk->id].offst);
		sel_val = (regVal & ~(BITMASK_1 << __clk_id_gate_sets[clk->id].bitIdx)) |
		          (BITMASK_1 << __clk_id_gate_sets[clk->id].bitIdx);
		writel(sel_val, pc->base+__clk_id_gate_sets[clk->id].offst);

		if(clk->id == 12){ /* Set sdh0 mux to syspll */
			regVal=readl(pc->base+0x18);
			regVal|=(0x3<<16);
			writel(regVal,pc->base+0x18);
		}else if(clk->id == 13){ /* Set sdh1 mux to syspll */
			regVal=readl(pc->base+0x18);
			regVal|=(0x3<<18);
			writel(regVal,pc->base+0x18);
		}
	}

	return ret;
}

static int ma35d1_clk_disable(struct clk *clk)
{
	struct __ma35d1_clk_priv *pc = dev_get_priv(clk->dev);
	int ret = 0;
	u32 regVal, sel_val;

	pr_debug("%s\n", __func__);
	if(clk->id <= vpll) {
		if (ARRAY_SIZE(__ma35d1_init_plls) <= clk->id)
			return -ENXIO;

		pc = &__ma35d1_init_plls[clk->id];
		if (!pc->data)
			return -ENXIO;

		if (pc->ops->enable_clk)
			ret = pc->ops->enable_clk(pc, 0);
	} else {
		regVal = readl(pc->base+__clk_id_gate_sets[clk->id].offst);
		sel_val = (regVal & ~(BITMASK_1 << __clk_id_gate_sets[clk->id].bitIdx)) & ~
		          (BITMASK_1 << __clk_id_gate_sets[clk->id].bitIdx);
		writel(sel_val, pc->base+__clk_id_gate_sets[clk->id].offst);
	}

	return ret;
}

ulong find_caclk_rate(void *base)
{
	u32 regVal, idx;
	u32 maskMap[5] = {BITMASK_1, BITMASK_1, BITMASK_2, BITMASK_3, BITMASK_4};
	ulong ll = 0;

	pr_debug("%s\n", __func__);

	idx = ca35clk_mux-ca35clk_mux;
	regVal = readl(base + __clk_id_mux_sets[idx].offst);
	regVal = regVal >> __clk_id_mux_sets[idx].bitIdx;
	regVal = regVal & maskMap[__clk_id_mux_sets[idx].bitWidth];

	switch(regVal) {
	case 0:
		ll = hxt_hz;
		break;
	case 1:
		ll = pllfreq[capll];
		break;
	case 2:
		ll = pllfreq[epll];
		break;
	case 3:
		ll = pllfreq[apll];
		break;
	}

	return ll;
}

ulong find_sysclk_rate(void *base, u8 num)
{
	u32 regVal, idx;
	u32 maskMap[5] = {BITMASK_1, BITMASK_1, BITMASK_2, BITMASK_3, BITMASK_4};
	ulong ll = 0;

	pr_debug("%s\n", __func__);

	idx = (sysclk0_mux - ca35clk_mux) + num;
	regVal = readl(base + __clk_id_mux_sets[idx].offst);
	regVal = regVal >> __clk_id_mux_sets[idx].bitIdx;
	regVal = regVal & maskMap[__clk_id_mux_sets[idx].bitWidth];

	if(num == 0) {
		switch(regVal) {
		case 0:
			ll = pllfreq[epll]/2;
			break;
		default:
			ll = pllfreq[syspll];
			break;
		}
	} else {
		switch(regVal) {
		case 0:
			ll = hxt_hz;
			break;

		case 2:
		case 3:
			ll = pllfreq[apll];
			break;
		case 1:
		default:
			ll = pllfreq[syspll];
			break;
		}
	}
	return ll;
}

static int ma35d1_clk_probe(struct udevice *dev)
{
	int i, err;
	struct __ma35d1_clk_priv *pc;
	struct __ma35d1_clk_data *data = dev_get_priv(dev);

	pr_debug("%s\n", __func__);

	data->va = (void *)dev_read_addr(dev);
	if (IS_ERR(data->va))
		return PTR_ERR(data->va);

	pr_debug("\n%s(dev=%p) 0x%p\n", __func__, dev, data->va);

	err = clk_get_by_index(dev, 0, &data->parent_hxt);
	if (err)
		return err;
	err = clk_get_by_index(dev, 1, &data->parent_lxt);
	if (err)
		return err;
	err = clk_get_by_index(dev, 2, &data->parent_hirc);
	if (err)
		return err;
	err = clk_get_by_index(dev, 3, &data->parent_lirc);
	if (err)
		return err;
	err = clk_get_by_index(dev, 4, &data->parent_usbphy0);
	if (err)
		return err;
	err = clk_get_by_index(dev, 5, &data->parent_usbphy1);
	if (err)
		return err;

	data->parent_hxt.rate = clk_get_rate(&data->parent_hxt);
	hxt_hz = clk_get_rate(&data->parent_hxt);

	data->parent_lxt.rate = clk_get_rate(&data->parent_lxt);
	lxt_hz = clk_get_rate(&data->parent_lxt);

	data->parent_hirc.rate = clk_get_rate(&data->parent_hirc);
	hirc_hz = clk_get_rate(&data->parent_hirc);

	data->parent_lirc.rate = clk_get_rate(&data->parent_lirc);
	lirc_hz = clk_get_rate(&data->parent_lirc);

	data->parent_usbphy0.rate = clk_get_rate(&data->parent_usbphy0);
	usbphy0_hz = clk_get_rate(&data->parent_usbphy0);

	data->parent_usbphy1.rate = clk_get_rate(&data->parent_usbphy1);
	usbphy1_hz = clk_get_rate(&data->parent_usbphy1);

	for (i = 0; i < ARRAY_SIZE(__ma35d1_init_plls); ++i) {
		pc = &__ma35d1_init_plls[i];
		pc->data = data;
		pc->pwd->clko = pllfreq[i]= CLK_GetPLLFreq(pc, data->parent_hxt.rate);
	}

	caclk = find_caclk_rate(data->va);
	sysclk[0] = find_sysclk_rate(data->va, 0);
	sysclk[1] = find_sysclk_rate(data->va, 1);

	return 0;
}

static struct clk_ops ma35d1_clk_ops = {
	.set_rate = ma35d1_clk_set_rate,
	.get_rate = ma35d1_clk_get_rate,
	.enable = ma35d1_clk_enable,
	.disable = ma35d1_clk_disable,
};

static const struct udevice_id ma35d1_clk_ids[] = {
	{ .compatible = "nuvoton,ma35d1-clk" },
	{ },
};

U_BOOT_DRIVER(ma35d1_clk) = {
	.name = "ma35d1_clk",
	.id = UCLASS_CLK,
	.of_match = ma35d1_clk_ids,
	.ops = &ma35d1_clk_ops,
	.priv_auto_alloc_size = sizeof(struct __ma35d1_clk_data),
	.probe = ma35d1_clk_probe,
	.flags = DM_FLAG_PRE_RELOC,
};
