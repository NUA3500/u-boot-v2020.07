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
/* TODO : CWWeng 2020/7/13
	icache_enable();
*/

	return 0;
}

void enable_caches(void)
{
	/* Enable D-cache. I-cache is already enabled in start.S */
/* TODO : CWWeng 2020/7/13
	dcache_enable();
*/
}
