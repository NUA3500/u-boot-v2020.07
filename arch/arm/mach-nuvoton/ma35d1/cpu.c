// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020 Nuvoton Inc.
 */

#include <common.h>
#include <cpu_func.h>
#include <asm/armv8/mmu.h>
#include <asm/cache.h>
#include <dm.h>
#include <init.h>
#include <wdt.h>
#include <dm/uclass-internal.h>

DECLARE_GLOBAL_DATA_PTR;

int arch_cpu_init(void)
{
	return 0;
}

