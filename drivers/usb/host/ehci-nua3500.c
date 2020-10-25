// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2020 Nuvoton Technology Corp.
 *
 */

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <log.h>
#include <malloc.h>
#include <usb.h>
#include <usb/ehci-ci.h>
#include <asm/io.h>

#include <dt-bindings/clock/nua3500-clk.h>
#include <syscon.h>
#include <regmap.h>
#include <nua3500-sys.h>

#include "ehci.h"

#if !CONFIG_IS_ENABLED(DM_USB)
#error "CONFIG_DM_USB must be enabled!!"
#endif

/* Declare global data pointer */
DECLARE_GLOBAL_DATA_PTR;

struct nua3500_ehci_priv {
	struct ehci_ctrl ctrl;
	struct usb_ehci *ehci;
	struct regmap	*sysreg;
	void __iomem	*reg;
	struct clk	clk;
};

static int ehci_nua3500_probe(struct udevice *dev)
{
	struct nua3500_ehci_priv *nua3500_ehci = dev_get_priv(dev);
	struct ofnode_phandle_args args;
	u32		regval;
	fdt_addr_t	hcd_base;
	struct ehci_hccr *hccr;
	struct ehci_hcor *hcor;
	int		ret;

	/*
	 *  enable clock
	 */
	ret = clk_get_by_index(dev, 0, &nua3500_ehci->clk);
	if (ret < 0) {
		dev_err(dev, "%s failed: %d\n", __func__, ret);
		return ret;
	}

	ret = clk_enable(&nua3500_ehci->clk);
	if (ret != 0) {
		dev_err(dev, "%s clk_enable() failed: %d\n", __func__, ret);
		return ret;
	}

	/* get system register map */
	ret = dev_read_phandle_with_args(dev, "nuvoton,sys", NULL,
					 0, 0, &args);
	if (ret) {
		dev_err(dev, "Failed to get syscon: %d\n", ret);
		return ret;
	}

	nua3500_ehci->sysreg = syscon_node_to_regmap(args.node);
	if (IS_ERR(nua3500_ehci->sysreg)) {
		ret = PTR_ERR(nua3500_ehci->sysreg);
		dev_err(dev, "can't get syscon: %d\n", ret);
		return ret;
	}

	/* USBPMISCR; HSUSBH0 & HSUSBH1 PHY */
	regmap_write(nua3500_ehci->sysreg, REG_SYS_USBPMISCR, 0x20002);

	/* set UHOVRCURH(SYS_MISCFCR0[12]) 1 => USBH Host over-current detect is high-active */
	/*                                 0 => USBH Host over-current detect is low-active  */
	regmap_read(nua3500_ehci->sysreg, REG_SYS_MISCFCR0, &regval);
	regmap_write(nua3500_ehci->sysreg, REG_SYS_MISCFCR0, (regval & ~(1 << 12)));
	regmap_write(nua3500_ehci->sysreg, REG_SYS_GPL_MFPH, 0x00990000);

	/*
	 * Get the base address for EHCI controller from the device node
	 */
	hcd_base = devfdt_get_addr(dev);
	if (hcd_base == FDT_ADDR_T_NONE) {
		debug("Can't get the EHCI register base address\n");
		return -ENXIO;
	}

	hccr = (struct ehci_hccr *)hcd_base;
	hcor = (struct ehci_hcor *)
		((u64)hccr + HC_LENGTH(ehci_readl(&hccr->cr_capbase)));

	return ehci_register(dev, hccr, hcor, NULL, 0, USB_INIT_HOST);
}

static const struct udevice_id ehci_nua3500_ids[] = {
	{ .compatible = "nuvoton,nua3500-ehci0" },
	{ .compatible = "nuvoton,nua3500-ehci1" },
	{ }
};

U_BOOT_DRIVER(usb_ehci) = {
	.name	= "nua3500_ehci",
	.id	= UCLASS_USB,
	.of_match = ehci_nua3500_ids,
	.probe = ehci_nua3500_probe,
	.remove = ehci_deregister,
	.ops	= &ehci_usb_ops,
	.platdata_auto_alloc_size = sizeof(struct usb_platdata),
	.priv_auto_alloc_size = sizeof(struct nua3500_ehci_priv),
	.flags	= DM_FLAG_ALLOC_PRIV_DMA,
};
