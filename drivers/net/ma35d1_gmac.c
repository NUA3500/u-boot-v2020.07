// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2020 Nuvoton Technology Corp.
 *
 */

#include <common.h>
#include <dm.h>
#include <clk.h>
#include <log.h>
#include <net.h>
#include <phy.h>
#include <syscon.h>
#include <asm/io.h>
#include <dm/pinctrl.h>
#include <linux/bitops.h>
#include <regmap.h>
#include "designware.h"

DECLARE_GLOBAL_DATA_PTR;

#define REG_SYS_GMAC0MISCR	0x108
#define REG_SYS_GMAC1MISCR	0x10C
#define REG_SYS_MACAD0LSR	0x110
#define REG_SYS_MACAD0HSR	0x114
#define REG_SYS_MACAD1LSR	0x118
#define REG_SYS_MACAD1HSR	0x11C

#define TXDLY_OFST	16
#define TXDLY_MSK	(0xF << TXDLY_OFST)
#define RXDLY_OFST	20
#define RXDLY_MSK	(0xF << RXDLY_OFST)
/*
 * Platform data for the gmac
 *
 * dw_eth_pdata: Required platform data for designware driver (must be first)
 */
struct gmac_ma35d1_platdata {
	struct dw_eth_pdata dw_eth_pdata;
	struct regmap *regmap;
	int id;
	int phy_mode;
	int tx_delay;
	int rx_delay;
};

static int gmac_ma35d1_ofdata_to_platdata(struct udevice *dev)
{
	struct gmac_ma35d1_platdata *pdata = dev_get_platdata(dev);

	pdata->id = dev_read_u32_default(dev, "mac-id", 0);
	pdata->tx_delay = dev_read_u32_default(dev, "tx_delay", -ENOENT);
	pdata->rx_delay = dev_read_u32_default(dev, "rx_delay", -ENOENT);

	if (pdata->tx_delay == -ENOENT)
		pdata->tx_delay = 0;
	if (pdata->rx_delay == -ENOENT)
		pdata->rx_delay = 0;

	return designware_eth_ofdata_to_platdata(dev);
}

static int gmac_ma35d1_probe(struct udevice *dev)
{
	struct gmac_ma35d1_platdata *pdata = dev_get_platdata(dev);
	struct dw_eth_pdata *dw_pdata = dev_get_platdata(dev);
	struct eth_pdata *eth_pdata = &dw_pdata->eth_pdata;
	struct ofnode_phandle_args args;
	u32 reg;
	struct clk clk;
	int ret;

	ret = dev_read_phandle_with_args(dev, "nuvoton,ma35d1-sys", NULL,
					 0, 0, &args);
	if (ret) {
		dev_err(dev, "Failed to get syscon: %d\n", ret);
		return ret;
	}

	pdata->regmap = syscon_node_to_regmap(args.node);
	if (IS_ERR(pdata->regmap)) {
		ret = PTR_ERR(pdata->regmap);
		dev_err(dev, "can't get syscon: %d\n", ret);
		return ret;
	}

	regmap_read(pdata->regmap,
		    pdata->id == 0 ? REG_SYS_GMAC0MISCR : REG_SYS_GMAC1MISCR,
		    &reg);

	if (eth_pdata->phy_interface == PHY_INTERFACE_MODE_RMII) {
		reg |= 1;
	} else {
		reg &= ~1;
	}
	reg &= ~(TXDLY_MSK | RXDLY_MSK);
	reg |= (pdata->tx_delay << TXDLY_OFST) | (pdata->rx_delay << RXDLY_OFST);
	regmap_write(pdata->regmap,
		     pdata->id == 0 ? REG_SYS_GMAC0MISCR : REG_SYS_GMAC1MISCR,
		     reg);

	ret = clk_set_defaults(dev, 0);
	if (ret)
		dev_err(dev, "clk_set_defaults failed %d\n", ret);

	ret = clk_get_by_index(dev, 0, &clk);
	if (ret)
		return ret;

	switch (eth_pdata->phy_interface) {
	case PHY_INTERFACE_MODE_RGMII:
		pdata->phy_mode = PHY_INTERFACE_MODE_RGMII;
		break;
	case PHY_INTERFACE_MODE_RGMII_ID:
		pdata->phy_mode = PHY_INTERFACE_MODE_RGMII_ID;
		break;
	case PHY_INTERFACE_MODE_RGMII_TXID:
		pdata->phy_mode = PHY_INTERFACE_MODE_RGMII_TXID;
		break;
	case PHY_INTERFACE_MODE_RGMII_RXID:
		pdata->phy_mode = PHY_INTERFACE_MODE_RGMII_RXID;
		break;
	case PHY_INTERFACE_MODE_RMII:
		pdata->phy_mode = PHY_INTERFACE_MODE_RMII;
		break;

	default:
		dev_err(dev, "No interface defined!\n");
		return -ENXIO;
	}

	return designware_eth_probe(dev);
}

static int ma35d1_read_rom_hwaddr(struct udevice *dev)
{
	struct gmac_ma35d1_platdata *pdata = dev_get_platdata(dev);
	struct eth_pdata *eth_pdata = dev_get_platdata(dev);
	u32 reg;

	regmap_read(pdata->regmap,
		    pdata->id == 0 ? REG_SYS_MACAD0LSR : REG_SYS_MACAD1LSR,
		    &reg);
	eth_pdata->enetaddr[5] = reg & 0xFF;
	eth_pdata->enetaddr[4] = (reg >> 8) & 0xFF;
	eth_pdata->enetaddr[3] = (reg >> 16) & 0xFF;
	eth_pdata->enetaddr[2] = (reg >> 24) & 0xFF;
	regmap_read(pdata->regmap,
		    pdata->id == 0 ? REG_SYS_MACAD0HSR : REG_SYS_MACAD1LSR,
		    &reg);
	eth_pdata->enetaddr[1] = reg & 0xFF;
	eth_pdata->enetaddr[0] = (reg >> 8) & 0xFF;

	return !is_valid_ethaddr(eth_pdata->enetaddr);
}

const struct eth_ops gmac_ma35d1_eth_ops = {
	.start			= designware_eth_start,
	.send			= designware_eth_send,
	.recv			= designware_eth_recv,
	.free_pkt		= designware_eth_free_pkt,
	.stop			= designware_eth_stop,
	.write_hwaddr		= designware_eth_write_hwaddr,
	.read_rom_hwaddr	= ma35d1_read_rom_hwaddr,
};

static const struct udevice_id ma35d1_gmac_ids[] = {
	{ .compatible = "nuvoton,ma35d1-gmac" },
	{ }
};

U_BOOT_DRIVER(eth_gmac_ma35d1) = {
	.name	= "gmac_ma35d1",
	.id	= UCLASS_ETH,
	.of_match = ma35d1_gmac_ids,
	.ofdata_to_platdata = gmac_ma35d1_ofdata_to_platdata,
	.probe	= gmac_ma35d1_probe,
	.ops	= &gmac_ma35d1_eth_ops,
	.priv_auto_alloc_size = sizeof(struct dw_eth_dev),
	.platdata_auto_alloc_size = sizeof(struct gmac_ma35d1_platdata),
	.flags = DM_FLAG_ALLOC_PRIV_DMA,
};
