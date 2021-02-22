/*
 * Copyright (c) 2020 Nuvoton technology corporation.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <asm/io.h>
#include <dm/device_compat.h>
#include <linux/bitops.h>
#include <linux/iopoll.h>
#include <i2c.h>

#include <dm/device.h>

/*-----------------------------------------*/
/* global interface variables declarations */
/*-----------------------------------------*/
#define I2C_CTL_STA_SI            0x28UL /*!< I2C_CTL setting for I2C control bits. It would set STA and SI bits     */
#define I2C_CTL_STA_SI_AA         0x2CUL /*!< I2C_CTL setting for I2C control bits. It would set STA, SI and AA bits */
#define I2C_CTL_STO_SI            0x18UL /*!< I2C_CTL setting for I2C control bits. It would set STO and SI bits     */
#define I2C_CTL_STO_SI_AA         0x1CUL /*!< I2C_CTL setting for I2C control bits. It would set STO, SI and AA bits */
#define I2C_CTL_SI                0x08UL /*!< I2C_CTL setting for I2C control bits. It would set SI bit              */
#define I2C_CTL_SI_AA             0x0CUL /*!< I2C_CTL setting for I2C control bits. It would set SI and AA bits      */
#define I2C_CTL_STA               0x20UL /*!< I2C_CTL setting for I2C control bits. It would set STA bit             */
#define I2C_CTL_STO               0x10UL /*!< I2C_CTL setting for I2C control bits. It would set STO bit             */
#define I2C_CTL_AA                0x04UL /*!< I2C_CTL setting for I2C control bits. It would set AA bit              */
#define I2C_CTL_ALL               0x3CUL /*!< I2C_CTL setting for I2C control bits. It would set ALL bit             */

#define I2C_CTL_ENABLE            0x40UL /*!< I2C_CTL setting for I2C control bits. It would set I2C Enable bit      */

#define I2C_MAX_TX_BUF  100
#define I2C_MAX_RX_BUF  100

/* MA35D1 I2C registers */
struct ma35d1_i2c_regs{
	volatile u32 CTL0;	// 0x00
	volatile u32 ADDR0;      // 0x04
	volatile u32 DAT;        // 0x08
	volatile u32 STATUS0;    // 0x0C
	volatile u32 CLKDIV;     // 0x10
	volatile u32 TOCTL;      // 0x14
	volatile u32 ADDR1;      // 0x18
	volatile u32 ADDR2;      // 0x1C
	volatile u32 ADDR3;      // 0x20
	volatile u32 ADDRMSK0;   // 0x24
	volatile u32 ADDRMSK1;   // 0x28
	volatile u32 ADDRMSK2;   // 0x2C
	volatile u32 ADDRMSK3;   // 0x30
	volatile u32 WKCTL;      // 0x3C
	volatile u32 WKSTS;      // 0x40
	volatile u32 CTL1;       // 0x44
	volatile u32 STATUS1;    // 0x48
	volatile u32 TMCTL;      // 0x4C
	volatile u32 BUSCTL;     // 0x50
	volatile u32 BUSTCTL;    // 0x54
	volatile u32 BUSSTS;     // 0x58
};

struct ma35d1_i2c_priv{
	struct ma35d1_i2c_regs *regs;
	struct clk clk;
};

typedef struct{
	uint8_t g_u8DeviceAddr;
	uint8_t g_au8TxData[I2C_MAX_TX_BUF];
	uint8_t g_u8RxData[I2C_MAX_RX_BUF];
	uint8_t g_u8DataLen;
	uint8_t g_u8EndFlag;
	uint32_t g_u32TransferLength;
	uint32_t g_u32AddressLength;
	uint32_t g_u32RxDataCount;
}i2c_dev;

static void _i2cReset(i2c_dev *dev)
{
	dev->g_u8DeviceAddr = -1;
	dev->g_u8DataLen = 0;
	dev->g_u8EndFlag = 0;
	dev->g_u32AddressLength = 0;
}

void I2C_MasterRx(struct ma35d1_i2c_regs *regs, i2c_dev *dev, uint32_t u32Status)
{
	if (u32Status == 0x08) {                    /* START has been transmitted and prepare SLA+W */
		regs->DAT = (dev->g_u8DeviceAddr << 1); /* Write SLA+W to Register I2CDAT */
		regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_SI_AA;
	} else if (u32Status == 0x18) {             /* SLA+W has been transmitted and ACK has been received */
		regs->DAT = dev->g_au8TxData[dev->g_u8DataLen++];
		regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_SI_AA;
	} else if (u32Status == 0x20) {             /* SLA+W has been transmitted and NACK has been received */
		regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | (I2C_CTL_STO | I2C_CTL_SI);
	} else if (u32Status == 0x28) {             /* DATA has been transmitted and ACK has been received */
		if (dev->g_u8DataLen < dev->g_u32AddressLength) {
			regs->DAT = dev->g_au8TxData[dev->g_u8DataLen++];
			regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_SI_AA;
		} else {
			regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | (I2C_CTL_STA | I2C_CTL_SI);
		}
	} else if (u32Status == 0x10) {             /* Repeat START has been transmitted and prepare SLA+R */
		regs->DAT = (dev->g_u8DeviceAddr << 1) | (0x01); /* Write SLA+R to Register I2CDAT */
		regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_SI_AA;
	} else if (u32Status == 0x40) {             /* SLA+R has been transmitted and ACK has been received */
		regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_SI_AA;
	} else if (u32Status == 0x48) {             /* Slave Address NACK */
		regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_STO_SI; /* Clear SI and send STOP */
	} else if (u32Status == 0x50) {            /* DATA has been received and ACK has been returned */
		dev->g_u8RxData[dev->g_u32RxDataCount++] = regs->DAT;
		if(dev->g_u32RxDataCount < (dev->g_u32TransferLength - 1u)) {
			regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_SI_AA;
		} else {
			regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_SI;
		}
	}else if (u32Status == 0x58) {             /* DATA has been received and NACK has been returned */
		dev->g_u8RxData[dev->g_u32RxDataCount++] = regs->DAT;
		regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | (I2C_CTL_STO | I2C_CTL_SI);
		dev->g_u8EndFlag = 1;
	} else {
		debug("%s: Status 0x%x is NOT processed ", __func__, u32Status);
	}
}

void I2C_MasterTx(struct ma35d1_i2c_regs *regs, i2c_dev *dev, uint32_t u32Status)
{
	if (u32Status == 0x08) {                    /* START has been transmitted */
		regs->DAT = (dev->g_u8DeviceAddr << 1); /* Write SLA+W to Register I2CDAT */
		regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_SI_AA;
	} else if (u32Status == 0x18) {             /* SLA+W has been transmitted and ACK has been received */
		regs->DAT = dev->g_au8TxData[dev->g_u8DataLen++];
		regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_SI_AA;
	} else if (u32Status == 0x20) {             /* SLA+W has been transmitted and NACK has been received */
		regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | (I2C_CTL_STA | I2C_CTL_STO | I2C_CTL_SI_AA);
	} else if (u32Status == 0x28) {             /* DATA has been transmitted and ACK has been received */
		if (dev->g_u8DataLen != (dev->g_u32TransferLength + dev->g_u32AddressLength)) {
			regs->DAT = dev->g_au8TxData[dev->g_u8DataLen++];
			regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_SI_AA;
		} else {
			regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | (I2C_CTL_STO | I2C_CTL_SI);
			dev->g_u8EndFlag = 1;
		}
	} else {
		debug("%s: Status 0x%x is NOT processed\n", __func__, u32Status);
	}
}

static int  ma35d1_i2c_read(struct udevice *dev, uchar chip, u8 *addr,int alen, uchar *buffer, int len)
{
 	struct ma35d1_i2c_priv *priv = dev_get_priv(dev);
	struct ma35d1_i2c_regs *regs = priv->regs;
	int i;
	uint32_t u32Status;
	uint32_t u32time_out;
	uint32_t I2C_TIME_OUT_COUNT = 0x200000;
	i2c_dev i2c_device;

	memset((void *)&i2c_device, 0, sizeof(i2c_dev));
	_i2cReset(&i2c_device);

	u32time_out = 0;
	i2c_device.g_u8DataLen = 0;
	i2c_device.g_u8DeviceAddr = chip;
	i2c_device.g_u32TransferLength = len;
	i2c_device.g_u32RxDataCount = 0;

	if(len > I2C_MAX_RX_BUF) {
		debug("%s: Rx max. length is %d ", __func__, I2C_MAX_RX_BUF);
		return 1;
	}

	i2c_device.g_u8EndFlag = 0;
	i2c_device.g_u32AddressLength = alen; // address length

	for(i = 0; i < alen; i++)
		i2c_device.g_au8TxData[i] = (uint8_t)(addr[alen - i]);

	regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_STA;

	while(1) {
		if(regs->CTL0 & I2C_CTL_SI) {
			u32time_out = 0;
			u32Status = regs->STATUS0;
			I2C_MasterRx(regs, &i2c_device, u32Status);
		}

		if(i2c_device.g_u8EndFlag) {
			for(i = 0 ; i < len; i++) {
				buffer[i] = i2c_device.g_u8RxData[i];
			}
			break;
		}

		u32time_out++;
		if(u32time_out > I2C_TIME_OUT_COUNT) {
			debug("%s: i2c Read Time Out!", __func__);
			return 1; // error
		}
	}

	return(0);
}

static int  ma35d1_i2c_write(struct udevice *dev, uchar chip, u8 *addr,int alen, uchar *buffer, int len)
{
	struct ma35d1_i2c_priv *priv = dev_get_priv(dev);
	struct ma35d1_i2c_regs *regs = priv->regs;
	int i;
	uint32_t u32Status;
	uint32_t u32time_out;
	uint32_t I2C_TIME_OUT_COUNT = 0x20000;
	i2c_dev i2c_device;

	memset((void *)&i2c_device, 0, sizeof(i2c_dev));
	_i2cReset(&i2c_device);

	i2c_device.g_u8DeviceAddr = chip;
	i2c_device.g_u32TransferLength = len;
	i2c_device.g_u8DataLen = 0;
	u32time_out = 0;
	i2c_device.g_u32AddressLength = alen; // address length

	if(len > (I2C_MAX_TX_BUF - alen)) {
		debug("%s: Tx max. length is %d ", __func__, (I2C_MAX_TX_BUF -2));
		return 1;
	}

	for(i = 0; i < alen; i++)
		i2c_device.g_au8TxData[i] = (uint8_t)(addr[alen - i]);

	for(i = alen; i < alen+len; i++)
		i2c_device.g_au8TxData[i] = (uint8_t)(buffer[i]);

	i2c_device.g_u8EndFlag = 0;

	regs->CTL0 = (regs->CTL0 &~ I2C_CTL_ALL) | I2C_CTL_STA;

	while(1) {
		if(regs->CTL0 & I2C_CTL_SI) {
			u32time_out = 0;
			u32Status = regs->STATUS0;
			I2C_MasterTx(regs, &i2c_device, u32Status);
		}

		if(i2c_device.g_u8EndFlag) {
			break;
		}
		u32time_out++;
		if(u32time_out > I2C_TIME_OUT_COUNT) {
			debug("%s: i2c Write Time Out!", __func__);
			return 1; // error
		}
	}

	return(0);
}

static int ma35d1_i2c_set_bus_speed(struct udevice *dev, unsigned int sp)
{
	struct ma35d1_i2c_priv *priv = dev_get_priv(dev);
	struct ma35d1_i2c_regs *regs = priv->regs;
	ulong clk_rate;
	uint32_t u32Div;

	regs->CTL0 &= ~I2C_CTL_ENABLE; // Disable I2C

	clk_rate = clk_get_rate(&priv->clk);
	if (IS_ERR_VALUE(clk_rate))
		return -EINVAL;

	/* assume speed above 1000 are Hz-specified */
	if(sp > 1000) sp = sp/1000;
	if(sp > 400) sp = 400;

	u32Div = (uint32_t)(((clk_rate * 10U) / (sp * 4U) + 5U) / 10U - 1U); /* Compute proper divider for I2C clock */

	regs->CLKDIV = u32Div;

	regs->CTL0 |= I2C_CTL_ENABLE; // Enable I2C

	return 0;
}

static int ma35d1_i2c_xfer(struct udevice *dev, struct i2c_msg *msg, int nmsgs)
{
	struct i2c_msg *dmsg, *omsg, dummy;

	memset(&dummy, 0, sizeof(struct i2c_msg));

	/*
	 * We expect either two messages (one with an offset and one with the
	 * actual data) or one message (just data or offset/data combined)
	 */
	if (nmsgs > 2 || nmsgs == 0) {
		debug("%s: Only one or two messages are supported.", __func__);
		return -1;
	}

	omsg = nmsgs == 1 ? &dummy : msg;
	dmsg = nmsgs == 1 ? msg : msg + 1;

	if (dmsg->flags & I2C_M_RD)
		return ma35d1_i2c_read(dev, dmsg->addr, omsg->buf,
				  omsg->len, dmsg->buf, dmsg->len);
	else
		return ma35d1_i2c_write(dev, dmsg->addr, omsg->buf,
				   omsg->len, dmsg->buf, dmsg->len);

	return 0;
}

static int ma35d1_i2c_probe(struct udevice *dev)
{
	struct ma35d1_i2c_priv *priv = dev_get_priv(dev);
	struct ma35d1_i2c_regs *regs;
	fdt_addr_t addr;
	int ret = 0;

	addr = devfdt_get_addr(dev);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;

	priv->regs = (struct ma35d1_i2c_regs *)addr;
	regs = priv->regs;

	ret = clk_get_by_index(dev, 0, &priv->clk);
	if (ret)
		return ret;

	ret = clk_enable(&priv->clk);
	if (ret)
	{
		printf("\n i2c clk enable fail \n");
		clk_free(&priv->clk);
	}

	regs->CTL0 |= I2C_CTL_ENABLE; // Enable I2C

	return ret;
}

static const struct dm_i2c_ops ma35d1_i2c_ops = {
	.xfer = ma35d1_i2c_xfer,
	.set_bus_speed = ma35d1_i2c_set_bus_speed,
};

static const struct udevice_id ma35d1_i2c_ids[] = {
	{ .compatible = "nuvoton,ma35d1-i2c" },
	{}
};

U_BOOT_DRIVER(nuvoton_i2c) = {
	.name = "nuvoton-i2c",
	.id = UCLASS_I2C,
	.of_match = ma35d1_i2c_ids,
	.probe = ma35d1_i2c_probe,
	.priv_auto_alloc_size = sizeof(struct ma35d1_i2c_priv),
	.ops = &ma35d1_i2c_ops,
};


