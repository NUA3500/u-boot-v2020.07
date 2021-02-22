// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020, Nuvoton Technology Corp. - All Rights Reserved
 */

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <log.h>
#include <wdt.h>
#include <asm/io.h>
#include <asm/system.h>
#include <syscon.h>
#include <regmap.h>
#include <ma35d1-sys.h>
#include <linux/bitops.h>

/* WDT register, bit field and offset definition */
#define WDT_CTL		0x00		/* Contnrol register */
#define WDTEN		BIT(7)		/* Enabled WDT */
#define RSTEN		BIT(1)		/* Enabled WDT reset system */
#define TOUTSEL_OFST	8		/* Offset for timeout selection */
#define WDT_RSTCNT	0x08		/* Reset Counter Register */
#define RESET_COUNTER	0x00005AA5	/* Magic word to reset counter */

struct ma35d1_wdt_priv {
	fdt_addr_t base;		/* WDT Register address */
	struct regmap *sys;		/* SYS base */
};

static void ma35d1_reg_lock(struct ma35d1_wdt_priv *priv)
{
	u32 el = current_el();
	u32 offset = el == 3 ? REG_SYS_RLKTZS : REG_SYS_RLKTZNS;

	regmap_write(priv->sys, offset, 1);
}

static int ma35d1_reg_unlock(struct ma35d1_wdt_priv *priv)
{
	u32 reg, el = current_el();
	u32 offset = el == 3 ? REG_SYS_RLKTZS : REG_SYS_RLKTZNS;

	regmap_read(priv->sys, offset, &reg);

	if (reg)
		return 1;
	do {
		regmap_write(priv->sys, offset, 0x59);
		regmap_write(priv->sys, offset, 0x16);
		regmap_write(priv->sys, offset, 0x88);
		regmap_read(priv->sys, offset, &reg);
	} while (reg != 1);

	return 0;
}

static int ma35d1_wdt_reset(struct udevice *dev)
{
	struct ma35d1_wdt_priv *priv = dev_get_priv(dev);

	writel(RESET_COUNTER, priv->base + WDT_RSTCNT);

	return 0;
}

static int ma35d1_wdt_start(struct udevice *dev, u64 timeout_ms, ulong flags)
{
	struct ma35d1_wdt_priv *priv = dev_get_priv(dev);
	u32 reg = WDTEN | RSTEN;
	int lock;

	if (timeout_ms <= 500)
		reg |= (5 << TOUTSEL_OFST);
	else if (timeout_ms <= 2000)	// 2 second
		reg |= (6 << TOUTSEL_OFST);
	else if (timeout_ms <= 8000)	// 8 second
		reg |= (7 << TOUTSEL_OFST);
	else // Any setting above 8 second is treat as 16 second
		reg |= (8 << TOUTSEL_OFST);

	/* Enable watchdog */
	lock = ma35d1_reg_unlock(priv);

	writel(reg, priv->base + WDT_CTL);

	if (!lock)
		ma35d1_reg_lock(priv);

	return 0;
}

static int ma35d1_wdt_stop(struct udevice *dev)
{
	struct ma35d1_wdt_priv *priv = dev_get_priv(dev);
	int lock;

	lock = ma35d1_reg_unlock(priv);
	writel(0, priv->base + WDT_CTL);
	if (!lock)
		ma35d1_reg_lock(priv);
	return 0;
}

static int ma35d1_wdt_probe(struct udevice *dev)
{
	struct ma35d1_wdt_priv *priv = dev_get_priv(dev);
	struct ofnode_phandle_args args;
	struct clk clk;
	int ret;

	ret = dev_read_phandle_with_args(dev, "nuvoton,ma35d1-sys", NULL,
					 0, 0, &args);
	if (ret) {
		dev_err(dev, "Failed to get syscon: %d\n", ret);
		return ret;
	}

	priv->sys = syscon_node_to_regmap(args.node);
	if (IS_ERR(priv->sys)) {
		ret = PTR_ERR(priv->sys);
		dev_err(dev, "can't get syscon: %d\n", ret);
		return ret;
	}

	priv->base = devfdt_get_addr(dev);
	if (priv->base == FDT_ADDR_T_NONE)
		return -EINVAL;

	/* Enable clock, default clock is from 32k LIRC */
	ret = clk_get_by_name(dev, "wdt", &clk);
	if (ret)
		return ret;

	ret = clk_enable(&clk);
	if (ret)
		return ret;

	return 0;
}

static const struct wdt_ops ma35d1_wdt_ops = {
	.start = ma35d1_wdt_start,
	.stop = ma35d1_wdt_stop,
	.reset = ma35d1_wdt_reset,
};

static const struct udevice_id ma35d1_wdt_match[] = {
	{ .compatible = "nuvoton,ma35d1-wdt" },
	{ }
};

U_BOOT_DRIVER(ma35d1_wdt) = {
	.name = "ma35d1-wdt",
	.id = UCLASS_WDT,
	.of_match = ma35d1_wdt_match,
	.priv_auto_alloc_size = sizeof(struct ma35d1_wdt_priv),
	.probe = ma35d1_wdt_probe,
	.ops = &ma35d1_wdt_ops,
};
