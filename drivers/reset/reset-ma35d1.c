// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2020, Nuvoton Technology corp.
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <log.h>
#include <reset-uclass.h>
#include <dm/device_compat.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/bitops.h>
#include <syscon.h>
#include <regmap.h>
#include <dt-bindings/reset/nuvoton,ma35d1-reset.h>
#include <ma35d1-sys.h>

#define RST_PRE_REG	32

struct ma35d1_reset_priv {
	struct regmap *base;
};

static int ma35d1_reset_request(struct reset_ctl *reset_ctl)
{
	return 0;
}

static int ma35d1_reset_free(struct reset_ctl *reset_ctl)
{
	return 0;
}

static void ma35d1_reg_lock(struct ma35d1_reset_priv *priv)
{
	u32 el = current_el();
	u32 offset = el == 3 ? REG_SYS_RLKTZS : REG_SYS_RLKTZNS;

	regmap_write(priv->base, offset, 1);
}

static int ma35d1_reg_unlock(struct ma35d1_reset_priv *priv)
{
	u32 reg, el = current_el();
	u32 offset = el == 3 ? REG_SYS_RLKTZS : REG_SYS_RLKTZNS;

	regmap_read(priv->base, offset, &reg);

	if (reg)
		return 1;
	do {
		regmap_write(priv->base, offset, 0x59);
		regmap_write(priv->base, offset, 0x16);
		regmap_write(priv->base, offset, 0x88);
		regmap_read(priv->base, offset, &reg);
	} while (reg != 1);

	return 0;
}

static int ma35d1_reset_assert(struct reset_ctl *reset_ctl)
{
	struct ma35d1_reset_priv *priv = dev_get_priv(reset_ctl->dev);
	int offset = (reset_ctl->id / RST_PRE_REG) * 4;
	u32 reg, lock;

	lock = ma35d1_reg_unlock(priv);
	regmap_read(priv->base, REG_SYS_IPRST0 + offset, &reg);
	reg |= 1 << (reset_ctl->id % RST_PRE_REG);

	regmap_write(priv->base, REG_SYS_IPRST0 + offset, reg);

	if (!lock)
		ma35d1_reg_lock(priv);

	return 0;
}

static int ma35d1_reset_deassert(struct reset_ctl *reset_ctl)
{
	struct ma35d1_reset_priv *priv = dev_get_priv(reset_ctl->dev);
	int offset = (reset_ctl->id / RST_PRE_REG) * 4;
	u32 reg, lock;

	lock = ma35d1_reg_unlock(priv);
	regmap_read(priv->base, REG_SYS_IPRST0 + offset, &reg);
	reg &= ~(1 << (reset_ctl->id % RST_PRE_REG));

	regmap_write(priv->base, REG_SYS_IPRST0 + offset, reg);

	if (!lock)
		ma35d1_reg_lock(priv);

	return 0;
}

static const struct reset_ops ma35d1_reset_ops = {
	.request	= ma35d1_reset_request,
	.rfree		= ma35d1_reset_free,
	.rst_assert	= ma35d1_reset_assert,
	.rst_deassert	= ma35d1_reset_deassert,
};

static const struct udevice_id ma35d1_reset_ids[] = {
	{ .compatible = "nuvoton,ma35d1-reset" },
	{ }
};

static int ma35d1_reset_probe(struct udevice *dev)
{
	struct ma35d1_reset_priv *priv = dev_get_priv(dev);
	struct ofnode_phandle_args args;
	int ret;

	ret = dev_read_phandle_with_args(dev, "nuvoton,ma35d1-sys", NULL,
					 0, 0, &args);
	if (ret) {
		dev_err(dev, "Failed to get syscon: %d\n", ret);
		return ret;
	}

	priv->base = syscon_node_to_regmap(args.node);
	if (IS_ERR(priv->base)) {
		ret = PTR_ERR(priv->base);
		dev_err(dev, "can't get syscon: %d\n", ret);
		return ret;
	}

	return 0;
}

U_BOOT_DRIVER(ma35d1_reset) = {
	.name			= "ma35d1_reset",
	.id			= UCLASS_RESET,
	.of_match		= ma35d1_reset_ids,
	.probe			= ma35d1_reset_probe,
	.priv_auto_alloc_size	= sizeof(struct ma35d1_reset_priv),
	.ops			= &ma35d1_reset_ops,
};
