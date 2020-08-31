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

#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>
#include <command.h>

#include "nua3500_spi.h"

#define REG_APBCLK1     0x40460210
#define REG_APBCLK2     0x40460214
#define REG_MFP_GPD_L	0x40460098

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
                                  unsigned int max_hz, unsigned int mode) {
	struct nua3500_spi_slave  *ns;

	ns = malloc(sizeof(struct nua3500_spi_slave));
	if (!ns)
		return NULL;
	memset(ns,0,sizeof(struct nua3500_spi_slave));

	ns->slave.bus = bus;
	ns->slave.cs = cs;
	ns->max_hz = max_hz;
	ns->mode = mode;
	//ns->slave.quad_enable = 0;
#ifdef CONFIG_NUA3500_SPI_Quad
	ns->slave.mode |= SPI_RX_QUAD;
#endif


	return &ns->slave;
}


void spi_free_slave(struct spi_slave *slave)
{
	struct nua3500_spi_slave *ns = to_nua3500_spi(slave);

	free(ns);

	return;
}

int spi_claim_bus(struct spi_slave *slave)
{
	struct nua3500_spi_slave *ns = to_nua3500_spi(slave);

/* TODO : CWWeng 2020.8.31 : CONFIG_USE_NUA3500_xxx needs to be replaced
 by device tree setting */
#if defined(CONFIG_USE_NUA3500_QSPI0)
	writel(readl(REG_APBCLK1) | (1<<6), REG_APBCLK1);       // QSPI0 clk
#endif

#if defined(CONFIG_USE_NUA3500_QSPI0)
	//QSPI0: D0, D1, D2, D3
	writel(readl(REG_MFP_GPD_L) | 0x00005555, REG_MFP_GPD_L);
#endif

	writel((readl(SPI_CTL) & ~0x1F00) | 0x800, SPI_CTL); //Data Width = 8 bit

	if(ns->mode & SPI_CS_HIGH)
		writel(SPI_SS_HIGH, SPI_SSCTL);
	else
		writel(0, SPI_SSCTL);

	if(ns->mode & SPI_CPOL)
		writel(readl(SPI_CTL) | SELECTPOL, SPI_CTL);
	else
		writel(readl(SPI_CTL) & ~SELECTPOL, SPI_CTL);

	if(ns->mode & SPI_CPHA)
		writel(readl(SPI_CTL) | RXNEG, SPI_CTL);
	else
		writel(readl(SPI_CTL) | TXNEG, SPI_CTL);

	spi_set_speed(slave, ns->max_hz);

	writel(readl(SPI_FIFOCTL) | 0x3, SPI_FIFOCTL); //TX/RX reset
	while ((readl(SPI_STATUS) & TXRXRST));

	writel((readl(SPI_CTL) & ~0xFF)|5, SPI_CTL);

	writel(readl(SPI_CTL) | SPIEN, SPI_CTL);
	while ((readl(SPI_STATUS) & SPIENSTS) == 0);

	return(0);

}


void spi_release_bus(struct spi_slave *slave)
{

#if defined(CONFIG_USE_NUA3500_QSPI0)
	writel(readl(REG_APBCLK1) & ~(1<<6), REG_APBCLK1);       // QSPI0 clk
#endif

#if defined(CONFIG_USE_NUA3500_QSPI0)
	//QSPI0: D0, D1, D2, D3
	writel(readl(REG_MFP_GPD_L) & ~0x00005555, REG_MFP_GPD_L);
#endif

#if defined(CONFIG_USE_NUA3500_QSPI0)
	//SPI0: D4, D5 = D[2], D[3]
	writel(readl(REG_MFP_GPD_L) & ~0x00550000, REG_MFP_GPD_L);
#endif

}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen,
             const void *dout, void *din, unsigned long flags)
{
	unsigned int len;
	unsigned int i;
	static unsigned char spi_cmd = 0;
	unsigned char *tx = (unsigned char *)dout;
	unsigned char *rx = din;


	if(bitlen == 0)
		goto out;

	if(bitlen % 8) {
		/* Errors always terminate an ongoing transfer */
		flags |= SPI_XFER_END;
		goto out;
	}

	len = bitlen / 8;

	if(flags & SPI_XFER_BEGIN) {
		spi_cs_activate(slave);
	}

	// handle quad mode
	//if (flags & SPI_6WIRE) {
	if ((spi_cmd == 0x6c) || (spi_cmd == 0x6b)) { //(flags & SPI_6WIRE) {
		//QSPI0: D4, D5 = D[2], D[3]
		writel((readl(REG_MFP_GPD_L) & ~0x00FF0000) | 0x00550000, REG_MFP_GPD_L);

		writel(readl(SPI_CTL) | SPI_QUAD_EN, SPI_CTL);
		if(rx)
			writel(readl(SPI_CTL) & ~SPI_DIR_2QM, SPI_CTL);
		else
			writel(readl(SPI_CTL) | SPI_DIR_2QM, SPI_CTL);
		//printf("QUAD=>(o)(0x%08x)\n", readl(SPI_CTL));
	} else {
		writel(readl(SPI_CTL) & ~SPI_QUAD_EN, SPI_CTL);
		//printf("QUAD=>(x)(0x%08x)\n", readl(SPI_CTL));
	}

	//Record the command for next spi_xfr() to check if using Quad mode
	if (tx)
		spi_cmd = *tx;
	else
		spi_cmd = 0;

	writel(readl(SPI_FIFOCTL) | 0x3, SPI_FIFOCTL); //TX/RX reset
	while ((readl(SPI_STATUS) & TXRXRST));

	//process non-alignment case
	for (i = 0; i < len; i++) {
		if(tx) {
			while ((readl(SPI_STATUS) & 0x20000)); //TXFULL
			writel(*tx++, SPI_TX);
		}

		if(rx) {
			while ((readl(SPI_STATUS) & 0x20000)); //TXFULL
			writel(0, SPI_TX);
			while ((readl(SPI_STATUS) & 0x100)); //RXEMPTY
			*rx++ = (unsigned char)readl(SPI_RX);
		}
	}

	while (readl(SPI_STATUS) & SPI_BUSY);

out:
	if (flags & SPI_XFER_END) {
		/*
		 * Wait until the transfer is completely done before
		 * we deactivate CS.
		 */
		while (readl(SPI_STATUS) & SPI_BUSY);

		spi_cs_deactivate(slave);
	}

	//if (flags & SPI_6WIRE) {
	if (1) { //(flags & SPI_6WIRE) {
		writel(readl(SPI_CTL) & ~SPI_QUAD_EN, SPI_CTL); //Disable Quad mode
		writel(readl(REG_MFP_GPD_L) & ~(0x00FF0000), REG_MFP_GPD_L);
	}

	return 0;
}

int  spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return(1);
}

void spi_cs_activate(struct spi_slave *slave)
{
	writel(readl(SPI_SSCTL) | SELECTSLAVE0, SPI_SSCTL);
	return;
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	writel(readl(SPI_SSCTL) & ~SELECTSLAVE0, SPI_SSCTL);
	return;
}


void spi_set_speed(struct spi_slave *slave, uint hz)
{
	unsigned int div;

	/* TODO : CWWeng 2020.8.31 : PCLK might change,
	we need to port ccf driver to /drivers/clk directory
	*/
	div = PCLK_CLK / hz;

	if (div)
		div--;

	if(div == 0)
		div = 1;

	if(div > 0x1FF)
		div = 0x1FF;

	writel(div, SPI_CLKDIV);

	return;
}

