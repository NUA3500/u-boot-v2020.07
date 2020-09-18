// SPDX-License-Identifier: GPL-2.0
/*
 * Configuration for Nuvoton NUA3500
 *
 * Copyright (C) 2020 Nuvoton Technology Corporation.
 */

#include <clk.h>
#include <common.h>
#include <cpu_func.h>
#include <dm.h>
#include <fdtdec.h>
#include <init.h>
#include <ram.h>
/* TODO: CWWeng 2020/7/13
#include <asm/arch/misc.h>
*/
#include <asm/armv8/mmu.h>
#include <asm/cache.h>
#include <asm/sections.h>
#include <dm/uclass.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void);
int dram_init_banksize(void);
void reset_cpu(ulong addr);
int print_cpuinfo(void);
int show_board_info(void);

#define SYS_CHIPCFG	0x404601F4

int timer_init(void)
{
	unsigned long freq = 12000000;

	/* Update clock frequency */
	asm volatile("msr cntfrq_el0, %0" : : "r" (freq) : "memory");

	gd->arch.tbl = 0;
	gd->arch.tbu = 0;

	return 0;
}

int dram_init(void)
{
	unsigned int ddr_size;

	ddr_size = ((readl(SYS_CHIPCFG) & 0xF0000) >> 16);
	switch (ddr_size) {
		case 8:
			gd->ram_size = 0x80000000; /* 2048 MB */
			break;
		case 7:
			gd->ram_size = 0x40000000; /* 1024 MB */
			break;
		case 6:
			gd->ram_size = 0x20000000; /* 512 MB */
			break;
		case 5:
			gd->ram_size = 0x10000000; /* 256 MB */
			break;
		case 4:
			gd->ram_size = 0x8000000; /* 128 MB */
			break;
		case 3:
			gd->ram_size = 0x4000000; /* 64 MB */
			break;
		case 2:
			gd->ram_size = 0x2000000; /* 32 MB */
			break;
		case 1:
			gd->ram_size = 0x1000000; /* 16 MB */
			break;
		case 0: /* Non MCP */
			gd->ram_size = CONFIG_SYS_SDRAM_SIZE; /* 256 MB */
			break;
	}

	return 0;

#if 0
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
	return 0;
#endif

/* TODO: CWWeng 2020/7/13
	int ret;

	ret = fdtdec_setup_memory_banksize();
	if (ret)
		return ret;

	return fdtdec_setup_mem_size_base();
*/
}

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = gd->ram_base;
	gd->bd->bi_dram[0].size = gd->ram_size;

	return 0;
}

void reset_cpu(ulong addr)
{
	psci_system_reset();
}

int print_cpuinfo(void)
{
	printf("CPU:   Nuvoton NUA3500\n");
	return 0;
}

int show_board_info(void)
{
	printf("Board:   Nuvoton NUA3500\n");
	return 0;
}

static struct mm_region nua3500_mem_map[] = {
	{
		/* DDR */
		.virt = 0x80000000UL,
		.phys = 0x80000000UL,
		.size = 0x20000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) | PTE_BLOCK_OUTER_SHARE,
	}, {
		0,
	}
};

struct mm_region *mem_map = nua3500_mem_map;

#if 0 /* TODO : CWWeng 2020.8.31 : update mem_map by inspecting the DTB. */
/* reference arch/arm/mach-bcm283x/init.c */
/*
 * I/O address space varies on different chip versions.
 * We set the base address by inspecting the DTB.
 */
static const struct udevice_id board_ids[] = {
	{ .compatible = "nuvoton,nua3500", .data = (ulong)&nua3500_mem_map},
	{ },
};

static void nua3500_update_mem_map(struct mm_region *pd)
{
	mem_map[0].virt = pd[0].virt;
	mem_map[0].phys = pd[0].phys;
	mem_map[0].size = pd[0].size;
	mem_map[0].attrs = pd[0].attrs;
}

int mach_cpu_init(void)
{
	int ret;
	struct mm_region *mm;
	const struct udevice_id *of_match = board_ids;

	ret = fdt_node_check_compatible(gd->fdt_blob, 0,
					of_match->compatible);
	if (!ret) {
		mm = (struct mm_region *)of_match->data;
		nua3500_update_mem_map(mm);
	}

	return 0;
}
#endif
