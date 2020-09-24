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
#include <linux/bitops.h>
#include <syscon.h>
#include <regmap.h>
#include <dt-bindings/reset/nuvoton,nua3500-reset.h>
#include <nua3500-sys.h>

#define RST_PRE_REG	32

struct nua3500_reset_priv {
	struct regmap *base;
};

static int nua3500_reset_request(struct reset_ctl *reset_ctl)
{
	return 0;
}

static int nua3500_reset_free(struct reset_ctl *reset_ctl)
{
	return 0;
}

static void nua3500_reg_lock(struct nua3500_reset_priv *priv)
{
	regmap_write(priv->base, REG_SYS_RLKTZS, 1);
}

static int nua3500_reg_unlock(struct nua3500_reset_priv *priv)
{
	u32 reg;

	regmap_read(priv->base, REG_SYS_RLKTZS, &reg);

	if (reg)
		return 1;
	do {
		regmap_write(priv->base, REG_SYS_RLKTZS, 0x59);
		regmap_write(priv->base, REG_SYS_RLKTZS, 0x16);
		regmap_write(priv->base, REG_SYS_RLKTZS, 0x88);
		regmap_read(priv->base, REG_SYS_RLKTZS, &reg);
	} while (reg != 1);

	return 0;
}

static int nua3500_reset_assert(struct reset_ctl *reset_ctl)
{
	struct nua3500_reset_priv *priv = dev_get_priv(reset_ctl->dev);
	int offset = (reset_ctl->id / RST_PRE_REG) * 4;
	u32 reg, lock;

	lock = nua3500_reg_unlock(priv);
	regmap_read(priv->base, REG_SYS_IPRST0 + offset, &reg);
	reg |= 1 << (reset_ctl->id % RST_PRE_REG);

	regmap_write(priv->base, REG_SYS_IPRST0 + offset, reg);

	if (!lock)
		nua3500_reg_lock(priv);

	return 0;
}

static int nua3500_reset_deassert(struct reset_ctl *reset_ctl)
{
	struct nua3500_reset_priv *priv = dev_get_priv(reset_ctl->dev);
	int offset = (reset_ctl->id / RST_PRE_REG) * 4;
	u32 reg, lock;

	lock = nua3500_reg_unlock(priv);
	regmap_read(priv->base, REG_SYS_IPRST0 + offset, &reg);
	reg &= ~(1 << (reset_ctl->id % RST_PRE_REG));

	regmap_write(priv->base, REG_SYS_IPRST0 + offset, reg);

	if (!lock)
		nua3500_reg_lock(priv);

	return 0;
}

static const struct reset_ops nua3500_reset_ops = {
	.request	= nua3500_reset_request,
	.rfree		= nua3500_reset_free,
	.rst_assert	= nua3500_reset_assert,
	.rst_deassert	= nua3500_reset_deassert,
};

static const struct udevice_id nua3500_reset_ids[] = {
	{ .compatible = "nuvoton,nua3500-reset" },
	{ }
};

static int nua3500_reset_probe(struct udevice *dev)
{
	struct nua3500_reset_priv *priv = dev_get_priv(dev);
	struct ofnode_phandle_args args;
	int ret;

	ret = dev_read_phandle_with_args(dev, "nuvoton,nua3500-sys", NULL,
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

U_BOOT_DRIVER(nua3500_reset) = {
	.name			= "nua3500_reset",
	.id			= UCLASS_RESET,
	.of_match		= nua3500_reset_ids,
	.probe			= nua3500_reset_probe,
	.priv_auto_alloc_size	= sizeof(struct nua3500_reset_priv),
	.ops			= &nua3500_reset_ops,
};
