// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020 Nuvoton Inc.
 */

#include <common.h>
#include <cpu_func.h>
#include <dm.h>
#include <init.h>
#include <wdt.h>
#include <dm/uclass-internal.h>

int arch_cpu_init(void)
{

	return 0;
}

void enable_caches(void)
{
	icache_enable();
}
