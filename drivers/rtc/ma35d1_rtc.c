/*
 * Copyright (c) 2020 Nuvoton technology corporation.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#include <common.h>
#include <command.h>
#include <clk.h>
#include <dm.h>
#include <asm/io.h>
#include <dm/device_compat.h>
#include <linux/bitops.h>
#include <linux/iopoll.h>
#include <rtc.h>

#define		REG_RTC_INIR                    (0x0000) /* Initial register */
#define		REG_RTC_AER                     (0x0004) /* Access Enable Register*/
#define		REG_RTC_TIME                    (0x0C)   /* Time Loading Register */
#define		REG_RTC_CAL                     (0x10)   /* Calendar Loading Register */
#define		REG_RTC_TIMEFMT                 (0x14)   /* Time Format Selection Register */
#define		REG_RTC_WEEKDAY                 (0x18)   /* Day of the Week Register */
#define		REG_RTC_INTSTS                  (0x002C) /* Interrupt Status */
#define		REG_RTC_SPARE                   (0x0040) /* Spare register */

#define		REG_RTC_INIR_ACK                (0x1)
#define		REG_RTC_INIR_NUMBER             (0xa5eb1357)
#define		RTC_INIT_TIMEOUT                (100000)

struct ma35d1_rtc_priv {
	fdt_addr_t base;
};

/*****************************************************************************
 * get time
 *****************************************************************************/
int ma35d1_rtc_get(struct udevice *dev, struct rtc_time *tmp)
{
	struct ma35d1_rtc_priv *priv = dev_get_priv(dev);
	uint32_t val = readl(priv->base + REG_RTC_INIR);
	int ret;
	uint32_t u32Time;
	uint32_t u32Cal;
	uint32_t u32Wday;

	if(!(val & REG_RTC_INIR_ACK)){
		/* init RTC */
		writel(REG_RTC_INIR_NUMBER, priv->base + REG_RTC_INIR);

		ret = readl_poll_timeout(priv->base + REG_RTC_INIR, val, (val & REG_RTC_INIR_ACK), RTC_INIT_TIMEOUT);
		if (ret){
			dev_dbg(dev, "%s timeout\n", __func__);
			return ret;
		}
	}

	u32Cal  = readl(priv->base + REG_RTC_CAL);
	u32Wday = readl(priv->base + REG_RTC_WEEKDAY);
	u32Time = readl(priv->base + REG_RTC_TIME);

	tmp->tm_year	= 2000+ (((u32Cal >> 20) & 0xf) * 10) + ((u32Cal >> 16) & 0xf);
	tmp->tm_mon	= (((u32Cal >> 12) & 0xf) * 10) + ((u32Cal >> 8) & 0xf);
	tmp->tm_mday	= (((u32Cal >> 4) & 0xf) * 10) + ((u32Cal >> 0) & 0xf);
	tmp->tm_wday	= u32Wday;
	tmp->tm_hour	= (((u32Time >> 20) & 0xf) * 10) + ((u32Time >> 16) & 0xf);
	tmp->tm_min	= (((u32Time >> 12) & 0xf) * 10) + ((u32Time >> 8) & 0xf);
	tmp->tm_sec	= (((u32Time >> 4) & 0xf) * 10) + ((u32Time >> 0) & 0xf);

	return 0;
}

/*****************************************************************************
 * set time
 *****************************************************************************/
int ma35d1_rtc_set(struct udevice *dev, const struct rtc_time *tmp)
{
	struct ma35d1_rtc_priv *priv = dev_get_priv(dev);
	uint32_t val = readl(priv->base + REG_RTC_INIR);
	int ret;
	uint32_t u32Time;
	uint32_t u32Cal;
	uint32_t u32Wday;

	dev_dbg(dev, "\n Set DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d \n",
	        tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
	        tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	if(!(val & REG_RTC_INIR_ACK)){
		/* init RTC */
		writel(REG_RTC_INIR_NUMBER, priv->base + REG_RTC_INIR);

		ret = readl_poll_timeout(priv->base + REG_RTC_INIR, val, (val & REG_RTC_INIR_ACK), RTC_INIT_TIMEOUT);
		if (ret){
			dev_dbg(dev, "%s timeout\n", __func__);
			return ret;
		}
	}

	u32Time = ((tmp->tm_sec%10) << 0 ) | ((tmp->tm_sec/10) << 4 ) |
	          ((tmp->tm_min%10) << 8 ) | ((tmp->tm_min/10) << 12) |
	          ((tmp->tm_hour%10)<< 16) | ((tmp->tm_hour/10)<< 20);

	writel(u32Time, priv->base + REG_RTC_TIME);

	u32Cal  = ((tmp->tm_mday%10) << 0 ) | ((tmp->tm_mday/10) << 4 ) |
	          ((tmp->tm_mon %10) << 8 ) | ((tmp->tm_mon /10) << 12) |
	          ((tmp->tm_year%10)<< 16) | (((tmp->tm_year/10)%10)<< 20);

	writel(u32Cal, priv->base + REG_RTC_CAL);

	u32Wday = (tmp->tm_wday & 0x7);

	writel(u32Wday, priv->base + REG_RTC_WEEKDAY);

	return 0;
}

/*****************************************************************************
 * reset rtc
 *****************************************************************************/
static int ma35d1_rtc_reset(struct udevice *dev)
{
	return 0; /* nothing to do */
}

static int ma35d1_rtc_probe(struct udevice *dev)
{
	struct ma35d1_rtc_priv *priv = dev_get_priv(dev);
	uint32_t val;
	struct clk clk;
	int ret;

	priv->base = dev_read_addr(dev);
	if (priv->base == FDT_ADDR_T_NONE)
		return -EINVAL;

	val = readl(priv->base + REG_RTC_INIR);

	ret = clk_get_by_index(dev, 0, &clk);
	if (ret)
		return ret;

	ret = clk_enable(&clk);
	if (ret) {
		clk_free(&clk);
		return ret;
	}

	/* init RTC */
	writel(REG_RTC_INIR_NUMBER, priv->base + REG_RTC_INIR);

	ret = readl_poll_timeout(priv->base + REG_RTC_INIR,
				 val,
				 (val & REG_RTC_INIR_ACK),
				 RTC_INIT_TIMEOUT);
	if (ret){
		#if 0
		clk_disable(&clk);
		clk_free(&clk);
		#endif
		dev_dbg(dev, "%s timeout\n", __func__);
		return ret;
	}

	return ret;
}



static const struct rtc_ops ma35d1_rtc_ops = {
	.get = ma35d1_rtc_get,
	.set = ma35d1_rtc_set,
	.reset = ma35d1_rtc_reset,
};

static const struct udevice_id ma35d1_rtc_ids[] = {
	{ .compatible = "nuvoton,ma35d1-rtc" },
	{ }
};

U_BOOT_DRIVER(rtc_nuvoton) = {
	.name	= "rtc-nuvoton",
	.id	= UCLASS_RTC,
	.probe	= ma35d1_rtc_probe,
	.of_match = ma35d1_rtc_ids,
	.ops	= &ma35d1_rtc_ops,
	.priv_auto_alloc_size = sizeof(struct ma35d1_rtc_priv),
};




