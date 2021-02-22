/* SPDX-License-Identifier: GPL-2.0+  */
/*
 * (C) Copyright 2020 Nuvoton Technology Corp.
 *
 */

#ifndef MA35D1_CRYPTO_H
#define MA35D1_CRYPTO_H

#define CRYPTO_BASE		(0x40300000UL)
#define KS_BASE			(0x40340800UL)

#define INTEN			(0x000)
#define INTEN_AESIEN			(0x1 << 0)
#define INTEN_AESEIEN			(0x1 << 1)
#define INTEN_PRNGIEN			(0x1 << 16)
#define INTEN_ECCIEN			(0x1 << 22)
#define INTEN_ECCEIEN			(0x1 << 23)
#define INTEN_HMACIEN			(0x1 << 24)
#define INTEN_HMACEIEN			(0x1 << 25)
#define INTSTS			(0x004)
#define INTSTS_AESIF			(0x1 << 0)
#define INTSTS_AESEIF			(0x1 << 1)
#define INTSTS_PRNGIF			(0x1 << 16)
#define INTSTS_ECCIF			(0x1 << 22)
#define INTSTS_ECCEIF			(0x1 << 23)
#define INTSTS_HMACIF			(0x1 << 24)
#define INTSTS_HMACEIF			(0x1 << 25)

#define PRNG_CTL		(0x008)
#define PRNG_CTL_START			(0x1 << 0)
#define PRNG_CTL_SEEDRLD		(0x1 << 1)
#define PRNG_CTL_KEYSZ_OFFSET		(2)
#define PRNG_CTL_KEYSZ_MASK		(0xf << 2)
#define PRNG_CTL_BUSY			(0x1 << 8)
#define PRNG_CTL_SEEDSRC		(0x1 << 16)
#define PRNG_SEED		(0x00C)
#define PRNG_KEY(x)		(0x010 + ((x) * 0x04))

#define AES_FDBCK(x)		(0x050 + ((x) * 0x04))
#define AES_GCM_IVCNT(x)	(0x080 + ((x) * 0x04))
#define AES_GCM_ACNT(x)		(0x088 + ((x) * 0x04))
#define AES_GCM_PCNT(x)		(0x090 + ((x) * 0x04))
#define AES_FBADDR		(0x0A0)
#define AES_CTL			(0x100)
#define AES_CTL_START			(0x1 << 0)
#define AES_CTL_STOP			(0x1 << 1)
#define AES_CTL_KEYSZ_OFFSET		2
#define AES_CTL_KEYSZ_MASK		(0x3 << 2)
#define AES_CTL_DMALAST			(0x1 << 5)
#define AES_CTL_DMACSCAD		(0x1 << 6)
#define AES_CTL_DMAEN			(0x1 << 7)
#define AES_CTL_OPMODE_OFFSET		8
#define AES_CTL_OPMODE_MASK		(0xff << 8)
#define AES_CTL_ENCRPT			(0x1 << 16)
#define AES_CTL_SM4EN			(0x1 << 17)
#define AES_CTL_FBIN			(0x1 << 20)
#define AES_CTL_FBOUT			(0x1 << 21)
#define AES_CTL_OUTSWAP			(0x1 << 22)
#define AES_CTL_INSWAP			(0x1 << 23)
#define AES_CTL_KOUTSWAP		(0x1 << 24)
#define AES_CTL_KINSWAP			(0x1 << 25)
#define AES_STS			(0x104)
#define AES_STS_BUSY			(0x1 << 0)
#define AES_STS_INBUFEMPTY		(0x1 << 8)
#define AES_STS_INBUFFULL		(0x1 << 9)
#define AES_STS_INBUFERR		(0x1 << 10)
#define AES_STS_CNTERR			(0x1 << 12)
#define AES_STS_OUTBUFEMPTY		(0x1 << 16)
#define AES_STS_OUTBUFFULL		(0x1 << 17)
#define AES_STS_OUTBUFERR		(0x1 << 18)
#define AES_STS_BUSERR			(0x1 << 20)
#define AES_STS_KSERR			(0x1 << 21)
#define AES_DATIN		(0x108)
#define AES_DATOUT		(0x10C)
#define AES_KEY(x)		(0x110 + ((x) * 0x04))
#define AES_IV(x)		(0x130 + ((x) * 0x04))
#define AES_SADDR		(0x140)
#define AES_DADDR		(0x144)
#define AES_CNT			(0x148)

#define HMAC_CTL		(0x300)
#define HMAC_CTL_START			(0x1 << 0)
#define HMAC_CTL_STOP			(0x1 << 1)
#define HMAC_CTL_DMAFIRST		(0x1 << 4)
#define HMAC_CTL_DMALAST		(0x1 << 5)
#define HMAC_CTL_DMACSCAD		(0x1 << 6)
#define HMAC_CTL_DMAEN			(0x1 << 7)
#define HMAC_CTL_OPMODE_OFFSET		8
#define HMAC_CTL_OPMODE_MASK		(0x7 << 8)
#define HMAC_CTL_HMACEN			(0x1 << 11)
#define HMAC_CTL_SHA3EN			(0x1 << 12)
#define HMAC_CTL_SM3EN			(0x1 << 13)
#define HMAC_CTL_MD5EN			(0x1 << 14)
#define HMAC_CTL_FBIN			(0x1 << 20)
#define HMAC_CTL_FBOUT			(0x1 << 21)
#define HMAC_CTL_OUTSWAP		(0x1 << 22)
#define HMAC_CTL_INSWAP			(0x1 << 23)
#define HMAC_CTL_NEXTDGST		(0x1 << 24)
#define HMAC_CTL_FINISHDGST		(0x1 << 25)
#define HMAC_STS		(0x304)
#define HMAC_STS_BUSY			(0x1 << 0)
#define HMAC_STS_DMABUSY		(0x1 << 1)
#define HMAC_STS_SHAKEBUSY		(0x1 << 2)
#define HMAC_STS_DMAERR			(0x1 << 8)
#define HMAC_STS_KSERR			(0x1 << 9)
#define HMAC_STS_DATINREQ		(0x1 << 16)
#define HMAC_DGST(x)		(0x308 + ((x) * 0x04))
#define HMAC_KEYCNT		(0x348)
#define HMAC_SADDR		(0x34C)
#define HMAC_DMACNT		(0x350)
#define HMAC_DATIN		(0x354)
#define HMAC_FDBCK(x)		(0x358 + ((x) * 0x04))
#define HMAC_FDBCK_WCNT			88
#define HMAC_FBADDR		(0x4FC)
#define HMAC_SHAKEDGST(x)	(0x500 + ((x) * 0x04))
#define HMAC_SHAKEDGST_WCNT		42

#define ECC_CTL			(0x800)
#define ECC_CTL_START			(0x1 << 0)
#define ECC_CTL_STOP			(0x1 << 1)
#define ECC_CTL_ECDSAS			(0x1 << 4)
#define ECC_CTL_ECDSAR			(0x1 << 5)
#define ECC_CTL_DMAEN			(0x1 << 7)
#define ECC_CTL_FSEL			(0x1 << 8)
#define ECC_CTL_ECCOP_OFFSET		9
#define ECC_CTL_ECCOP_MASK		(0x3 << 9)
#define ECC_CTL_MODOP_OFFSET		11
#define ECC_CTL_MODOP_MASK		(0x3 << 9)
#define ECC_CTL_CSEL			(0x1 << 13)
#define ECC_CTL_SCAP			(0x1 << 14)
#define ECC_CTL_LDAP1			(0x1 << 16)
#define ECC_CTL_LDAP2			(0x1 << 17)
#define ECC_CTL_LDA			(0x1 << 18)
#define ECC_CTL_LDB			(0x1 << 19)
#define ECC_CTL_LDN			(0x1 << 20)
#define ECC_CTL_LDK			(0x1 << 21)
#define ECC_CTL_CURVEM_OFFSET		22
#define ECC_CTL_CURVEM_MASK		(0x3ff << 22)
#define ECC_STS			(0x804)
#define ECC_STS_BUSY			(0x1 << 0)
#define ECC_STS_DMABUSY			(0x1 << 1)
#define ECC_STS_BUSERR			(0x1 << 16)
#define ECC_STS_KSERR			(0x1 << 17)
#define ECC_X1(x)		(0x808 + ((x) * 0x04))
#define ECC_Y1(x)		(0x850 + ((x) * 0x04))
#define ECC_X2(x)		(0x898 + ((x) * 0x04))
#define ECC_Y2(x)		(0x8E0 + ((x) * 0x04))
#define ECC_A(x)		(0x928 + ((x) * 0x04))
#define ECC_B(x)		(0x970 + ((x) * 0x04))
#define ECC_N(x)		(0x9B8 + ((x) * 0x04))
#define ECC_K(x)		(0xA00 + ((x) * 0x04))
#define ECC_KEY_WCNT			18
#define ECC_SADDR		(0xA48)
#define ECC_DADDR		(0xA4C)
#define ECC_STARTREG		(0xA50)
#define ECC_WORDCNT		(0xA54)

#define AES_KSCTL		(0xF10)
#define AES_KSCTL_NUM_OFFSET		0
#define AES_KSCTL_NUM_MASK		(0x1f << 0)
#define AES_KSCTL_RSRC			(0x1 << 5)
#define AES_KSCTL_RSSRC_OFFSET		6
#define AES_KSCTL_RSSRC_MASK		(0x3 << 6)

#define ECC_KSCTL		(0xF40)
#define ECC_KSCTL_NUMK_OFFSET		0
#define ECC_KSCTL_NUMK_MASK		(0x1f << 0)
#define ECC_KSCTL_RSRCK			(0x1 << 5)
#define ECC_KSCTL_RSSRCK_OFFSET		6
#define ECC_KSCTL_RSSRCK_MASK		(0x3 << 6)
#define ECC_KSCTL_TRUST			(0x1 << 16)
#define ECC_KSCTL_PRIV			(0x1 << 18)
#define ECC_KSCTL_XY			(0x1 << 20)
#define ECC_KSCTL_WDST			(0x1 << 21)
#define ECC_KSCTL_WSDST_OFFSET		22
#define ECC_KSCTL_WSDST_MASK		(0x3 << 22)
#define ECC_KSCTL_OWNER_OFFSET		24
#define ECC_KSCTL_OWNER_MASK		(0x7 << 24)
#define ECC_KSSTS		0xF44
#define ECC_KSSTS_NUM_OFFSET		0
#define ECC_KSSTS_NUM_MASK		(0x1f << 0)
#define ECC_KSXY			0xF48
#define ECC_KSXY_NUMX_OFFSET		0
#define ECC_KSXY_NUMX_MASK		(0x1f << 0)
#define ECC_KSXY_RSRCXY			(0x1 << 5)
#define ECC_KSXY_RSSRCX_OFFSET		6
#define ECC_KSXY_RSSRCX_MASK		(0x3 << 6)
#define ECC_KSXY_NUMY_OFFSET		8
#define ECC_KSXY_NUMY_MASK		(0x1f << 8)
#define ECC_KSXY_RSSRCY_OFFSET		14
#define ECC_KSXY_RSSRCY_MASK		(0x3 << 14)

/*
 *  Key Store control register
 */
#define KS_CTL			(KS_BASE + 0x00)
#define KS_CTL_START			(0x1 << 0)
#define KS_CTL_CONT			(0x1 << 7)
#define KS_CTL_INIT			(0x1 << 8)
#define KS_CTL_SILENT			(0x1 << 10)
#define KS_CTL_SCMB			(0x1 << 11)
#define KS_CTL_TCLR			(0x1 << 14)
#define KS_CTL_IEN			(0x1 << 15)

#define KS_STS			(KS_BASE + 0x08)
#define KS_STS_IF			(0x1 << 0)
#define KS_STS_EIF			(0x1 << 1)
#define KS_STS_BUSY			(0x1 << 2)
#define KS_STS_SRAMFULL			(0x1 << 3)
#define KS_STS_INITDONE			(0x1 << 7)

#define AES_KEYSZ_SEL_128       (0x0 << AES_CTL_KEYSZ_OFFSET)
#define AES_KEYSZ_SEL_192       (0x1 << AES_CTL_KEYSZ_OFFSET)
#define AES_KEYSZ_SEL_256       (0x2 << AES_CTL_KEYSZ_OFFSET)

#define AES_MODE_ECB            (0x00 << AES_CTL_OPMODE_OFFSET)
#define AES_MODE_CBC            (0x01 << AES_CTL_OPMODE_OFFSET)
#define AES_MODE_CFB            (0x02 << AES_CTL_OPMODE_OFFSET)
#define AES_MODE_OFB            (0x03 << AES_CTL_OPMODE_OFFSET)
#define AES_MODE_CTR            (0x04 << AES_CTL_OPMODE_OFFSET)
#define AES_MODE_CBC_CS1        (0x10 << AES_CTL_OPMODE_OFFSET)
#define AES_MODE_CBC_CS2        (0x11 << AES_CTL_OPMODE_OFFSET)
#define AES_MODE_CBC_CS3        (0x12 << AES_CTL_OPMODE_OFFSET)
#define AES_MODE_GCM            (0x20 << AES_CTL_OPMODE_OFFSET)
#define AES_MODE_GHASH          (0x21 << AES_CTL_OPMODE_OFFSET)
#define AES_MODE_CCM            (0x22 << AES_CTL_OPMODE_OFFSET)

#define SHA_OPMODE_SHA1		(0x0 << HMAC_CTL_OPMODE_OFFSET)
#define SHA_OPMODE_SHA224	(0x5 << HMAC_CTL_OPMODE_OFFSET)
#define SHA_OPMODE_SHA256	(0x4 << HMAC_CTL_OPMODE_OFFSET)
#define SHA_OPMODE_SHA384	(0x7 << HMAC_CTL_OPMODE_OFFSET)
#define SHA_OPMODE_SHA512	(0x6 << HMAC_CTL_OPMODE_OFFSET)
#define SHA_OPMODE_SHAKE128	(0x0 << HMAC_CTL_OPMODE_OFFSET)
#define SHA_OPMODE_SHAKE256	(0x1 << HMAC_CTL_OPMODE_OFFSET)

#define ECCOP_POINT_MUL		(0x0 << ECC_CTL_ECCOP_OFFSET)
#define ECCOP_MODULE		(0x1 << ECC_CTL_ECCOP_OFFSET)
#define ECCOP_POINT_ADD		(0x2 << ECC_CTL_ECCOP_OFFSET)
#define ECCOP_POINT_DOUBLE	(0x3 << ECC_CTL_ECCOP_OFFSET)

#define MODOP_DIV		(0x0 << ECC_CTL_MODOP_OFFSET)
#define MODOP_MUL		(0x1 << ECC_CTL_MODOP_OFFSET)
#define MODOP_ADD		(0x2 << ECC_CTL_MODOP_OFFSET)
#define MODOP_SUB		(0x3 << ECC_CTL_MODOP_OFFSET)

#define AES_BUFF_SIZE		(PAGE_SIZE)
#define SHA_BUFF_SIZE		(PAGE_SIZE)
#define SHA_FDBCK_SIZE		(HMAC_FDBCK_WCNT * 4)

#define AES_KEY_SIZE_128	0UL
#define AES_KEY_SIZE_192	1UL
#define AES_KEY_SIZE_256	2UL

enum {
	CURVE_P_192  = 0x01,
	CURVE_P_224  = 0x02,
	CURVE_P_256  = 0x03,
	CURVE_P_384  = 0x04,
	CURVE_P_521  = 0x05,
	CURVE_K_163  = 0x11,
	CURVE_K_233  = 0x12,
	CURVE_K_283  = 0x13,
	CURVE_K_409  = 0x14,
	CURVE_K_571  = 0x15,
	CURVE_B_163  = 0x21,
	CURVE_B_233  = 0x22,
	CURVE_B_283  = 0x23,
	CURVE_B_409  = 0x24,
	CURVE_B_571  = 0x25,
	CURVE_KO_192 = 0x31,
	CURVE_KO_224 = 0x32,
	CURVE_KO_256 = 0x33,
	CURVE_BP_256 = 0x41,
	CURVE_BP_384 = 0x42,
	CURVE_BP_512 = 0x43,
	CURVE_SM2_256 = 0x50,
	CURVE_25519  = 0x51,
	CURVE_UNDEF,
};

enum {
	CURVE_GF_P,
	CURVE_GF_2M,
};

#endif /* MA35D1_CRYPTO_H */
