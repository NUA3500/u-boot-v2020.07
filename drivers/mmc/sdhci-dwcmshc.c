// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020 Nuvoton Technology Corp.
 */

#include <common.h>
#include <dm.h>
#include <dm/device.h>
#include <linux/io.h>
#include <linux/sizes.h>
#include <malloc.h>
#include <mmc.h>
#include <sdhci.h>

#define SDHCI_DWCMSHC_FMAX 100000000
#define SDHCI_DWCMSHC_FMIN 400000

struct sdhci_dwcmshc_plat {
	struct mmc_config cfg;
	struct mmc mmc;
	void __iomem *ioaddr;
};

static int sdhci_dwcmshc_bind(struct udevice *dev)
{
	struct sdhci_dwcmshc_plat *plat = dev_get_platdata(dev);

	return sdhci_bind(dev, &plat->mmc, &plat->cfg);
}

static int sdhci_dwcmshc_probe(struct udevice *dev)
{
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct sdhci_dwcmshc_plat *plat = dev_get_platdata(dev);
	struct sdhci_host *host = dev_get_priv(dev);
	fdt_addr_t base;
	int ret;

#if 1	
	u64 sys_base;
	sys_base = 0x40460000;
	__raw_writel(__raw_readl(sys_base+0x204)|(1<<16),sys_base+0x204); //Enable SDH0
	__raw_writel(__raw_readl(sys_base+0x218)|(3<<16),sys_base+0x218); //Set SDH0 Clock to System PLL
	#if 0
	/* SDH0 multi-function */
	__raw_writel(0x66666666,sys_base+0x90);
	__raw_writel(0x00006666,sys_base+0x94);
	#endif
#endif

	base = devfdt_get_addr(dev);
	if (base == FDT_ADDR_T_NONE)
		return -EINVAL;
	plat->ioaddr = devm_ioremap(dev, base, SZ_1K);
	if (!plat->ioaddr)
		return -ENOMEM;

	host->name = dev->name;
	host->ioaddr = plat->ioaddr;
	host->quirks = SDHCI_QUIRK_NO_HISPD_BIT | SDHCI_QUIRK_BROKEN_VOLTAGE |
		       SDHCI_QUIRK_32BIT_DMA_ADDR;// | SDHCI_QUIRK_WAIT_SEND_CMD;
	/* MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195 */
	host->voltages = MMC_VDD_165_195;

	host->mmc = &plat->mmc;
	host->mmc->dev = dev;
	ret = sdhci_setup_cfg(&plat->cfg, host, SDHCI_DWCMSHC_FMAX,
			SDHCI_DWCMSHC_FMIN);

	if (ret)
		return ret;
	upriv->mmc = &plat->mmc;
	host->mmc->priv = host;
	return sdhci_probe(dev);
}

static const struct udevice_id sdhci_dwcmshc_match[] = {
	{ .compatible = "snps,dwcmshc-sdhci" },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(sdhci_dwcmshc) = {
	.name		= "sdhci-dwcmshc",
	.id		= UCLASS_MMC,
	.of_match	= sdhci_dwcmshc_match,
	.bind		= sdhci_dwcmshc_bind,
	.probe		= sdhci_dwcmshc_probe,
	.ops		= &sdhci_ops,
	.priv_auto_alloc_size = sizeof(struct sdhci_host),
	.platdata_auto_alloc_size = sizeof(struct sdhci_dwcmshc_plat),
};
