// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2020
 * Nuvoton Technology Corp. <www.nuvoton.com>
 *
 * SPI driver for NUA3500
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <malloc.h>
#include <asm/io.h>
#include <clk.h>
#include <common.h>
#include <dm.h>
#include <errno.h>
#include <fdtdec.h>
#include <dm/device_compat.h>
#include <linux/bitops.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/ioport.h>
//#include <mach/clk.h>
#include <clk.h>
#include <spi.h>
#include <spi-mem.h>

/* QSPI register offsets */
#define	CTL	0x0
#define	CLKDIV	0x4
#define	SSCTL	0x8
#define	FIFOCTL	0x10
#define	STATUS	0x14
#define	TX	0x20
#define	RX	0x30

#define ENINT           (0x01 << 17)
#define TXNEG           (0x01 << 2)
#define RXNEG           (0x01 << 1)
#define LSB             (0x01 << 13)
#define SELECTLEV       (0x01 << 2)
#define SELECTPOL       (0x01 << 3)
#define SELECTSLAVE0    0x01
#define SELECTSLAVE1    0x02
#define SPIEN           0x01
#define SPIENSTS        (0x01 << 15)
#define TXRXRST         (0x01 << 23)
#define TXFULL          0x20000
#define RXEMPTY         0x100
#define SPI_BUSY        0x00000001
#define SPI_SS_ACT      0x00000001
#define SPI_SS_HIGH     0x00000004
#define SPI_QUAD_EN     0x400000
#define SPI_DIR_2QM     0x100000

#define QSPI_IFR_WIDTH_SINGLE_BIT_SPI   (0 << 0)
#define QSPI_IFR_WIDTH_DUAL_OUTPUT      (1 << 0)
#define QSPI_IFR_WIDTH_QUAD_OUTPUT      (2 << 0)

#define QUAD_READ_0x6B	0x6B
#define QUAD_WRITE_0x32	0x32
#define QUAD_WRITE_0x34	0x34

struct nua3500_qspi {
	void __iomem *regs;
	void __iomem *mem;
	resource_size_t mmap_size;
	struct udevice *dev;
	ulong bus_clk_rate;
	u32 mr;
};

struct nua3500_qspi_mode {
	u8 cmd_buswidth;
	u8 addr_buswidth;
	u8 data_buswidth;
	u32 config;
};

static const struct nua3500_qspi_mode nua3500_qspi_modes[] = {
	{ 1, 1, 1, QSPI_IFR_WIDTH_SINGLE_BIT_SPI },
	{ 1, 1, 2, QSPI_IFR_WIDTH_DUAL_OUTPUT },
	{ 1, 1, 4, QSPI_IFR_WIDTH_QUAD_OUTPUT },
};

static u32 nua3500_qspi_read(struct nua3500_qspi *nq, u32 offset)
{
	u32 value = readl(nq->regs + offset);

	return value;
}

static void nua3500_qspi_write(u32 value, struct nua3500_qspi *nq, u32 offset)
{
	writel(value, nq->regs + offset);
}

static inline bool nua3500_qspi_is_compatible(const struct spi_mem_op *op,
        const struct nua3500_qspi_mode *mode)
{
	if (op->cmd.buswidth != mode->cmd_buswidth)
		return false;

	if (op->addr.nbytes && op->addr.buswidth != mode->addr_buswidth)
		return false;

	if (op->data.nbytes && op->data.buswidth != mode->data_buswidth)
		return false;

	return true;
}

static int nua3500_qspi_find_mode(const struct spi_mem_op *op)
{
	u32 i;

	for (i = 0; i < ARRAY_SIZE(nua3500_qspi_modes); i++)
		if (nua3500_qspi_is_compatible(op, &nua3500_qspi_modes[i]))
			return i;

	return -ENOTSUPP;
}

static bool nua3500_qspi_supports_op(struct spi_slave *slave,
                                     const struct spi_mem_op *op)
{
	if (nua3500_qspi_find_mode(op) < 0)
		return false;

	return true;
}

static int nua3500_qspi_exec_op(struct spi_slave *slave,
                                const struct spi_mem_op *op)
{
	struct nua3500_qspi *nq = dev_get_priv(slave->dev->parent);
	u32 i;
	unsigned char *tx = (unsigned char *)op->data.buf.out;
	unsigned char *rx = op->data.buf.in;

	/* Activate SS */
	nua3500_qspi_write(nua3500_qspi_read(nq, SSCTL) | SELECTSLAVE0, nq, SSCTL);

	/* Send/Receive data */
	nua3500_qspi_write(nua3500_qspi_read(nq, FIFOCTL) | 0x3, nq, FIFOCTL); //TX/RX reset
	while ((nua3500_qspi_read(nq, STATUS) & TXRXRST));

	if (op->cmd.opcode) {
		while ((nua3500_qspi_read(nq, STATUS) & TXFULL)); //TXFULL
		nua3500_qspi_write(op->cmd.opcode, nq, TX);
		//read FIFO to clear dummy return
		while ((nua3500_qspi_read(nq, STATUS) & RXEMPTY)); //RXEMPTY
		nua3500_qspi_read(nq, RX);
	}

	if (op->addr.nbytes) {
		for (i = op->addr.nbytes; i > 0; i--) {
			nua3500_qspi_write(((op->addr.val) >> (8*(i-1))) & (0xFF), nq, TX);
			//read FIFO to clear dummy return
			while ((nua3500_qspi_read(nq, STATUS) & RXEMPTY)); //RXEMPTY
			nua3500_qspi_read(nq, RX);
		}
	}

	if (op->dummy.nbytes) {
		for (i = 0; i < op->dummy.nbytes; i++) {
			nua3500_qspi_write(0, nq, TX);
			//read FIFO to clear dummy return
			while ((nua3500_qspi_read(nq, STATUS) & RXEMPTY)); //RXEMPTY
			nua3500_qspi_read(nq, RX);
		}
	}

	/* Skip to the final steps if there is no data */
	if (op->data.nbytes) {
		if (op->data.dir == SPI_MEM_DATA_OUT) {
			/* Set to Quad mode direction out for Quad write command */
			if ((op->cmd.opcode == QUAD_WRITE_0x32) || (op->cmd.opcode == QUAD_WRITE_0x34)) {
				nua3500_qspi_write(nua3500_qspi_read(nq, CTL) | SPI_QUAD_EN | SPI_DIR_2QM, nq, CTL);
			}

			for (i = 0; i < op->data.nbytes; i++) {
				while ((nua3500_qspi_read(nq, STATUS) & TXFULL)); //TXFULL
				nua3500_qspi_write(*tx++, nq, TX);
			}

		} else {
			/* Set to Quad mode for Quad read command */
			if (op->cmd.opcode == QUAD_READ_0x6B) {
				nua3500_qspi_write(nua3500_qspi_read(nq, CTL) | SPI_QUAD_EN, nq, CTL);
			}

			for (i = 0; i < op->data.nbytes; i++) {
				while ((nua3500_qspi_read(nq, STATUS) & TXFULL)); //TXFULL
				nua3500_qspi_write(0, nq, TX);
				while ((nua3500_qspi_read(nq, STATUS) & RXEMPTY)); //RXEMPTY
				*rx++ = (unsigned char)nua3500_qspi_read(nq, RX);
			}
		}
	}

	while (nua3500_qspi_read(nq, STATUS) & SPI_BUSY);

	/* Restore to 1-bit mode */
	nua3500_qspi_write(nua3500_qspi_read(nq, CTL) & ~(SPI_QUAD_EN | SPI_DIR_2QM), nq, CTL);

	/* Deactiveate SS */
	nua3500_qspi_write(nua3500_qspi_read(nq, SSCTL) & ~SELECTSLAVE0, nq, SSCTL);

	return 0;
}

static int nua3500_qspi_set_speed(struct udevice *bus, uint hz)
{
	struct nua3500_qspi *nq = dev_get_priv(bus);
	unsigned int div;

	div = DIV_ROUND_UP(nq->bus_clk_rate, hz);

	if (div)
		div--;

	if(div == 0)
		div = 1;

	if(div > 0x1FF)
		div = 0x1FF;

	nua3500_qspi_write(div, nq, CLKDIV);

	return 0;

}

static int nua3500_qspi_set_mode(struct udevice *bus, uint mode)
{
	return 0;
}

static int nua3500_qspi_init(struct nua3500_qspi *nq)
{
	/* Initialize data width to 8 bit */
	nua3500_qspi_write((nua3500_qspi_read(nq, CTL) & ~0x1F00) | 0x800, nq, CTL);

	/* Enable the QSPI controller */
	nua3500_qspi_write((nua3500_qspi_read(nq, CTL) | SPIEN), nq, CTL);

	return 0;
}

static int nua3500_qspi_probe(struct udevice *dev)
{
	struct nua3500_qspi *nq = dev_get_priv(dev);
	struct resource res;
	struct clk clk;
	int ret;
	ulong clk_rate;

	/* Map the registers */
	ret = dev_read_resource_byname(dev, "qspi_base", &res);
	if (ret) {
		dev_err(dev, "missing registers\n");
		return ret;
	}

	nq->regs = devm_ioremap(dev, res.start, resource_size(&res));
	if (IS_ERR(nq->regs))
		return PTR_ERR(nq->regs);

	ret = clk_get_by_name(dev, "qspi0", &clk);
	if (ret)
		return ret;

	ret = clk_enable(&clk);
	if (ret)
		return ret;

	clk_rate = clk_get_rate(&clk);
	if (!clk_rate)
		return -EINVAL;

	nq->bus_clk_rate = clk_rate;

	nua3500_qspi_init(nq);

	return 0;
}

static int nua3500_qspi_claim_bus(struct udevice *bus)
{
	return 0;
}

static int nua3500_qspi_release_bus(struct udevice *bus)
{
	return 0;
}

static const struct spi_controller_mem_ops nua3500_qspi_mem_ops = {
	.supports_op = nua3500_qspi_supports_op,
	.exec_op = nua3500_qspi_exec_op,
};

static const struct dm_spi_ops nua3500_qspi_ops = {
	.claim_bus      = nua3500_qspi_claim_bus,
	.release_bus    = nua3500_qspi_release_bus,
	.set_speed = nua3500_qspi_set_speed,
	.set_mode = nua3500_qspi_set_mode,
	.mem_ops = &nua3500_qspi_mem_ops,
};

static const struct udevice_id nua3500_qspi_ids[] = {
	{
		.compatible = "nuvoton,nua3500-qspi",
	},
	{ /* sentinel */ }
};

U_BOOT_DRIVER(nua3500_qspi) = {
	.name           = "nua3500_qspi",
	.id             = UCLASS_SPI,
	.of_match       = nua3500_qspi_ids,
	.ops            = &nua3500_qspi_ops,
	.priv_auto_alloc_size = sizeof(struct nua3500_qspi),
	.probe          = nua3500_qspi_probe,
};
