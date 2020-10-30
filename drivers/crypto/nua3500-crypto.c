// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2020 Nuvoton Technology Corp.
 *
 */

#include <common.h>
#include <dm.h>
#include <log.h>
#include <malloc.h>
#include <asm/io.h>

#include <dt-bindings/clock/nua3500-clk.h>
#include <syscon.h>
#include <regmap.h>
#include <nua3500-sys.h>

#include "nua3500-crypto.h"

struct nua3500_crypto_priv {
	struct regmap	*sysreg;
	void    *reg_base;
};

static struct nua3500_crypto_priv  _nua3500_crypto;

static inline void nu_write_reg(u32 val, u32 off)
{
	writel(val, _nua3500_crypto.reg_base + off);
}

static inline uint32_t nu_read_reg(u32 off)
{
	return readl(_nua3500_crypto.reg_base + off);
}

/**
 * Nuvton AES hardware accelerator decrypt AES-256 CFB mode encrypted image with
 * AES key from Key Store OTP.
 *
 * @keynum		The Key Store OTP key number that stored the AES key.
 * @src_addr		Source address of cypher text
 * @dst_addr		Destination address of plain text output
 * @data_len		Number bytes to decrypt
 */
int nua3500_aes_decrypt(u32 keynum, u32 src_addr,
			u32 dst_addr, u32 data_len)
{
	u32	ctrl;
	int	i;

	/*------------------------------------------------*/
	/*  Init Key Store                                */
	/*------------------------------------------------*/
	/* Start Key Store Initial */
	writel(KS_CTL_INIT | KS_CTL_START, KS_CTL);

	/* Waiting for Key Store inited */
	while ((readl(KS_STS) & KS_STS_INITDONE) == 0)
		;

	/* Waiting for Key Store ready */
	while (readl(KS_STS) & KS_STS_BUSY)
		;

	/*------------------------------------------------*/
	/*  Start AES-256 CFB mode decode                 */
	/*------------------------------------------------*/
	nu_write_reg(0, AES_CTL);
	nu_write_reg((INTEN_AESIEN | INTEN_AESEIEN), INTEN);
	nu_write_reg((INTSTS_AESIF | INTSTS_AESEIF), INTSTS);

	nu_write_reg(((0x2 << AES_KSCTL_RSSRC_OFFSET) |
			AES_KSCTL_RSRC | keynum), AES_KSCTL);

	/* program AES IV */
	for (i = 0; i < 4; i++)
		nu_write_reg(0, AES_IV(i));

	ctrl = (AES_KEYSZ_SEL_256 | AES_MODE_CFB | AES_CTL_INSWAP |
		AES_CTL_OUTSWAP | AES_CTL_DMAEN);

	nu_write_reg(data_len, AES_CNT);
	nu_write_reg(src_addr, AES_SADDR);
	nu_write_reg(dst_addr, AES_DADDR);

	/* start AES */
	nu_write_reg(ctrl | AES_CTL_START, AES_CTL);

	while ((nu_read_reg(INTSTS) & (INTSTS_AESIF | INTSTS_AESEIF)) == 0)
		;
	nu_write_reg((INTSTS_AESIF | INTSTS_AESEIF), INTSTS);

	return 0;
}

static int nua3500_crypto_bind(struct udevice *dev)
{
	/*
	 * Get the base address for Crypto from the device node
	 */
	_nua3500_crypto.reg_base = (void *)devfdt_get_addr(dev);
	if (_nua3500_crypto.reg_base == (void *)FDT_ADDR_T_NONE) {
		printf("Can't get the CRYPTO register base address\n");
		return -ENXIO;
	}
	return 0;
}

static const struct udevice_id nua3500_crypto_ids[] = {
	{ .compatible = "nuvoton,nua3500-crypto" },
	{ }
};

U_BOOT_DRIVER(crypto_nua3500) = {
	.name	= "nua3500_crypto",
	.id	= UCLASS_NOP,
	.of_match = nua3500_crypto_ids,
	.bind = nua3500_crypto_bind,
	.priv_auto_alloc_size = sizeof(struct nua3500_crypto_priv),
	.flags	= DM_FLAG_ALLOC_PRIV_DMA,
};
