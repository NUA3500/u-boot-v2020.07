// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2020 Nuvoton Technology Corp.
 *
 */


#include <cpu_func.h>
#include <common.h>
#include <dm.h>
#include <dm/device_compat.h>
#include <dm/uclass.h>
#include <dm/pinctrl.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <malloc.h>
#include <linux/delay.h>
#include <nand.h>
#include <clk.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>

#include <dt-bindings/clock/ma35d1-clk.h>

/* SYS Registers */
#define REG_SYS_PWRONOTP        (0x004)    /* Power-on Setting OTP Source Register (TZNS) */
#define REG_SYS_PWRONPIN        (0x008)    /* Power-on Setting Pin Source Register (TZNS) */

/* NFI Registers */
#define REG_NAND_FB0		(0x000)	/* DMAC Control and Status Register */
#define REG_NAND_DMACCSR	(0x400)	/* DMAC Control and Status Register */
#define REG_NAND_DMACSAR	(0x408)	/* DMAC Transfer Starting Address Register */
#define REG_NAND_DMACBCR	(0x40C)	/* DMAC Transfer Byte Count Register */
#define REG_NAND_DMACIER	(0x410)	/* DMAC Interrupt Enable Register */
#define REG_NAND_DMACISR	(0x414)	/* DMAC Interrupt Status Register */

#define REG_NAND_FMICSR		(0x800)	/* Global Control and Status Register */
#define REG_NAND_FMIIER		(0x804)	/* Global Interrupt Control Register */
#define REG_NAND_FMIISR		(0x808)	/* Global Interrupt Status Register */

/* NAND-type Flash Registers */
#define REG_SMCSR		(0x8A0)	/* NAND Flash Control and Status Register */
#define REG_SMTCR		(0x8A4)	/* NAND Flash Timing Control Register */
#define REG_SMIER		(0x8A8)	/* NAND Flash Interrupt Control Register */
#define REG_SMISR		(0x8AC)	/* NAND Flash Interrupt Status Register */
#define REG_SMCMD		(0x8B0)	/* NAND Flash Command Port Register */
#define REG_SMADDR		(0x8B4)	/* NAND Flash Address Port Register */
#define REG_SMDATA		(0x8B8)	/* NAND Flash Data Port Register */
#define REG_SMREACTL		(0x8BC)	/* NAND Flash Smart-Media Redundant Area Control Register */
#define REG_NFECR		(0x8C0)	/* NAND Flash Extend Control Regsiter */
#define REG_SMECC_ST0		(0x8D0)	/* Smart-Media ECC Error Status 0 */
#define REG_SMECC_ST1		(0x8D4)	/* Smart-Media ECC Error Status 1 */
#define REG_SMECC_ST2		(0x8D8)	/* Smart-Media ECC Error Status 2 */
#define REG_SMECC_ST3		(0x8DC)	/* Smart-Media ECC Error Status 3 */

/* NAND-type Flash BCH Error Address Registers */
#define REG_BCH_ECC_ADDR0	(0x900)	/* BCH error byte address 0 */
#define REG_BCH_ECC_ADDR1	(0x904)	/* BCH error byte address 1 */
#define REG_BCH_ECC_ADDR2	(0x908)	/* BCH error byte address 2 */
#define REG_BCH_ECC_ADDR3	(0x90C)	/* BCH error byte address 3 */
#define REG_BCH_ECC_ADDR4	(0x910)	/* BCH error byte address 4 */
#define REG_BCH_ECC_ADDR5	(0x914)	/* BCH error byte address 5 */
#define REG_BCH_ECC_ADDR6	(0x918)	/* BCH error byte address 6 */
#define REG_BCH_ECC_ADDR7	(0x91C)	/* BCH error byte address 7 */
#define REG_BCH_ECC_ADDR8	(0x920)	/* BCH error byte address 8 */
#define REG_BCH_ECC_ADDR9	(0x924)	/* BCH error byte address 9 */
#define REG_BCH_ECC_ADDR10	(0x928)	/* BCH error byte address 10 */
#define REG_BCH_ECC_ADDR11	(0x92C)	/* BCH error byte address 11 */

/* NAND-type Flash BCH Error Data Registers */
#define REG_BCH_ECC_DATA0	(0x960)	/* BCH error byte data 0 */
#define REG_BCH_ECC_DATA1	(0x964)	/* BCH error byte data 1 */
#define REG_BCH_ECC_DATA2	(0x968)	/* BCH error byte data 2 */
#define REG_BCH_ECC_DATA3	(0x96C)	/* BCH error byte data 3 */
#define REG_BCH_ECC_DATA4	(0x970)	/* BCH error byte data 4 */
#define REG_BCH_ECC_DATA5	(0x974)	/* BCH error byte data 5 */

/* NAND-type Flash Redundant Area Registers */
#define REG_SMRA0		(0xA00)	/* Smart-Media Redundant Area Register */
#define REG_SMRA1		(0xA04)	/* Smart-Media Redundant Area Register */

/*******************************************/
#define NAND_EN     0x08
#define READYBUSY   (0x01 << 10)
#define ENDADDR     (0x01 << 31)

/*-----------------------------------------------------------------------------
 * Define some constants for BCH
 *---------------------------------------------------------------------------*/
// define the total padding bytes for 512/1024 data segment
#define BCH_PADDING_LEN_512     32
#define BCH_PADDING_LEN_1024    64
// define the BCH parity code lenght for 512 bytes data pattern
#define BCH_PARITY_LEN_T8  15
#define BCH_PARITY_LEN_T12 23
// define the BCH parity code lenght for 1024 bytes data pattern
#define BCH_PARITY_LEN_T24 45


#define BCH_T12   0x00200000
#define BCH_T8    0x00100000
#define BCH_T24   0x00040000


struct ma35d1_nand_info {
	struct udevice		*dev;
	struct mtd_info         mtd;
	struct nand_chip        chip;
	void __iomem 		*reg;
	int                     eBCHAlgo;
	int                     m_i32SMRASize;
};
struct ma35d1_nand_info *ma35d1_nand;

static struct nand_ecclayout ma35d1_nand_oob;

static const int g_i32BCHAlgoIdx[4] = { BCH_T8, BCH_T8, BCH_T12, BCH_T24 };
static const int g_i32ParityNum[3][4] = {
	{ 0,  60,  92,  90 },  // for 2K
	{ 0, 120, 184, 180 },  // for 4K
	{ 0, 240, 368, 360 },  // for 8K
};

static void ma35d1_layout_oob_table ( struct nand_ecclayout* pNandOOBTbl, int oobsize , int eccbytes )
{
	pNandOOBTbl->eccbytes = eccbytes;

	pNandOOBTbl->oobavail = oobsize - 4 - eccbytes ;

	pNandOOBTbl->oobfree[0].offset = 4;  // Bad block marker size

	pNandOOBTbl->oobfree[0].length = oobsize - eccbytes - pNandOOBTbl->oobfree[0].offset ;
}


static void ma35d1_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(nand);

	if (ctrl & NAND_CTRL_CHANGE) {
		ulong IO_ADDR_W = (ulong)REG_SMDATA;

		if ((ctrl & NAND_CLE))
			IO_ADDR_W = REG_SMCMD;
		if ((ctrl & NAND_ALE))
			IO_ADDR_W = REG_SMADDR;

		nand->IO_ADDR_W = (void __iomem *)(nand_info->reg + IO_ADDR_W);
	}

	if (cmd != NAND_CMD_NONE)
		writeb(cmd, nand->IO_ADDR_W);
}


/* select chip */
static void ma35d1_nand_select_chip(struct mtd_info *mtd, int chip)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(nand);

	writel(readl(nand_info->reg+REG_SMCSR)&(~0x06000000), nand_info->reg+REG_SMCSR);
}


static int ma35d1_dev_ready(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(chip);
	int ready;

	ready = (readl(nand_info->reg+REG_SMISR) & READYBUSY) ? 1 : 0;

	return ready;
}


static void ma35d1_nand_command(struct mtd_info *mtd, unsigned int command, int column, int page_addr)
{
	struct nand_chip *chip = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(chip);

	writel(0x400, nand_info->reg+REG_SMISR);

	if (command == NAND_CMD_READOOB) {
		column += mtd->writesize;
		command = NAND_CMD_READ0;
	}

	writel(command & 0xff, nand_info->reg+REG_SMCMD);

	if (command == NAND_CMD_READID) {
		writel(ENDADDR|column, nand_info->reg+REG_SMADDR);
		return;
	} else if (command == NAND_CMD_PARAM) {
		writel(ENDADDR|column, nand_info->reg+REG_SMADDR);

	} else {

		if (column != -1 || page_addr != -1) {
			if (column != -1) {
				writel(column&0xFF, nand_info->reg+REG_SMADDR);
				if ( page_addr != -1 )
					writel(column >> 8, nand_info->reg+REG_SMADDR);
				else
					writel((column >> 8) | ENDADDR, nand_info->reg+REG_SMADDR);
			}

			if (page_addr != -1) {
				writel(page_addr&0xFF, nand_info->reg+REG_SMADDR);

				if ( chip->options & NAND_ROW_ADDR_3) {
//				if ( chip->chipsize > (128 << 20) ) {
					writel((page_addr >> 8)&0xFF, nand_info->reg+REG_SMADDR);
					writel(((page_addr >> 16)&0xFF)|ENDADDR, nand_info->reg+REG_SMADDR);
				} else {
					writel(((page_addr >> 8)&0xFF)|ENDADDR, nand_info->reg+REG_SMADDR);
				}
			}
		}
	}

	switch (command) {
	case NAND_CMD_CACHEDPROG:
	case NAND_CMD_PAGEPROG:
	case NAND_CMD_ERASE1:
	case NAND_CMD_ERASE2:
	case NAND_CMD_SEQIN:
	case NAND_CMD_RNDIN:
	case NAND_CMD_STATUS:
		return;

	case NAND_CMD_RESET:
		if (chip->dev_ready)
			break;

		if ( chip->chip_delay )
			udelay(chip->chip_delay);

		writel(NAND_CMD_STATUS, nand_info->reg+REG_SMCMD);
		writel(command, nand_info->reg+REG_SMCMD);

		while (!(readl(nand_info->reg+REG_SMISR) & READYBUSY)) ;
		return;

	case NAND_CMD_RNDOUT:
		writel(NAND_CMD_RNDOUTSTART, nand_info->reg+REG_SMCMD);
		ndelay(10);
		return;

	case NAND_CMD_READ0:
		writel(NAND_CMD_READSTART, nand_info->reg+REG_SMCMD);
		break;
	default:
		if (!chip->dev_ready) {
			if ( chip->chip_delay )
				udelay(chip->chip_delay);
			return;
		}
	}

	while (!(readl(nand_info->reg+REG_SMISR) & READYBUSY)) ;

}

/*
 * ma35d1_nand_read_byte - read a byte from NAND controller into buffer
 * @mtd: MTD device structure
 */
static unsigned char ma35d1_nand_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(nand);

	return ((unsigned char)readl(nand_info->reg+REG_SMDATA));
}

/*
 * ma35d1_nand_write_buf - write data from buffer into NAND controller
 * @mtd: MTD device structure
 * @buf: virtual address in RAM of source
 * @len: number of data bytes to be transferred
 */

static void ma35d1_nand_write_buf(struct mtd_info *mtd, const unsigned char *buf, int len)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(nand);
	int i;

	for (i = 0; i < len; i++)
		writel(buf[i], nand_info->reg+REG_SMDATA);
}

/*
 * ma35d1_nand_read_buf - read data from NAND controller into buffer
 * @mtd: MTD device structure
 * @buf: virtual address in RAM of source
 * @len: number of data bytes to be transferred
 */
static void ma35d1_nand_read_buf(struct mtd_info *mtd, unsigned char *buf, int len)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(nand);
	int volatile i;

	for (i = 0; i < len; i++)
		buf[i] = (unsigned char)readl(nand_info->reg+REG_SMDATA);
}


/*
 * Enable HW ECC : unused on most chips
 */
void ma35d1_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
}

/*
 * Calculate HW ECC
 * function called after a write
 * mtd:        MTD block structure
 * dat:        raw data (unused)
 * ecc_code:   buffer for ECC
 */
static int ma35d1_nand_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
	return 0;
}

/*
 * HW ECC Correction
 * function called after a read
 * mtd:        MTD block structure
 * dat:        raw data read from the chip
 * read_ecc:   ECC from the chip (unused)
 * isnull:     unused
 */
static int ma35d1_nand_correct_data(struct mtd_info *mtd, u_char *dat,
                                    u_char *read_ecc, u_char *calc_ecc)
{
	return 0;
}


/*-----------------------------------------------------------------------------
 * Correct data by BCH alrogithm.
 *      Support 8K page size NAND and BCH T4/8/12/15/24.
 *---------------------------------------------------------------------------*/
void fmiSM_CorrectData_BCH(struct mtd_info *mtd, u8 ucFieidIndex, u8 ucErrorCnt, u8* pDAddr)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(nand);
	u32 uaData[24], uaAddr[24];
	u32 uaErrorData[6];
	u8  ii, jj;
	u32 uPageSize;
	u32 field_len, padding_len, parity_len;
	u32 total_field_num;
	u8  *smra_index;

	//--- assign some parameters for different BCH and page size
	switch (readl(nand_info->reg+REG_SMCSR) & 0x007C0000) {
	case BCH_T24:
		field_len   = 1024;
		padding_len = BCH_PADDING_LEN_1024;
		parity_len  = BCH_PARITY_LEN_T24;
		break;
	case BCH_T12:
		field_len   = 512;
		padding_len = BCH_PADDING_LEN_512;
		parity_len  = BCH_PARITY_LEN_T12;
		break;
	case BCH_T8:
		field_len   = 512;
		padding_len = BCH_PADDING_LEN_512;
		parity_len  = BCH_PARITY_LEN_T8;
		break;
	default:
		return;
	}

	uPageSize = readl(nand_info->reg+REG_SMCSR) & 0x00030000;
	switch (uPageSize) {
	case 0x30000:
		total_field_num = 8192 / field_len;
		break;
	case 0x20000:
		total_field_num = 4096 / field_len;
		break;
	case 0x10000:
		total_field_num = 2048 / field_len;
		break;
	case 0x00000:
		total_field_num =  512 / field_len;
		break;
	default:
		return;
	}

	//--- got valid BCH_ECC_DATAx and parse them to uaData[]
	// got the valid register number of BCH_ECC_DATAx since one register include 4 error bytes
	jj = ucErrorCnt/4;
	jj ++;
	if (jj > 6)
		jj = 6;     // there are 6 BCH_ECC_DATAx registers to support BCH T24

	for(ii=0; ii<jj; ii++) {
		uaErrorData[ii] = readl(nand_info->reg+REG_BCH_ECC_DATA0 + ii*4);
	}

	for(ii=0; ii<jj; ii++) {
		uaData[ii*4+0] = uaErrorData[ii] & 0xff;
		uaData[ii*4+1] = (uaErrorData[ii]>>8) & 0xff;
		uaData[ii*4+2] = (uaErrorData[ii]>>16) & 0xff;
		uaData[ii*4+3] = (uaErrorData[ii]>>24) & 0xff;
	}

	//--- got valid REG_BCH_ECC_ADDRx and parse them to uaAddr[]
	// got the valid register number of REG_BCH_ECC_ADDRx since one register include 2 error addresses
	jj = ucErrorCnt/2;
	jj ++;
	if (jj > 12)
		jj = 12;    // there are 12 REG_BCH_ECC_ADDRx registers to support BCH T24

	for(ii=0; ii<jj; ii++) {
		uaAddr[ii*2+0] = readl(nand_info->reg+REG_BCH_ECC_ADDR0 + ii*4) & 0x07ff;   // 11 bits for error address
		uaAddr[ii*2+1] = (readl(nand_info->reg+REG_BCH_ECC_ADDR0 + ii*4)>>16) & 0x07ff;
	}

	//--- pointer to begin address of field that with data error
	pDAddr += (ucFieidIndex-1) * field_len;

	//--- correct each error bytes
	for(ii=0; ii<ucErrorCnt; ii++) {
		// for wrong data in field
		if (uaAddr[ii] < field_len) {
			*(pDAddr+uaAddr[ii]) ^= uaData[ii];
		}
		// for wrong first-3-bytes in redundancy area
		else if (uaAddr[ii] < (field_len+3)) {
			uaAddr[ii] -= field_len;
			uaAddr[ii] += (parity_len*(ucFieidIndex-1));    // field offset
			*((u8 *)(nand_info->reg+REG_SMRA0) + uaAddr[ii]) ^= uaData[ii];
		}
		// for wrong parity code in redundancy area
		else {
			// BCH_ERR_ADDRx = [data in field] + [3 bytes] + [xx] + [parity code]
			//                                   |<--     padding bytes      -->|
			// The BCH_ERR_ADDRx for last parity code always = field size + padding size.
			// So, the first parity code = field size + padding size - parity code length.
			// For example, for BCH T12, the first parity code = 512 + 32 - 23 = 521.
			// That is, error byte address offset within field is
			uaAddr[ii] = uaAddr[ii] - (field_len + padding_len - parity_len);

			// smra_index point to the first parity code of first field in register SMRA0~n
			smra_index = (u8 *)
			             (nand_info->reg+REG_SMRA0 + (readl(nand_info->reg+REG_SMREACTL) & 0x1ff) - // bottom of all parity code -
			              (parity_len * total_field_num)                             // byte count of all parity code
			             );

			// final address = first parity code of first field +
			//                 offset of fields +
			//                 offset within field
			*((u8 *)smra_index + (parity_len * (ucFieidIndex-1)) + uaAddr[ii]) ^= uaData[ii];
		}
	}   // end of for (ii<ucErrorCnt)
}

int fmiSMCorrectData (struct mtd_info *mtd, unsigned long uDAddr )
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(nand);
	int uStatus, ii, jj, i32FieldNum=0;
	volatile int uErrorCnt = 0;

	if ( readl ( nand_info->reg+REG_SMISR ) & 0x4 ) {
		if ( ( readl(nand_info->reg+REG_SMCSR) & 0x7C0000) == BCH_T24 )
			i32FieldNum = mtd->writesize / 1024;    // Block=1024 for BCH
		else
			i32FieldNum = mtd->writesize / 512;

		if ( i32FieldNum < 4 )
			i32FieldNum  = 1;
		else
			i32FieldNum /= 4;

		for ( jj=0; jj<i32FieldNum; jj++ ) {
			uStatus = readl ( nand_info->reg+REG_SMECC_ST0+jj*4 );
			if ( !uStatus )
				continue;

			for ( ii=1; ii<5; ii++ ) {
				if ( !(uStatus & 0x03) ) { // No error

					uStatus >>= 8;
					continue;

				} else if ( (uStatus & 0x03)==0x01 ) { // Correctable error

					uErrorCnt = (uStatus >> 2) & 0x1F;
					fmiSM_CorrectData_BCH(mtd, jj*4+ii, uErrorCnt, (u8 *)uDAddr);

					break;
				} else { // uncorrectable error or ECC error
					pr_err("uncorrectable!\n");
					return -1;
				}
				uStatus >>= 8;
			}
		} //jj
	}
	return uErrorCnt;
}


static inline int ma35d1_nand_dma_transfer(struct mtd_info *mtd, const u_char *addr, unsigned int len, int is_write)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(nand);

	// For save, wait DMAC to ready
	while ( readl(nand_info->reg+REG_NAND_DMACCSR) & 0x200 );

	// Reinitial dmac
	// DMAC enable
	writel( readl(nand_info->reg+REG_NAND_DMACCSR) | 0x3, nand_info->reg+REG_NAND_DMACCSR);
	while (readl(nand_info->reg+REG_NAND_DMACCSR) & 0x2);

	// Clear DMA finished flag
	writel( readl(nand_info->reg+REG_SMISR) | 0x1, nand_info->reg+REG_SMISR);

	// Disable Interrupt
	writel(readl(nand_info->reg+REG_SMIER) & ~(0x1), nand_info->reg+REG_SMIER);

	// Fill dma_addr
	flush_dcache_range((unsigned long)addr, (unsigned long)addr + len);
	writel((unsigned long)addr, nand_info->reg+REG_NAND_DMACSAR);

	// Enable target abort interrupt generation during DMA transfer.
	writel( 0x1, nand_info->reg+REG_NAND_DMACIER);

	// Clear Ready/Busy 0 Rising edge detect flag
	writel(0x400, nand_info->reg+REG_SMISR);

	// Set which BCH algorithm
	if ( nand_info->eBCHAlgo >= 0 ) {
		// Set BCH algorithm
		writel( (readl(nand_info->reg+REG_SMCSR) & (~0x7C0000)) | g_i32BCHAlgoIdx[nand_info->eBCHAlgo], nand_info->reg+REG_SMCSR);
		// Enable H/W ECC, ECC parity check enable bit during read page
		writel( readl(nand_info->reg+REG_SMCSR) | 0x00800080, nand_info->reg+REG_SMCSR);
	} else  {
		// Disable H/W ECC / ECC parity check enable bit during read page
		writel( readl(nand_info->reg+REG_SMCSR) & (~0x00800080), nand_info->reg+REG_SMCSR);
	}

	writel( nand_info->m_i32SMRASize , nand_info->reg+REG_SMREACTL );

	writel( readl(nand_info->reg+REG_SMIER) & (~0x4), nand_info->reg+REG_SMIER );

	writel ( 0x4, nand_info->reg+REG_SMISR );

	// Enable SM_CS0
	writel((readl(nand_info->reg+REG_SMCSR)&(~0x06000000))|0x04000000, nand_info->reg+REG_SMCSR);
	/* setup and start DMA using dma_addr */

	if ( is_write ) {
		register char *ptr= (char *)(nand_info->reg+REG_SMRA0);
		// To mark this page as dirty.
		if ( ptr[3] == 0xFF )
			ptr[3] = 0;
		if ( ptr[2] == 0xFF )
			ptr[2] = 0;

		writel ( readl(nand_info->reg+REG_SMCSR) | 0x4, nand_info->reg+REG_SMCSR );
		while ( !(readl(nand_info->reg+REG_SMISR) & 0x1) );

	} else {
		// Blocking for reading
		// Enable DMA Read

		writel ( readl(nand_info->reg+REG_SMCSR) | 0x2, nand_info->reg+REG_SMCSR);

		if ( readl(nand_info->reg+REG_SMCSR) & 0x80 ) {
			do {
				int stat=0;
				if ( (stat=fmiSMCorrectData ( mtd,  (unsigned long)addr)) < 0 ) {
					mtd->ecc_stats.failed++;
					writel(0x4, nand_info->reg+REG_SMISR );
					writel(0x3, nand_info->reg+REG_NAND_DMACCSR);          // reset DMAC
					writel(readl(nand_info->reg+REG_SMCSR)|0x1, nand_info->reg+REG_SMCSR);    // reset SM controller
					break;
				} else if ( stat > 0 ) {
					//mtd->ecc_stats.corrected += stat; //Occure: MLC UBIFS mount error
					writel(0x4, nand_info->reg+REG_SMISR );
				}

			} while (!(readl(nand_info->reg+REG_SMISR) & 0x1) || (readl(nand_info->reg+REG_SMISR) & 0x4));
		} else
			while (!(readl(nand_info->reg+REG_SMISR) & 0x1));
		invalidate_dcache_range((unsigned long)addr, (unsigned long)addr + len);
	}

	// Clear DMA finished flag
	writel(readl(nand_info->reg+REG_SMISR) | 0x1, nand_info->reg+REG_SMISR);

	return 0;
}


/**
 * nand_write_page_hwecc - [REPLACABLE] hardware ecc based page write function
 * @mtd:        mtd info structure
 * @chip:       nand chip info structure
 * @buf:        data buffer
 */
static int ma35d1_nand_write_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf, int oob_required, int page)
{
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(chip);
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	register char * ptr=(char *)(nand_info->reg+REG_SMRA0);

	memset ( (void*)ptr, 0xFF, mtd->oobsize );
	memcpy ( (void*)ptr, (void*)chip->oob_poi,  mtd->oobsize - chip->ecc.total );

	ma35d1_nand_command(mtd, NAND_CMD_SEQIN, 0, page);
	ma35d1_nand_dma_transfer( mtd, buf, mtd->writesize , 0x1);
	ma35d1_nand_command(mtd, NAND_CMD_PAGEPROG, -1, -1);

	// Copy parity code in SMRA to calc
	memcpy ( (void*)ecc_calc,  (void*)( (long)ptr + ( mtd->oobsize - chip->ecc.total ) ), chip->ecc.total );

	// Copy parity code in calc to oob_poi
	memcpy ( (void*)(chip->oob_poi+(mtd->oobsize-chip->ecc.total)), (void*)ecc_calc, chip->ecc.total);

	return 0;
}

/**
 * ma35d1_nand_read_page_hwecc_oob_first - hardware ecc based page write function
 * @mtd:        mtd info structure
 * @chip:       nand chip info structure
 * @buf:        buffer to store read data
 * @page:       page number to read
 */
static int ma35d1_nand_read_page_hwecc_oob_first(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf, int oob_required, int page)
{
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(chip);
	uint8_t *p = buf;
	char * ptr= (char *)(nand_info->reg+REG_SMRA0);

	/* At first, read the OOB area  */
	ma35d1_nand_command(mtd, NAND_CMD_READOOB, 0, page);
	ma35d1_nand_read_buf(mtd, chip->oob_poi, mtd->oobsize);

	// Second, copy OOB data to SMRA for page read
	memcpy ( (void*)ptr, (void*)chip->oob_poi, mtd->oobsize );

	if ((*(ptr+2) != 0) && (*(ptr+3) != 0))
	{
		memset((void*)p, 0xff, mtd->writesize);
	}
	else
	{
		// Third, read data from nand
		ma35d1_nand_command(mtd, NAND_CMD_READ0, 0, page);
		ma35d1_nand_dma_transfer(mtd, p, mtd->writesize, 0x0);

		// Fouth, restore OOB data from SMRA
		memcpy ( (void*)chip->oob_poi, (void*)ptr, mtd->oobsize );
	}

	return 0;
}

/**
 * ma35d1_nand_read_oob_hwecc - [REPLACABLE] the most common OOB data read function
 * @mtd:        mtd info structure
 * @chip:       nand chip info structure
 * @page:       page number to read
 * @sndcmd:     flag whether to issue read command or not
 */
static int ma35d1_nand_read_oob_hwecc(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
	struct nand_chip *nand = mtd_to_nand(mtd);
	struct ma35d1_nand_info *nand_info = nand_get_controller_data(nand);
	char * ptr=(char *)(nand_info->reg+REG_SMRA0);

	/* At first, read the OOB area  */
	ma35d1_nand_command(mtd, NAND_CMD_READOOB, 0, page);

	ma35d1_nand_read_buf(mtd, chip->oob_poi, mtd->oobsize);

	// Second, copy OOB data to SMRA for page read
	memcpy ( (void*)ptr, (void*)chip->oob_poi, mtd->oobsize );

	if ((*(ptr+2) != 0) && (*(ptr+3) != 0))
	{
		memset((void*)chip->oob_poi, 0xff, mtd->oobsize);
	}
	return 0; //CWWeng 2017.2.14
}



int ma35d1_nand_init(struct ma35d1_nand_info *nand_info)
{
	struct nand_chip *nand = &nand_info->chip;
	struct mtd_info *mtd = nand_to_mtd(nand);
	unsigned int reg;
	int ret=0;

	nand_set_controller_data(nand, nand_info);
	nand->options |= NAND_NO_SUBPAGE_WRITE;

	nand->flash_node = dev_of_offset(nand_info->dev);
	/* hwcontrol always must be implemented */
	nand->cmd_ctrl = ma35d1_hwcontrol;
	nand->cmdfunc = ma35d1_nand_command;
	nand->dev_ready = ma35d1_dev_ready;
	nand->select_chip = ma35d1_nand_select_chip;

	nand->read_byte = ma35d1_nand_read_byte;
	nand->write_buf = ma35d1_nand_write_buf;
	nand->read_buf = ma35d1_nand_read_buf;
	nand->chip_delay = 50;

	nand->ecc.hwctl     = ma35d1_nand_enable_hwecc;
	nand->ecc.calculate = ma35d1_nand_calculate_ecc;
	nand->ecc.correct   = ma35d1_nand_correct_data;
	nand->ecc.write_page= ma35d1_nand_write_page_hwecc;
	nand->ecc.read_page = ma35d1_nand_read_page_hwecc_oob_first;
	nand->ecc.read_oob  = ma35d1_nand_read_oob_hwecc;
	nand->ecc.layout    = &ma35d1_nand_oob;

	mtd->priv = nand;

	// Enable SM_EN
	writel(NAND_EN, nand_info->reg+REG_NAND_FMICSR);

	// Enable SM_CS0
	writel((readl(nand_info->reg+REG_SMCSR)&(~0x06000000))|0x04000000, nand_info->reg+REG_SMCSR);
	writel(0x1, nand_info->reg+REG_NFECR); /* un-lock write protect */

	// NAND Reset
	writel(readl(nand_info->reg+REG_SMCSR) | 0x1, nand_info->reg+REG_SMCSR);    // software reset
	while (readl(nand_info->reg+REG_SMCSR) & 0x1);

	/* Detect NAND chips */
	ret = nand_scan(mtd, 1);
	if (ret)
		return ret;

	//Set PSize bits of SMCSR register to select NAND card page size
	reg = readl(nand_info->reg+REG_SMCSR) & (~0x30000);
	writel(reg | (mtd->writesize << 5), nand_info->reg+REG_SMCSR);

	if (nand->ecc.strength == 0) {
		nand_info->eBCHAlgo = 0; /* No ECC */
		ma35d1_layout_oob_table(&ma35d1_nand_oob, mtd->oobsize, g_i32ParityNum[mtd->writesize>>12][nand_info->eBCHAlgo]);

	} else if (nand->ecc.strength <= 8) {
		nand_info->eBCHAlgo = 1; /* T8 */
		ma35d1_layout_oob_table(&ma35d1_nand_oob, mtd->oobsize, g_i32ParityNum[mtd->writesize>>12][nand_info->eBCHAlgo]);

	} else if (nand->ecc.strength <= 12) {
		nand_info->eBCHAlgo = 2; /* T12 */
		ma35d1_layout_oob_table(&ma35d1_nand_oob, mtd->oobsize, g_i32ParityNum[mtd->writesize>>12][nand_info->eBCHAlgo]);

	} else if (nand->ecc.strength <= 24) {
		nand_info->eBCHAlgo = 3; /* T24 */
		ma35d1_layout_oob_table(&ma35d1_nand_oob, mtd->oobsize, g_i32ParityNum[mtd->writesize>>12][nand_info->eBCHAlgo]);

	} else {
		pr_warn("NAND Controller is not support this flash. (%d, %d)\n", mtd->writesize, mtd->oobsize);
	}

	nand_info->m_i32SMRASize  = mtd->oobsize;
	nand->ecc.steps = mtd->writesize / nand->ecc.size;
	nand->ecc.bytes = ma35d1_nand_oob.eccbytes / nand->ecc.steps;
	nand->ecc.total = ma35d1_nand_oob.eccbytes;
	//mtd_set_ooblayout(mtd, &ma35d1_ooblayout_ops);

	nand->options = 0;

	// Redundant area size
	writel( nand_info->m_i32SMRASize , nand_info->reg+REG_SMREACTL );

	// Protect redundant 3 bytes
	// because we need to implement write_oob function to partial data to oob available area.
	// Please note we skip 4 bytes
	writel( readl(nand_info->reg+REG_SMCSR) | 0x100, nand_info->reg+REG_SMCSR);

	// To read/write the ECC parity codes automatically from/to NAND Flash after data area field written.
	writel( readl(nand_info->reg+REG_SMCSR) | 0x10, nand_info->reg+REG_SMCSR);
	// Set BCH algorithm
	writel( (readl(nand_info->reg+REG_SMCSR) & (~0x007C0000)) | g_i32BCHAlgoIdx[nand_info->eBCHAlgo], nand_info->reg+REG_SMCSR);
	// Enable H/W ECC, ECC parity check enable bit during read page
	writel( readl(nand_info->reg+REG_SMCSR) | 0x00800080, nand_info->reg+REG_SMCSR);

	nand_register(0, mtd);

	return 0;
}

static int ma35d1_nand_probe(struct udevice *dev)
{
	struct ma35d1_nand_info *info = dev_get_priv(dev);
	struct resource res;
	int ret;

	info->dev = dev;

	/* get nand info */
	ret = dev_read_resource_byname(dev, "nand", &res);
	if (ret)
		return ret;

	info->reg = devm_ioremap(dev, res.start, resource_size(&res));

	return ma35d1_nand_init(info);
}

static const struct udevice_id ma35d1_nand_ids[] = {
	{ .compatible = "nuvoton,ma35d1-nand" },
	{ }
};

U_BOOT_DRIVER(ma35d1_nand) = {
	.name = "ma35d1-nand",
	.id = UCLASS_MTD,
	.of_match = ma35d1_nand_ids,
	.probe = ma35d1_nand_probe,
	.priv_auto_alloc_size = sizeof(struct ma35d1_nand_info),
};

void board_nand_init(void)
{
	struct udevice *dev;
	struct clk clk;
	int ret;

	/* enable nand clock */
	ret = uclass_get_device_by_driver(UCLASS_CLK, DM_GET_DRIVER(ma35d1_clk), &dev);
	if (ret)
		pr_err("Failed to get nand clock. (error %d)\n", ret);

	clk.id = nand_gate;
	ret = clk_request(dev, &clk);
	if (ret < 0) {
		dev_err(dev, "%s clk_request() failed: %d\n", __func__, ret);
	}
	ret = clk_enable(&clk);
	if (ret == -ENOTSUPP) {
		dev_err(dev, "clk not supported yet\n");
	}

	ret = uclass_get_device_by_driver(UCLASS_MTD,
					  DM_GET_DRIVER(ma35d1_nand),
					  &dev);
	if (ret && ret != -ENODEV)
		pr_err("Failed to initialize ma35d1 NAND controller. (error %d)\n", ret);
}
