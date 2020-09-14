/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2020 Nuvoton Technology Corp.
 */

#ifndef __DRIVERS_PINCTRL_NVT_H
#define __DRIVERS_PINCTRL_NVT_H

#include <linux/bitops.h>
#include <linux/types.h>

#define MAX_NVT_PINS_ENTRIES 50

/**
 * @priv: common pinctrl private basedata
 * @pin_base: first pin number
 * @nr_pins: number of pins in this bank
 * @name: name of the bank
 * @valid: is all necessary information present
 * @reg_base: register base of the gpio bank
 */
struct nvt_pin_bank {
	struct udevice *dev;
	struct nvt_pinctrl_priv *priv;
	u32 pin_base;
	u8 nr_pins;
	char *name;
	bool valid;
	void __iomem *reg_base;
};

/**
 */
struct nvt_pin_ctrl {
	struct nvt_pin_bank *pin_banks;
	u32 nr_banks;
	u32 nr_pins;
	int (*get_pin_num)(int offset, int shift);
};

/**
 */
struct nvt_pinctrl_priv {
	struct nvt_pin_ctrl *ctrl;
	struct regmap *regmap;
};

extern const struct pinctrl_ops nvt_pinctrl_ops;
int nvt_pinctrl_probe(struct udevice *dev);
void nvt_get_recalced_mux(struct nvt_pin_bank *bank, int pin,
			  int *reg, u8 *bit, int *mask);
bool nvt_get_mux_route(struct nvt_pin_bank *bank, int pin,
		       int mux, u32 *reg, u32 *value);
int nvt_get_mux_data(int mux_type, int pin, u8 *bit, int *mask);
int nvt_translate_drive_value(int type, int strength);
int nvt_translate_pull_value(int type, int pull);

#endif /* __DRIVERS_PINCTRL_NVT_H */
