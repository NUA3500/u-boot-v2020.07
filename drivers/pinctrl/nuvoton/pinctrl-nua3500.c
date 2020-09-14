// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020 Nuvoton Technology Corp.
 */

#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <regmap.h>
#include <syscon.h>
#include <linux/bitops.h>

#include "pinctrl-nvt.h"

int nua3500_get_pin_num(int offset, int shift)
{
	// 8 pins per register.
	return (offset - 0x80) * 2 + shift / 4;
}

static struct nvt_pin_ctrl nua3500_pin_ctrl = {
	.get_pin_num = nua3500_get_pin_num,
};

static const struct udevice_id nua3500_pinctrl_ids[] = {
	{
	 .compatible = "nuvoton,nua3500-pinctrl",
	 .data = (ulong)&nua3500_pin_ctrl},
	{}
};

U_BOOT_DRIVER(pinctrl_nvt) = {
	.name = "nua3500-pinctrl",
	.id = UCLASS_PINCTRL,
	.of_match = nua3500_pinctrl_ids,
	.priv_auto_alloc_size = sizeof(struct nvt_pinctrl_priv),
	.ops = &nvt_pinctrl_ops,
	.probe = nvt_pinctrl_probe,
};
