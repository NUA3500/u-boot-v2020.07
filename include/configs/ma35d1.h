/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Configuration for Nuvoton MA35D1
 *
 * Copyright (C) 2020 Nuvoton Technology Corporation
 */

#ifndef __MA35D1_H
#define __MA35D1_H

#include <linux/sizes.h>

#define CONFIG_CPU_ARMV8

#define CONFIG_SYS_LOAD_ADDR			0x81000000

#define CONFIG_SYS_MALLOC_LEN			SZ_2M
#define CONFIG_SYS_BOOTM_LEN			SZ_64M

/* Uboot definition */
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_TEXT_BASE + \
						SZ_2M - \
						GENERATED_GBL_DATA_SIZE)


#define CONFIG_SYS_MMC_ENV_DEV 0

#define CONFIG_ETHPRIME			"eth0"

/*
 * NAND flash configuration
 */
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_SYS_MAX_NAND_DEVICE	1

#define CONFIG_DW_ALTDESCRIPTOR

#endif
