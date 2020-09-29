// SPDX-License-Identifier: GPL-2.0
/*
 * Nuvoton UART driver
 *
 * Copyright (C) 2020 Nuvoton Technology Corporation
 */

#include <clk.h>
#include <common.h>
#include <div64.h>
#include <dm.h>
#include <dt-structs.h>
#include <errno.h>
#include <log.h>
#include <serial.h>
#include <watchdog.h>
#include <asm/io.h>
#include <asm/types.h>
#include <linux/err.h>
#include <dm/device_compat.h>
#include <debug_uart.h>

DECLARE_GLOBAL_DATA_PTR;

static int nua3500_serial_putc(struct udevice *dev, const char ch);
static int nua3500_serial_getc(struct udevice *dev);
static int nua3500_serial_setbrg(struct udevice *dev, int baudrate);

#define TX_RX_FIFO_RESET    0x06
#define ENABLE_DLAB         0x80    // Enable Divider Latch Access
#define DISABLE_DLAB        0x7F
#define ENABLE_TIME_OUT     (0x80+0x20)
#define THR_EMPTY           0x20    // Transmit Holding Register Empty
#define RX_DATA_READY       0x01
#define RX_FIFO_EMPTY       0x4000
#define TX_FIFO_EMPTY       0x400000
#define TX_FIFO_FULL        0x800000

struct nua3500_serial_regs {
	volatile u32 RBR_THR;
	volatile u32 IER;
	volatile u32 FCR;
	volatile u32 LCR;
	volatile u32 MCR;
	volatile u32 MSR;
	volatile u32 FSR;
	volatile u32 ISR;
	volatile u32 TOR;
	volatile u32 BAUD;
	volatile u32 IRCR;
	volatile u32 ALTCON;
	volatile u32 FUNSEL;
};

struct nua3500_serial_platdata {
	unsigned long base;
	unsigned int clock;
};

struct nua3500_serial_priv {
	struct nua3500_serial_regs *regs;
};

static int nua3500_serial_putc(struct udevice *dev, const char ch)
{
	struct nua3500_serial_priv *priv = dev_get_priv(dev);
	struct nua3500_serial_regs *regs = priv->regs;

	if(regs->FSR & TX_FIFO_FULL)
		return -EAGAIN;
	else
		regs->RBR_THR = ch;

	return 0;
}

static int nua3500_serial_pending(struct udevice *dev, bool input)
{
	struct nua3500_serial_priv *priv = dev_get_priv(dev);
	struct nua3500_serial_regs *regs = priv->regs;
	unsigned int lsr;

	lsr = regs->FSR;

	if (input) {
		return (lsr & RX_FIFO_EMPTY) ? 0 : 1;
	} else {
		return (lsr & TX_FIFO_EMPTY) ? 0 : 1;
	}
}

static int nua3500_serial_getc(struct udevice *dev)
{
	struct nua3500_serial_priv *priv = dev_get_priv(dev);
	struct nua3500_serial_regs *regs = priv->regs;

	if (!(regs->FSR & RX_FIFO_EMPTY))
		return (regs->RBR_THR);
	else
		return -EAGAIN;
}

static int nua3500_serial_setbrg(struct udevice *dev, int baudrate)
{
	struct nua3500_serial_priv *priv = dev_get_priv(dev);
	struct nua3500_serial_regs *regs = priv->regs;

	// Set for emulation
	regs->LCR |=0x07;
	regs->BAUD = 0x3000000E;
	regs->FCR |=0x02;

	return 0;
}

static const struct dm_serial_ops nua3500_serial_ops = {
	.putc = nua3500_serial_putc,
	.pending = nua3500_serial_pending,
	.getc = nua3500_serial_getc,
	.setbrg = nua3500_serial_setbrg,
};

static int nua3500_serial_probe(struct udevice *dev)
{
	struct nua3500_serial_platdata *plat = dev_get_platdata(dev);
	struct nua3500_serial_priv *priv = dev_get_priv(dev);
	fdt_addr_t addr;

	addr = devfdt_get_addr(dev);

	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;

	plat->base = addr;
	priv->regs = (struct nua3500_serial_regs *)plat->base;

	//enable clock
	__raw_writel(__raw_readl(0X4046020C) | (0x1 << 12), 0X4046020C);
	__raw_writel(__raw_readl(0X40460220) & ~(3 << 16), 0X40460220);

	return 0;
}

static const struct udevice_id nua3500_serial_id[] = {
	{.compatible = "nuvoton,nua3500-uart"},
	{}
};

U_BOOT_DRIVER(serial_nua3500) = {
	.name = "serial_nua3500",
	.id = UCLASS_SERIAL,
	.of_match = nua3500_serial_id,
	.platdata_auto_alloc_size = sizeof(struct nua3500_serial_platdata),
	.probe = nua3500_serial_probe,
	.ops = &nua3500_serial_ops,
	.priv_auto_alloc_size = sizeof(struct nua3500_serial_priv),
	.flags = DM_FLAG_PRE_RELOC,
};



