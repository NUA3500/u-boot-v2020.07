/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Configuration for Nuvoton NUA3500
 *
 * Copyright (C) 2020 Nuvoton Technology Corporation
 */

#ifndef __NUA3500_H
#define __NUA3500_H

#include <linux/sizes.h>

#define CONFIG_CPU_ARMV8

#define COUNTER_FREQUENCY			12000000

/* DRAM definition */
#define CONFIG_SYS_SDRAM_BASE			0x80000000
#define CONFIG_SYS_SDRAM_SIZE			0x10000000

/* FDT address */
#define CONFIG_SYS_FDT_BASE			0x88000000

#define CONFIG_SYS_LOAD_ADDR			0x81000000
#define CONFIG_LOADADDR				CONFIG_SYS_LOAD_ADDR

#define CONFIG_SYS_MALLOC_LEN			SZ_1M
#define CONFIG_SYS_BOOTM_LEN			SZ_64M

/* Uboot definition */
#define CONFIG_SYS_INIT_SP_ADDR			(CONFIG_SYS_TEXT_BASE + \
						SZ_2M - \
						GENERATED_GBL_DATA_SIZE)
/* TODO: CWWeng 2020/7/13
ENV related settings
CONFIG_EXTRA_ENV_SETTINGS
*/

#define CONFIG_SYS_MMC_ENV_DEV 0

/*
 * NAND flash configuration
 */
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_SYS_MAX_NAND_DEVICE	1

#define CONFIG_DW_ALTDESCRIPTOR

#endif

