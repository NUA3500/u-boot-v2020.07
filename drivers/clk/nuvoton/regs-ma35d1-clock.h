/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2020 Nuvoton Technology Corporation.
 */

#ifndef __DRIVERS_CLK_NUVOTON_REGS_MA35D1_CLOCK_H
#define __DRIVERS_CLK_NUVOTON_REGS_MA35D1_CLOCK_H

/* Clock Control Registers  */
#define REG_CLK_PWRCTL      (0x00)    /* System Power-down Control Register */
#define REG_CLK_SYSCLK0     (0x04)    /* AXI and AHB Device Clock Enable Control Register 0 */
#define REG_CLK_SYSCLK1     (0x08)    /* AXI and AHB Device Clock Enable Control Register 1 */
#define REG_CLK_APBCLK0     (0x0C)    /* APB Devices Clock Enable Control Register 0 */
#define REG_CLK_APBCLK1     (0x10)    /* APB Devices Clock Enable Control Register 1 */
#define REG_CLK_APBCLK2     (0x14)    /* APB Devices Clock Enable Control Register 2 */
#define REG_CLK_CLKSEL0     (0x18)    /* Clock Source Select Control Register 0 */
#define REG_CLK_CLKSEL1     (0x1C)    /* Clock Source Select Control Register 1 */
#define REG_CLK_CLKSEL2     (0x20)    /* Clock Source Select Control Register 2 */
#define REG_CLK_CLKSEL3     (0x24)    /* Clock Source Select Control Register 3 */
#define REG_CLK_CLKSEL4     (0x28)    /* Clock Source Select Control Register 4 */
#define REG_CLK_CLKDIV0     (0x2C)    /* Clock Divider Number Register 0 */
#define REG_CLK_CLKDIV1     (0x30)    /* Clock Divider Number Register 1 */
#define REG_CLK_CLKDIV2     (0x34)    /* Clock Divider Number Register 2 */
#define REG_CLK_CLKDIV3     (0x38)    /* Clock Divider Number Register 3 */
#define REG_CLK_CLKDIV4     (0x3C)    /* Clock Divider Number Register 4 */
#define REG_CLK_CLKOCTL     (0x40)    /* Clock Output Control Register (Write Protect) */
#define REG_CLK_STATUS      (0x50)    /* Clock Status Monitor Register  */
#define REG_CLK_PLL0CTL0    (0x60)    /* CA-pLL Control Register 0(Write Protect) */
#define REG_CLK_PLL0CTL1    (0x64)    /* CA-pLL Control Register 1(Write Protect) */
#define REG_CLK_PLL0CTL2    (0x68)    /* CA-pLL Control Register 2(Write Protect) */
#define REG_CLK_PLL1CTL0    (0x70)    /* SYS-pLL Control Register 0(Write Protect) */
#define REG_CLK_PLL1CTL1    (0x74)    /* SYS-pLL Control Register 1(Write Protect) */
#define REG_CLK_PLL1CTL2    (0x78)    /* SYS-pLL Control Register 2(Write Protect) */
#define REG_CLK_PLL2CTL0    (0x80)    /* DDR-pLL Control Register 0(Write Protect) */
#define REG_CLK_PLL2CTL1    (0x84)    /* DDR-pLL Control Register 1(Write Protect) */
#define REG_CLK_PLL2CTL2    (0x88)    /* DDR-pLL Control Register 2(Write Protect) */
#define REG_CLK_PLL3CTL0    (0x90)    /* APLL Control Register 0(Write Protect) */
#define REG_CLK_PLL3CTL1    (0x94)    /* APLL Control Register 1(Write Protect) */
#define REG_CLK_PLL3CTL2    (0x98)    /* APLL Control Register 2(Write Protect) */
#define REG_CLK_PLL4CTL0    (0xA0)    /* EPLL Control Register 0(Write Protect) */
#define REG_CLK_PLL4CTL1    (0xA4)    /* EPLL Control Register 1(Write Protect) */
#define REG_CLK_PLL4CTL2    (0xA8)    /* EPLL Control Register 2(Write Protect) */
#define REG_CLK_PLL5CTL0    (0xB0)    /* VPLL Control Register 0(Write Protect) */
#define REG_CLK_PLL5CTL1    (0xB4)    /* VPLL Control Register 1(Write Protect) */
#define REG_CLK_PLL5CTL2    (0xB8)    /* VPLL Control Register 2(Write Protect) */
#define REG_CLK_CLKDCTL     (0xC0)    /* Clock Fail Detector Control Register(Write Protect) */
#define REG_CLK_CLKDSTS     (0xC4)    /* Clock Fail Detector Status Register(Write Protect) */
#define REG_CLK_CDUPB       (0xC8)    /* Clock Frequency Detector Upper Boundary Register(Write Protect) */
#define REG_CLK_CDLOWB      (0xCC)    /* Clock Frequency Detector Lower Boundary Register(Write Protect) */
#define REG_CLK_CKFLTRCTL   (0xD0)    /* Clock Filter Control Register (Write Protect) */
#define REG_CLK_TESTCLK     (0xF0)    /* Test Clock Control Register */


/**
    @addtogroup CLK_CONST CLK Bit Field Definition
    Constant Definitions for CLK Controller
@{ */

#define CLK_PWRCTL_HXTEN_Pos             (0)                                               /*!< CLK_T::PWRCTL: HXTEN Position          */
#define CLK_PWRCTL_HXTEN_Msk             (0x1ul << CLK_PWRCTL_HXTEN_Pos)                   /*!< CLK_T::PWRCTL: HXTEN Mask              */

#define CLK_PWRCTL_LXTEN_Pos             (1)                                               /*!< CLK_T::PWRCTL: LXTEN Position          */
#define CLK_PWRCTL_LXTEN_Msk             (0x1ul << CLK_PWRCTL_LXTEN_Pos)                   /*!< CLK_T::PWRCTL: LXTEN Mask              */

#define CLK_PWRCTL_HIRCEN_Pos            (2)                                               /*!< CLK_T::PWRCTL: HIRCEN Position         */
#define CLK_PWRCTL_HIRCEN_Msk            (0x1ul << CLK_PWRCTL_HIRCEN_Pos)                  /*!< CLK_T::PWRCTL: HIRCEN Mask             */

#define CLK_PWRCTL_LIRCEN_Pos            (3)                                               /*!< CLK_T::PWRCTL: LIRCEN Position         */
#define CLK_PWRCTL_LIRCEN_Msk            (0x1ul << CLK_PWRCTL_LIRCEN_Pos)                  /*!< CLK_T::PWRCTL: LIRCEN Mask             */

#define CLK_PWRCTL_PDWKDLY_Pos           (4)                                               /*!< CLK_T::PWRCTL: PDWKDLY Position        */
#define CLK_PWRCTL_PDWKDLY_Msk           (0x1ul << CLK_PWRCTL_PDWKDLY_Pos)                 /*!< CLK_T::PWRCTL: PDWKDLY Mask            */

#define CLK_PWRCTL_PDWKIEN_Pos           (5)                                               /*!< CLK_T::PWRCTL: PDWKIEN Position        */
#define CLK_PWRCTL_PDWKIEN_Msk           (0x1ul << CLK_PWRCTL_PDWKIEN_Pos)                 /*!< CLK_T::PWRCTL: PDWKIEN Mask            */

#define CLK_PWRCTL_PDWKIF_Pos            (6)                                               /*!< CLK_T::PWRCTL: PDWKIF Position         */
#define CLK_PWRCTL_PDWKIF_Msk            (0x1ul << CLK_PWRCTL_PDWKIF_Pos)                  /*!< CLK_T::PWRCTL: PDWKIF Mask             */

#define CLK_PWRCTL_PDEN_Pos              (7)                                               /*!< CLK_T::PWRCTL: PDEN Position           */
#define CLK_PWRCTL_PDEN_Msk              (0x1ul << CLK_PWRCTL_PDEN_Pos)                    /*!< CLK_T::PWRCTL: PDEN Mask               */

#define CLK_PWRCTL_HXTDS_Pos             (10)                                              /*!< CLK_T::PWRCTL: HXTDS Position          */
#define CLK_PWRCTL_HXTDS_Msk             (0x3ul << CLK_PWRCTL_HXTDS_Pos)                   /*!< CLK_T::PWRCTL: HXTDS Mask              */

#define CLK_PWRCTL_HIRCSTBS_Pos          (16)                                              /*!< CLK_T::PWRCTL: HIRCSTBS Position       */
#define CLK_PWRCTL_HIRCSTBS_Msk          (0x3ul << CLK_PWRCTL_HIRCSTBS_Pos)                /*!< CLK_T::PWRCTL: HIRCSTBS Mask           */

#define CLK_SYSCLK0_CA35CKEN_Pos         (0)                                               /*!< CLK_T::SYSCLK0: CA35CKEN Position      */
#define CLK_SYSCLK0_CA35CKEN_Msk         (0x1ul << CLK_SYSCLK0_CA35CKEN_Pos)               /*!< CLK_T::SYSCLK0: CA35CKEN Mask          */

#define CLK_SYSCLK0_CM4CKEN_Pos          (1)                                               /*!< CLK_T::SYSCLK0: CM4CKEN Position       */
#define CLK_SYSCLK0_CM4CKEN_Msk          (0x1ul << CLK_SYSCLK0_CM4CKEN_Pos)                /*!< CLK_T::SYSCLK0: CM4CKEN Mask           */

#define CLK_SYSCLK0_TAHBCKEN_Pos         (2)                                               /*!< CLK_T::SYSCLK0: TAHBCKEN Position      */
#define CLK_SYSCLK0_TAHBCKEN_Msk         (0x1ul << CLK_SYSCLK0_TAHBCKEN_Pos)               /*!< CLK_T::SYSCLK0: TAHBCKEN Mask          */

#define CLK_SYSCLK0_DDR6CKEN_Pos         (3)                                               /*!< CLK_T::SYSCLK0: DDR6CKEN Position      */
#define CLK_SYSCLK0_DDR6CKEN_Msk         (0x1ul << CLK_SYSCLK0_DDR6CKEN_Pos)               /*!< CLK_T::SYSCLK0: DDR6CKEN Mask          */

#define CLK_SYSCLK0_LVRDBEN_Pos          (4)                                               /*!< CLK_T::SYSCLK0: LVRDBEN Position      */
#define CLK_SYSCLK0_LVRDBEN_Msk          (0x1ul << CLK_SYSCLK1_LVRDBEN_Pos)                /*!< CLK_T::SYSCLK0: LVRDBEN Mask          */

#define CLK_SYSCLK0_SDH0CKEN_Pos         (16)                                              /*!< CLK_T::SYSCLK0: SDH0CKEN Position      */
#define CLK_SYSCLK0_SDH0CKEN_Msk         (0x1ul << CLK_SYSCLK0_SDH0CKEN_Pos)               /*!< CLK_T::SYSCLK0: SDH0CKEN Mask          */

#define CLK_SYSCLK0_SDH1CKEN_Pos         (17)                                              /*!< CLK_T::SYSCLK0: SDH1CKEN Position      */
#define CLK_SYSCLK0_SDH1CKEN_Msk         (0x1ul << CLK_SYSCLK0_SDH1CKEN_Pos)               /*!< CLK_T::SYSCLK0: SDH1CKEN Mask          */

#define CLK_SYSCLK0_NANDCKEN_Pos         (18)                                              /*!< CLK_T::SYSCLK0: NANDCKEN Position      */
#define CLK_SYSCLK0_NANDCKEN_Msk         (0x1ul << CLK_SYSCLK0_NANDCKEN_Pos)               /*!< CLK_T::SYSCLK0: NANDCKEN Mask          */

#define CLK_SYSCLK0_USBDCKEN_Pos         (19)                                              /*!< CLK_T::SYSCLK0: USBDCKEN Position      */
#define CLK_SYSCLK0_USBDCKEN_Msk         (0x1ul << CLK_SYSCLK0_USBDCKEN_Pos)               /*!< CLK_T::SYSCLK0: USBDCKEN Mask          */

#define CLK_SYSCLK0_USBHCKEN_Pos         (20)                                              /*!< CLK_T::SYSCLK0: USBHCKEN Position      */
#define CLK_SYSCLK0_USBHCKEN_Msk         (0x1ul << CLK_SYSCLK0_USBHCKEN_Pos)               /*!< CLK_T::SYSCLK0: USBHCKEN Mask          */

#define CLK_SYSCLK0_HUSBH0EN_Pos         (21)                                              /*!< CLK_T::SYSCLK0: HUSBH0EN Position      */
#define CLK_SYSCLK0_HUSBH0EN_Msk         (0x1ul << CLK_SYSCLK0_HUSBH0EN_Pos)               /*!< CLK_T::SYSCLK0: HUSBH0EN Mask          */

#define CLK_SYSCLK0_HUSBH1EN_Pos         (22)                                              /*!< CLK_T::SYSCLK0: HUSBH1EN Position      */
#define CLK_SYSCLK0_HUSBH1EN_Msk         (0x1ul << CLK_SYSCLK0_HUSBH1EN_Pos)               /*!< CLK_T::SYSCLK0: HUSBH1EN Mask          */

#define CLK_SYSCLK0_GFXCKEN_Pos          (24)                                              /*!< CLK_T::SYSCLK0: GFXCKEN Position       */
#define CLK_SYSCLK0_GFXCKEN_Msk          (0x1ul << CLK_SYSCLK0_GFXCKEN_Pos)                /*!< CLK_T::SYSCLK0: GFXCKEN Mask           */

#define CLK_SYSCLK0_VC8KCKEN_Pos         (25)                                              /*!< CLK_T::SYSCLK0: VC8KCKEN Position     */
#define CLK_SYSCLK0_VC8KCKEN_Msk         (0x1ul << CLK_SYSCLK0_VC8KCKEN_Pos)               /*!< CLK_T::SYSCLK0: VC8KCKEN Mask         */

#define CLK_SYSCLK0_DCUCKEN_Pos          (26)                                              /*!< CLK_T::SYSCLK0: DCUCKEN Position      */
#define CLK_SYSCLK0_DCUCKEN_Msk          (0x1ul << CLK_SYSCLK0_DCUCKEN_Pos)                /*!< CLK_T::SYSCLK0: DCUCKEN Mask          */

#define CLK_SYSCLK0_GMAC0CKEN_Pos        (27)                                              /*!< CLK_T::SYSCLK0: GMAC0CKEN Position     */
#define CLK_SYSCLK0_GMAC0CKEN_Msk        (0x1ul << CLK_SYSCLK0_GMAC0CKEN_Pos)              /*!< CLK_T::SYSCLK0: GMAC0CKEN Mask         */

#define CLK_SYSCLK0_GMAC1CKEN_Pos        (28)                                              /*!< CLK_T::SYSCLK0: GMAC1CKEN Position     */
#define CLK_SYSCLK0_GMAC1CKEN_Msk        (0x1ul << CLK_SYSCLK0_GMAC1CKEN_Pos)              /*!< CLK_T::SYSCLK0: GMAC1CKEN Mask         */

#define CLK_SYSCLK0_CAP0HCKEN_Pos        (29)                                              /*!< CLK_T::SYSCLK0: CAP0HCKEN Position     */
#define CLK_SYSCLK0_CAP0HCKEN_Msk        (0x1ul << CLK_SYSCLK0_CAP0HCKEN_Pos)              /*!< CLK_T::SYSCLK0: CAP0HCKEN Mask         */

#define CLK_SYSCLK0_CAP1HCKEN_Pos        (30)                                              /*!< CLK_T::SYSCLK0: CAP1HCKEN Position     */
#define CLK_SYSCLK0_CAP1HCKEN_Msk        (0x1ul << CLK_SYSCLK0_CAP1HCKEN_Pos)              /*!< CLK_T::SYSCLK0: CAP1HCKEN Mask         */

#define CLK_SYSCLK1_PDMA0CKEN_Pos        (0)                                               /*!< CLK_T::SYSCLK1: PDMA0CKEN Position     */
#define CLK_SYSCLK1_PDMA0CKEN_Msk        (0x1ul << CLK_SYSCLK1_PDMA0CKEN_Pos)              /*!< CLK_T::SYSCLK1: PDMA0CKEN Mask         */

#define CLK_SYSCLK1_PDMA1CKEN_Pos        (1)                                               /*!< CLK_T::SYSCLK1: PDMA1CKEN Position     */
#define CLK_SYSCLK1_PDMA1CKEN_Msk        (0x1ul << CLK_SYSCLK1_PDMA1CKEN_Pos)              /*!< CLK_T::SYSCLK1: PDMA1CKEN Mask         */

#define CLK_SYSCLK1_PDMA2CKEN_Pos        (2)                                               /*!< CLK_T::SYSCLK1: PDMA2CKEN Position     */
#define CLK_SYSCLK1_PDMA2CKEN_Msk        (0x1ul << CLK_SYSCLK1_PDMA2CKEN_Pos)              /*!< CLK_T::SYSCLK1: PDMA2CKEN Mask         */

#define CLK_SYSCLK1_PDMA3CKEN_Pos        (3)                                               /*!< CLK_T::SYSCLK1: PDMA3CKEN Position     */
#define CLK_SYSCLK1_PDMA3CKEN_Msk        (0x1ul << CLK_SYSCLK1_PDMA3CKEN_Pos)              /*!< CLK_T::SYSCLK1: PDMA3CKEN Mask         */

#define CLK_SYSCLK1_WH0CKEN_Pos          (4)                                               /*!< CLK_T::SYSCLK1: WH0CKEN Position       */
#define CLK_SYSCLK1_WH0CKEN_Msk          (0x1ul << CLK_SYSCLK1_WH0CKEN_Pos)                /*!< CLK_T::SYSCLK1: WH0CKEN Mask           */

#define CLK_SYSCLK1_WH1CKEN_Pos          (5)                                               /*!< CLK_T::SYSCLK1: WH1CKEN Position       */
#define CLK_SYSCLK1_WH1CKEN_Msk          (0x1ul << CLK_SYSCLK1_WH1CKEN_Pos)                /*!< CLK_T::SYSCLK1: WH1CKEN Mask           */

#define CLK_SYSCLK1_HWSCKEN_Pos          (6)                                               /*!< CLK_T::SYSCLK1: HWSCKEN Position       */
#define CLK_SYSCLK1_HWSCKEN_Msk          (0x1ul << CLK_SYSCLK1_HWSCKEN_Pos)                /*!< CLK_T::SYSCLK1: HWSCKEN Mask           */

#define CLK_SYSCLK1_EBICKEN_Pos          (7)                                               /*!< CLK_T::SYSCLK1: EBICKEN Position       */
#define CLK_SYSCLK1_EBICKEN_Msk          (0x1ul << CLK_SYSCLK1_EBICKEN_Pos)                /*!< CLK_T::SYSCLK1: EBICKEN Mask           */

#define CLK_SYSCLK1_SRAM0CKEN_Pos        (8)                                               /*!< CLK_T::SYSCLK1: SRAM0CKEN Position     */
#define CLK_SYSCLK1_SRAM0CKEN_Msk        (0x1ul << CLK_SYSCLK1_SRAM0CKEN_Pos)              /*!< CLK_T::SYSCLK1: SRAM0CKEN Mask         */

#define CLK_SYSCLK1_SRAM1CKEN_Pos        (9)                                               /*!< CLK_T::SYSCLK1: SRAM1CKEN Position     */
#define CLK_SYSCLK1_SRAM1CKEN_Msk        (0x1ul << CLK_SYSCLK1_SRAM1CKEN_Pos)              /*!< CLK_T::SYSCLK1: SRAM1CKEN Mask         */

#define CLK_SYSCLK1_ROMCKEN_Pos          (10)                                              /*!< CLK_T::SYSCLK1: ROMCKEN Position       */
#define CLK_SYSCLK1_ROMCKEN_Msk          (0x1ul << CLK_SYSCLK1_ROMCKEN_Pos)                /*!< CLK_T::SYSCLK1: ROMCKEN Mask           */

#define CLK_SYSCLK1_TRACKEN_Pos          (11)                                              /*!< CLK_T::SYSCLK1: TRACKEN Position      */
#define CLK_SYSCLK1_TRACKEN_Msk          (0x1ul << CLK_SYSCLK1_TRACKEN_Pos)                /*!< CLK_T::SYSCLK1: TRACKEN Mask          */

#define CLK_SYSCLK1_DBGCKEN_Pos          (12)                                              /*!< CLK_T::SYSCLK1: DBGCKEN Position     */
#define CLK_SYSCLK1_DBGCKEN_Msk          (0x1ul << CLK_SYSCLK1_DBGCKEN_Pos)                /*!< CLK_T::SYSCLK1: DBGCKEN Mask         */

#define CLK_SYSCLK1_CLKOCKEN_Pos         (13)                                              /*!< CLK_T::SYSCLK1: CLKOCKEN Position      */
#define CLK_SYSCLK1_CLKOCKEN_Msk         (0x1ul << CLK_SYSCLK1_CLKOCKEN_Pos)               /*!< CLK_T::SYSCLK1: CLKOCKEN Mask          */

#define CLK_SYSCLK1_GTMRCKEN_Pos         (14)                                              /*!< CLK_T::SYSCLK1: GTMRCKEN Position      */
#define CLK_SYSCLK1_GTMRCKEN_Msk         (0x1ul << CLK_SYSCLK1_GTMRCKEN_Pos)               /*!< CLK_T::SYSCLK1: GTMRCKEN Mask          */

#define CLK_SYSCLK1_GPACKEN_Pos          (16)                                              /*!< CLK_T::SYSCLK1: GPACKEN Position       */
#define CLK_SYSCLK1_GPACKEN_Msk          (0x1ul << CLK_SYSCLK1_GPACKEN_Pos)                /*!< CLK_T::SYSCLK1: GPACKEN Mask           */

#define CLK_SYSCLK1_GPBCKEN_Pos          (17)                                              /*!< CLK_T::SYSCLK1: GPBCKEN Position       */
#define CLK_SYSCLK1_GPBCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPBCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPBCKEN Mask           */

#define CLK_SYSCLK1_GPCCKEN_Pos          (18)                                              /*!< CLK_T::SYSCLK1: GPCCKEN Position       */
#define CLK_SYSCLK1_GPCCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPCCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPCCKEN Mask           */

#define CLK_SYSCLK1_GPDCKEN_Pos          (19)                                              /*!< CLK_T::SYSCLK1: GPDCKEN Position       */
#define CLK_SYSCLK1_GPDCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPDCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPDCKEN Mask           */

#define CLK_SYSCLK1_GPECKEN_Pos          (20)                                              /*!< CLK_T::SYSCLK1: GPECKEN Position       */
#define CLK_SYSCLK1_GPECKEN_Msk          (0x1ul << CLK_SYSCLK1_GPECKEN_Pos)                /*!< CLK_T::SYSCLK1: GPECKEN Mask           */

#define CLK_SYSCLK1_GPFCKEN_Pos          (21)                                              /*!< CLK_T::SYSCLK1: GPFCKEN Position       */
#define CLK_SYSCLK1_GPFCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPFCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPFCKEN Mask           */

#define CLK_SYSCLK1_GPGCKEN_Pos          (22)                                              /*!< CLK_T::SYSCLK1: GPGCKEN Position       */
#define CLK_SYSCLK1_GPGCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPGCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPGCKEN Mask           */

#define CLK_SYSCLK1_GPHCKEN_Pos          (23)                                              /*!< CLK_T::SYSCLK1: GPHCKEN Position       */
#define CLK_SYSCLK1_GPHCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPHCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPHCKEN Mask           */

#define CLK_SYSCLK1_GPICKEN_Pos          (24)                                              /*!< CLK_T::SYSCLK1: GPICKEN Position       */
#define CLK_SYSCLK1_GPICKEN_Msk          (0x1ul << CLK_SYSCLK1_GPICKEN_Pos)                /*!< CLK_T::SYSCLK1: GPICKEN Mask           */

#define CLK_SYSCLK1_GPJCKEN_Pos          (25)                                              /*!< CLK_T::SYSCLK1: GPJCKEN Position       */
#define CLK_SYSCLK1_GPJCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPJCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPJCKEN Mask           */

#define CLK_SYSCLK1_GPKCKEN_Pos          (26)                                              /*!< CLK_T::SYSCLK1: GPKCKEN Position       */
#define CLK_SYSCLK1_GPKCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPKCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPKCKEN Mask           */

#define CLK_SYSCLK1_GPLCKEN_Pos          (27)                                              /*!< CLK_T::SYSCLK1: GPLCKEN Position       */
#define CLK_SYSCLK1_GPLCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPLCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPLCKEN Mask           */

#define CLK_SYSCLK1_GPMCKEN_Pos          (28)                                              /*!< CLK_T::SYSCLK1: GPMCKEN Position       */
#define CLK_SYSCLK1_GPMCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPMCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPMCKEN Mask           */

#define CLK_SYSCLK1_GPNCKEN_Pos          (29)                                              /*!< CLK_T::SYSCLK1: GPNCKEN Position       */
#define CLK_SYSCLK1_GPNCKEN_Msk          (0x1ul << CLK_SYSCLK1_GPNCKEN_Pos)                /*!< CLK_T::SYSCLK1: GPNCKEN Mask           */


#define CLK_APBCLK0_TMR0CKEN_Pos         (0)                                               /*!< CLK_T::APBCLK0: TMR0CKEN Position      */
#define CLK_APBCLK0_TMR0CKEN_Msk         (0x1ul << CLK_APBCLK0_TMR0CKEN_Pos)               /*!< CLK_T::APBCLK0: TMR0CKEN Mask          */

#define CLK_APBCLK0_TMR1CKEN_Pos         (1)                                               /*!< CLK_T::APBCLK0: TMR1CKEN Position      */
#define CLK_APBCLK0_TMR1CKEN_Msk         (0x1ul << CLK_APBCLK0_TMR1CKEN_Pos)               /*!< CLK_T::APBCLK0: TMR1CKEN Mask          */

#define CLK_APBCLK0_TMR2CKEN_Pos         (2)                                               /*!< CLK_T::APBCLK0: TMR2CKEN Position      */
#define CLK_APBCLK0_TMR2CKEN_Msk         (0x1ul << CLK_APBCLK0_TMR2CKEN_Pos)               /*!< CLK_T::APBCLK0: TMR2CKEN Mask          */

#define CLK_APBCLK0_TMR3CKEN_Pos         (3)                                               /*!< CLK_T::APBCLK0: TMR3CKEN Position      */
#define CLK_APBCLK0_TMR3CKEN_Msk         (0x1ul << CLK_APBCLK0_TMR3CKEN_Pos)               /*!< CLK_T::APBCLK0: TMR3CKEN Mask          */

#define CLK_APBCLK0_TMR4CKEN_Pos         (4)                                               /*!< CLK_T::APBCLK0: TMR4CKEN Position      */
#define CLK_APBCLK0_TMR4CKEN_Msk         (0x1ul << CLK_APBCLK0_TMR4CKEN_Pos)               /*!< CLK_T::APBCLK0: TMR4CKEN Mask          */

#define CLK_APBCLK0_TMR5CKEN_Pos         (5)                                               /*!< CLK_T::APBCLK0: TMR5CKEN Position      */
#define CLK_APBCLK0_TMR5CKEN_Msk         (0x1ul << CLK_APBCLK0_TMR5CKEN_Pos)               /*!< CLK_T::APBCLK0: TMR5CKEN Mask          */

#define CLK_APBCLK0_TMR6CKEN_Pos         (6)                                               /*!< CLK_T::APBCLK0: TMR6CKEN Position      */
#define CLK_APBCLK0_TMR6CKEN_Msk         (0x1ul << CLK_APBCLK0_TMR6CKEN_Pos)               /*!< CLK_T::APBCLK0: TMR6CKEN Mask          */

#define CLK_APBCLK0_TMR7CKEN_Pos         (7)                                               /*!< CLK_T::APBCLK0: TMR7CKEN Position      */
#define CLK_APBCLK0_TMR7CKEN_Msk         (0x1ul << CLK_APBCLK0_TMR7CKEN_Pos)               /*!< CLK_T::APBCLK0: TMR7CKEN Mask          */

#define CLK_APBCLK0_TMR8CKEN_Pos         (8)                                               /*!< CLK_T::APBCLK0: TMR8CKEN Position      */
#define CLK_APBCLK0_TMR8CKEN_Msk         (0x1ul << CLK_APBCLK0_TMR8CKEN_Pos)               /*!< CLK_T::APBCLK0: TMR8CKEN Mask          */

#define CLK_APBCLK0_TMR9CKEN_Pos         (9)                                               /*!< CLK_T::APBCLK0: TMR9CKEN Position      */
#define CLK_APBCLK0_TMR9CKEN_Msk         (0x1ul << CLK_APBCLK0_TMR9CKEN_Pos)               /*!< CLK_T::APBCLK0: TMR9CKEN Mask          */

#define CLK_APBCLK0_TMR10CKEN_Pos        (10)                                              /*!< CLK_T::APBCLK0: TMR10CKEN Position      */
#define CLK_APBCLK0_TMR10CKEN_Msk        (0x1ul << CLK_APBCLK0_TMR10CKEN_Pos)              /*!< CLK_T::APBCLK0: TMR10CKEN Mask          */

#define CLK_APBCLK0_TMR11CKEN_Pos        (11)                                              /*!< CLK_T::APBCLK0: TMR11CKEN Position      */
#define CLK_APBCLK0_TMR11CKEN_Msk        (0x1ul << CLK_APBCLK0_TMR11CKEN_Pos)              /*!< CLK_T::APBCLK0: TMR11CKEN Mask          */

#define CLK_APBCLK0_UART0CKEN_Pos        (12)                                              /*!< CLK_T::APBCLK0: UART0CKEN Position     */
#define CLK_APBCLK0_UART0CKEN_Msk        (0x1ul << CLK_APBCLK0_UART0CKEN_Pos)              /*!< CLK_T::APBCLK0: UART0CKEN Mask         */

#define CLK_APBCLK0_UART1CKEN_Pos        (13)                                              /*!< CLK_T::APBCLK0: UART1CKEN Position     */
#define CLK_APBCLK0_UART1CKEN_Msk        (0x1ul << CLK_APBCLK0_UART1CKEN_Pos)              /*!< CLK_T::APBCLK0: UART1CKEN Mask         */

#define CLK_APBCLK0_UART2CKEN_Pos        (14)                                              /*!< CLK_T::APBCLK0: UART2CKEN Position     */
#define CLK_APBCLK0_UART2CKEN_Msk        (0x1ul << CLK_APBCLK0_UART2CKEN_Pos)              /*!< CLK_T::APBCLK0: UART2CKEN Mask         */

#define CLK_APBCLK0_UART3CKEN_Pos        (15)                                              /*!< CLK_T::APBCLK0: UART3CKEN Position     */
#define CLK_APBCLK0_UART3CKEN_Msk        (0x1ul << CLK_APBCLK0_UART3CKEN_Pos)              /*!< CLK_T::APBCLK0: UART3CKEN Mask         */

#define CLK_APBCLK0_UART4CKEN_Pos        (16)                                              /*!< CLK_T::APBCLK0: UART4CKEN Position     */
#define CLK_APBCLK0_UART4CKEN_Msk        (0x1ul << CLK_APBCLK0_UART4CKEN_Pos)              /*!< CLK_T::APBCLK0: UART4CKEN Mask         */

#define CLK_APBCLK0_UART5CKEN_Pos        (17)                                              /*!< CLK_T::APBCLK0: UART5CKEN Position     */
#define CLK_APBCLK0_UART5CKEN_Msk        (0x1ul << CLK_APBCLK0_UART5CKEN_Pos)              /*!< CLK_T::APBCLK0: UART5CKEN Mask         */

#define CLK_APBCLK0_UART6CKEN_Pos        (18)                                              /*!< CLK_T::APBCLK0: UART6CKEN Position     */
#define CLK_APBCLK0_UART6CKEN_Msk        (0x1ul << CLK_APBCLK0_UART6CKEN_Pos)              /*!< CLK_T::APBCLK0: UART6CKEN Mask         */

#define CLK_APBCLK0_UART7CKEN_Pos        (19)                                              /*!< CLK_T::APBCLK0: UART7CKEN Position     */
#define CLK_APBCLK0_UART7CKEN_Msk        (0x1ul << CLK_APBCLK0_UART7CKEN_Pos)              /*!< CLK_T::APBCLK0: UART7CKEN Mask         */

#define CLK_APBCLK0_UART8CKEN_Pos        (20)                                              /*!< CLK_T::APBCLK0: UART8CKEN Position     */
#define CLK_APBCLK0_UART8CKEN_Msk        (0x1ul << CLK_APBCLK0_UART8CKEN_Pos)              /*!< CLK_T::APBCLK0: UART8CKEN Mask         */

#define CLK_APBCLK0_UART9CKEN_Pos        (21)                                              /*!< CLK_T::APBCLK0: UART9CKEN Position     */
#define CLK_APBCLK0_UART9CKEN_Msk        (0x1ul << CLK_APBCLK0_UART9CKEN_Pos)              /*!< CLK_T::APBCLK0: UART9CKEN Mask         */

#define CLK_APBCLK0_UART10CKEN_Pos       (22)                                              /*!< CLK_T::APBCLK0: UART10CKEN Position    */
#define CLK_APBCLK0_UART10CKEN_Msk       (0x1ul << CLK_APBCLK0_UART10CKEN_Pos)             /*!< CLK_T::APBCLK0: UART10CKEN Mask        */

#define CLK_APBCLK0_UART11CKEN_Pos       (23)                                              /*!< CLK_T::APBCLK0: UART11CKEN Position    */
#define CLK_APBCLK0_UART11CKEN_Msk       (0x1ul << CLK_APBCLK0_UART11CKEN_Pos)             /*!< CLK_T::APBCLK0: UART11CKEN Mask        */

#define CLK_APBCLK0_UART12CKEN_Pos       (24)                                              /*!< CLK_T::APBCLK0: UART12CKEN Position    */
#define CLK_APBCLK0_UART12CKEN_Msk       (0x1ul << CLK_APBCLK0_UART12CKEN_Pos)             /*!< CLK_T::APBCLK0: UART12CKEN Mask        */

#define CLK_APBCLK0_UART13CKEN_Pos       (25)                                              /*!< CLK_T::APBCLK0: UART13CKEN Position    */
#define CLK_APBCLK0_UART13CKEN_Msk       (0x1ul << CLK_APBCLK0_UART13CKEN_Pos)             /*!< CLK_T::APBCLK0: UART13CKEN Mask        */

#define CLK_APBCLK0_UART14CKEN_Pos       (26)                                              /*!< CLK_T::APBCLK0: UART14CKEN Position    */
#define CLK_APBCLK0_UART14CKEN_Msk       (0x1ul << CLK_APBCLK0_UART14CKEN_Pos)             /*!< CLK_T::APBCLK0: UART14CKEN Mask        */

#define CLK_APBCLK0_UART15CKEN_Pos       (27)                                              /*!< CLK_T::APBCLK0: UART15CKEN Position    */
#define CLK_APBCLK0_UART15CKEN_Msk       (0x1ul << CLK_APBCLK0_UART15CKEN_Pos)             /*!< CLK_T::APBCLK0: UART15CKEN Mask        */

#define CLK_APBCLK0_UART16CKEN_Pos       (28)                                              /*!< CLK_T::APBCLK0: UART16CKEN Position    */
#define CLK_APBCLK0_UART16CKEN_Msk       (0x1ul << CLK_APBCLK0_UART16CKEN_Pos)             /*!< CLK_T::APBCLK0: UART16CKEN Mask        */

#define CLK_APBCLK0_RTCCKEN_Pos          (29)                                              /*!< CLK_T::APBCLK0: RTCCKEN Position       */
#define CLK_APBCLK0_RTCCKEN_Msk          (0x1ul << CLK_APBCLK0_RTCCKEN_Pos)                /*!< CLK_T::APBCLK0: RTCCKEN Mask           */

#define CLK_APBCLK0_DDRPCKEN_Pos         (30)                                              /*!< CLK_T::APBCLK0: DDRPCKEN Position      */
#define CLK_APBCLK0_DDRPCKEN_Msk         (0x1ul << CLK_APBCLK0_DDRPCKEN_Pos)               /*!< CLK_T::APBCLK0: DDRPCKEN Mask          */

#define CLK_APBCLK0_KPICKEN_Pos          (31)                                              /*!< CLK_T::APBCLK0: KPICKEN Position       */
#define CLK_APBCLK0_KPICKEN_Msk          (0x1ul << CLK_APBCLK0_KPICKEN_Pos)                /*!< CLK_T::APBCLK0: KPICKEN Mask           */

#define CLK_APBCLK1_I2C0CKEN_Pos         (0)                                               /*!< CLK_T::APBCLK1: I2C0CKEN Position      */
#define CLK_APBCLK1_I2C0CKEN_Msk         (0x1ul << CLK_APBCLK1_I2C0CKEN_Pos)               /*!< CLK_T::APBCLK1: I2C0CKEN Mask          */

#define CLK_APBCLK1_I2C1CKEN_Pos         (1)                                               /*!< CLK_T::APBCLK1: I2C1CKEN Position      */
#define CLK_APBCLK1_I2C1CKEN_Msk         (0x1ul << CLK_APBCLK1_I2C1CKEN_Pos)               /*!< CLK_T::APBCLK1: I2C1CKEN Mask          */

#define CLK_APBCLK1_I2C2CKEN_Pos         (2)                                               /*!< CLK_T::APBCLK1: I2C2CKEN Position      */
#define CLK_APBCLK1_I2C2CKEN_Msk         (0x1ul << CLK_APBCLK1_I2C2CKEN_Pos)               /*!< CLK_T::APBCLK1: I2C2CKEN Mask          */

#define CLK_APBCLK1_I2C3CKEN_Pos         (3)                                               /*!< CLK_T::APBCLK1: I2C3CKEN Position      */
#define CLK_APBCLK1_I2C3CKEN_Msk         (0x1ul << CLK_APBCLK1_I2C3CKEN_Pos)               /*!< CLK_T::APBCLK1: I2C3CKEN Mask          */

#define CLK_APBCLK1_I2C4CKEN_Pos         (4)                                               /*!< CLK_T::APBCLK1: I2C4CKEN Position      */
#define CLK_APBCLK1_I2C4CKEN_Msk         (0x1ul << CLK_APBCLK1_I2C4CKEN_Pos)               /*!< CLK_T::APBCLK1: I2C4CKEN Mask          */

#define CLK_APBCLK1_I2C5CKEN_Pos         (5)                                               /*!< CLK_T::APBCLK1: I2C5CKEN Position      */
#define CLK_APBCLK1_I2C5CKEN_Msk         (0x1ul << CLK_APBCLK1_I2C5CKEN_Pos)               /*!< CLK_T::APBCLK1: I2C5CKEN Mask          */

#define CLK_APBCLK1_QSPI0CKEN_Pos        (6)                                               /*!< CLK_T::APBCLK1: QSPI0CKEN Position     */
#define CLK_APBCLK1_QSPI0CKEN_Msk        (0x1ul << CLK_APBCLK1_QSPI0CKEN_Pos)              /*!< CLK_T::APBCLK1: QSPI0CKEN Mask         */

#define CLK_APBCLK1_QSPI1CKEN_Pos        (7)                                               /*!< CLK_T::APBCLK1: QSPI1CKEN Position     */
#define CLK_APBCLK1_QSPI1CKEN_Msk        (0x1ul << CLK_APBCLK1_QSPI1CKEN_Pos)              /*!< CLK_T::APBCLK1: QSPI1CKEN Mask         */

#define CLK_APBCLK1_CAN0CKEN_Pos         (8)                                               /*!< CLK_T::APBCLK1: CAN0CKEN Position      */
#define CLK_APBCLK1_CAN0CKEN_Msk         (0x1ul << CLK_APBCLK1_CAN0CKEN_Pos)               /*!< CLK_T::APBCLK1: CAN0CKEN Mask          */

#define CLK_APBCLK1_CAN1CKEN_Pos         (9)                                               /*!< CLK_T::APBCLK1: CAN1CKEN Position      */
#define CLK_APBCLK1_CAN1CKEN_Msk         (0x1ul << CLK_APBCLK1_CAN1CKEN_Pos)               /*!< CLK_T::APBCLK1: CAN1CKEN Mask          */

#define CLK_APBCLK1_CAN2CKEN_Pos         (10)                                              /*!< CLK_T::APBCLK1: CAN2CKEN Position      */
#define CLK_APBCLK1_CAN2CKEN_Msk         (0x1ul << CLK_APBCLK1_CAN2CKEN_Pos)               /*!< CLK_T::APBCLK1: CAN2CKEN Mask          */

#define CLK_APBCLK1_CAN3CKEN_Pos         (11)                                              /*!< CLK_T::APBCLK1: CAN3CKEN Position      */
#define CLK_APBCLK1_CAN3CKEN_Msk         (0x1ul << CLK_APBCLK1_CAN3CKEN_Pos)               /*!< CLK_T::APBCLK1: CAN3CKEN Mask          */

#define CLK_APBCLK1_SMC0CKEN_Pos         (12)                                              /*!< CLK_T::APBCLK1: SMC0CKEN Position      */
#define CLK_APBCLK1_SMC0CKEN_Msk         (0x1ul << CLK_APBCLK1_SMC0CKEN_Pos)               /*!< CLK_T::APBCLK1: SMC0CKEN Mask          */

#define CLK_APBCLK1_SMC1CKEN_Pos         (13)                                              /*!< CLK_T::APBCLK1: SMC1CKEN Position      */
#define CLK_APBCLK1_SMC1CKEN_Msk         (0x1ul << CLK_APBCLK1_SMC1CKEN_Pos)               /*!< CLK_T::APBCLK1: SMC1CKEN Mask          */

#define CLK_APBCLK1_WDT0CKEN_Pos         (16)                                              /*!< CLK_T::APBCLK1: WDT0CKEN Position      */
#define CLK_APBCLK1_WDT0CKEN_Msk         (0x1ul << CLK_APBCLK1_WDT0CKEN_Pos)               /*!< CLK_T::APBCLK1: WDT0CKEN Mask          */

#define CLK_APBCLK1_WDT1CKEN_Pos         (17)                                              /*!< CLK_T::APBCLK1: WDT1CKEN Position      */
#define CLK_APBCLK1_WDT1CKEN_Msk         (0x1ul << CLK_APBCLK1_WDT1CKEN_Pos)               /*!< CLK_T::APBCLK1: WDT1CKEN Mask          */

#define CLK_APBCLK1_WDT2CKEN_Pos         (18)                                              /*!< CLK_T::APBCLK1: WDT2CKEN Position      */
#define CLK_APBCLK1_WDT2CKEN_Msk         (0x1ul << CLK_APBCLK1_WDT2CKEN_Pos)               /*!< CLK_T::APBCLK1: WDT2CKEN Mask          */

#define CLK_APBCLK1_EPWM0CKEN_Pos        (24)                                              /*!< CLK_T::APBCLK1: EPWM0CKEN Position      */
#define CLK_APBCLK1_EPWM0CKEN_Msk        (0x1ul << CLK_APBCLK1_EPWM0CKEN_Pos)              /*!< CLK_T::APBCLK1: EPWM0CKEN Mask          */

#define CLK_APBCLK1_EPWM1CKEN_Pos        (25)                                              /*!< CLK_T::APBCLK1: EPWM1CKEN Position      */
#define CLK_APBCLK1_EPWM1CKEN_Msk        (0x1ul << CLK_APBCLK1_EPWM1CKEN_Pos)              /*!< CLK_T::APBCLK1: EPWM1CKEN Mask          */

#define CLK_APBCLK1_EPWM2CKEN_Pos        (26)                                              /*!< CLK_T::APBCLK1: EPWM2CKEN Position      */
#define CLK_APBCLK1_EPWM2CKEN_Msk        (0x1ul << CLK_APBCLK1_EPWM2CKEN_Pos)              /*!< CLK_T::APBCLK1: EPWM2CKEN Mask          */

#define CLK_APBCLK2_I2S0CKEN_Pos         (0)                                               /*!< CLK_T::APBCLK2: I2S0CKEN Position      */
#define CLK_APBCLK2_I2S0CKEN_Msk         (0x1ul << CLK_APBCLK2_I2S0CKEN_Pos)               /*!< CLK_T::APBCLK2: I2S0CKEN Mask          */

#define CLK_APBCLK2_I2S1CKEN_Pos         (1)                                               /*!< CLK_T::APBCLK2: I2S1CKEN Position      */
#define CLK_APBCLK2_I2S1CKEN_Msk         (0x1ul << CLK_APBCLK2_I2S1CKEN_Pos)               /*!< CLK_T::APBCLK2: I2S1CKEN Mask          */

#define CLK_APBCLK2_SSMCCEN_Pos          (2)                                               /*!< CLK_T::APBCLK2: SSMCCEN Position      */
#define CLK_APBCLK2_SSMCCEN_Msk          (0x1ul << CLK_APBCLK2_SSMCCEN_Pos)                /*!< CLK_T::APBCLK2: SSMCCEN Mask          */

#define CLK_APBCLK2_SSPCCEN_Pos          (3)                                               /*!< CLK_T::APBCLK2: SSPCCEN Position      */
#define CLK_APBCLK2_SSPCCEN_Msk          (0x1ul << CLK_APBCLK2_SSPCCEN_Pos)                /*!< CLK_T::APBCLK2: SSPCCEN Mask          */

#define CLK_APBCLK2_SPI0CKEN_Pos         (4)                                               /*!< CLK_T::APBCLK2: SPI0CKEN Position      */
#define CLK_APBCLK2_SPI0CKEN_Msk         (0x1ul << CLK_APBCLK2_SPI0CKEN_Pos)               /*!< CLK_T::APBCLK2: SPI0CKEN Mask          */

#define CLK_APBCLK2_SPI1CKEN_Pos         (5)                                               /*!< CLK_T::APBCLK2: SPI1CKEN Position      */
#define CLK_APBCLK2_SPI1CKEN_Msk         (0x1ul << CLK_APBCLK2_SPI1CKEN_Pos)               /*!< CLK_T::APBCLK2: SPI1CKEN Mask          */

#define CLK_APBCLK2_SPI2CKEN_Pos         (6)                                               /*!< CLK_T::APBCLK2: SPI0CKEN Position      */
#define CLK_APBCLK2_SPI2CKEN_Msk         (0x1ul << CLK_APBCLK2_SPI2CKEN_Pos)               /*!< CLK_T::APBCLK2: SPI0CKEN Mask          */

#define CLK_APBCLK2_SPI3CKEN_Pos         (7)                                               /*!< CLK_T::APBCLK2: SPI1CKEN Position      */
#define CLK_APBCLK2_SPI3CKEN_Msk         (0x1ul << CLK_APBCLK2_SPI3CKEN_Pos)               /*!< CLK_T::APBCLK2: SPI1CKEN Mask          */

#define CLK_APBCLK2_ECAP0CKEN_Pos        (8)                                               /*!< CLK_T::APBCLK2: ECAP0CKEN Position     */
#define CLK_APBCLK2_ECAP0CKEN_Msk        (0x1ul << CLK_APBCLK2_ECAP0CKEN_Pos)              /*!< CLK_T::APBCLK2: ECAP0CKEN Mask         */

#define CLK_APBCLK2_ECAP1CKEN_Pos        (9)                                               /*!< CLK_T::APBCLK2: ECAP1CKEN Position     */
#define CLK_APBCLK2_ECAP1CKEN_Msk        (0x1ul << CLK_APBCLK2_ECAP1CKEN_Pos)              /*!< CLK_T::APBCLK2: ECAP1CKEN Mask         */

#define CLK_APBCLK2_ECAP2CKEN_Pos        (10)                                              /*!< CLK_T::APBCLK2: ECAP2CKEN Position     */
#define CLK_APBCLK2_ECAP2CKEN_Msk        (0x1ul << CLK_APBCLK2_ECAP2CKEN_Pos)              /*!< CLK_T::APBCLK2: ECAP2CKEN Mask         */

#define CLK_APBCLK2_QEI0CKEN_Pos         (12)                                              /*!< CLK_T::APBCLK2: QEI0CKEN Position     */
#define CLK_APBCLK2_QEI0CKEN_Msk         (0x1ul << CLK_APBCLK2_QEI0CKEN_Pos)               /*!< CLK_T::APBCLK2: QEI0CKEN Mask         */

#define CLK_APBCLK2_QEI1CKEN_Pos         (13)                                              /*!< CLK_T::APBCLK2: QEI1CKEN Position     */
#define CLK_APBCLK2_QEI1CKEN_Msk         (0x1ul << CLK_APBCLK2_QEI1CKEN_Pos)               /*!< CLK_T::APBCLK2: QEI1CKEN Mask         */

#define CLK_APBCLK2_QEI2CKEN_Pos         (14)                                              /*!< CLK_T::APBCLK2: QEI2CKEN Position     */
#define CLK_APBCLK2_QEI2CKEN_Msk         (0x1ul << CLK_APBCLK2_QEI2CKEN_Pos)               /*!< CLK_T::APBCLK2: QEI2CKEN Mask         */

#define CLK_APBCLK2_ADCCKEN_Pos          (24)                                              /*!< CLK_T::APBCLK2: ADCCKEN Position      */
#define CLK_APBCLK2_ADCCKEN_Msk          (0x1ul << CLK_APBCLK2_ADCCKEN_Pos)                /*!< CLK_T::APBCLK2: ADCCKEN Mask          */

#define CLK_APBCLK2_EADCCKEN_Pos         (25)                                              /*!< CLK_T::APBCLK2: EADCCKEN Position     */
#define CLK_APBCLK2_EADCCKEN_Msk         (0x1ul << CLK_APBCLK2_EADCCKEN_Pos)               /*!< CLK_T::APBCLK2: EADCCKEN Mask         */

#define CLK_CLKSEL0_CA35CKSEL_Pos        (0)                                               /*!< CLK_T::CLKSEL0: CA35CKSEL Position    */
#define CLK_CLKSEL0_CA35CKSEL_Msk        (0x3ul << CLK_CLKSEL0_CA35CKSEL_Pos)              /*!< CLK_T::CLKSEL0: CA35CKSEL Mask        */

#define CLK_CLKSEL0_SYSCK0SEL_Pos        (2)                                               /*!< CLK_T::CLKSEL0: SYSCK0SEL Position    */
#define CLK_CLKSEL0_SYSCK0SEL_Msk        (0x1ul << CLK_CLKSEL0_SYSCK0SEL_Pos)              /*!< CLK_T::CLKSEL0: SYSCK0SEL Mask        */

#define CLK_CLKSEL0_LVRDBSEL_Pos         (3)                                               /*!< CLK_T::CLKSEL0: LVRDBSEL Position    */
#define CLK_CLKSEL0_LVRDBSEL_Msk         (0x1ul << CLK_CLKSEL0_LVRDBSEL_Pos)               /*!< CLK_T::CLKSEL0: LVRDBSEL Mask        */

#define CLK_CLKSEL0_SYSCK1SEL_Pos        (4)                                               /*!< CLK_T::CLKSEL0: SYSCK1SEL Position    */
#define CLK_CLKSEL0_SYSCK1SEL_Msk        (0x3ul << CLK_CLKSEL0_SYSCK1SEL_Pos)              /*!< CLK_T::CLKSEL0: SYSCK1SEL Mask        */

#define CLK_CLKSEL0_M4STSEL_Pos          (8)                                               /*!< CLK_T::CLKSEL0: M4STSEL Position      */
#define CLK_CLKSEL0_M4STSEL_Msk          (0x7ul << CLK_CLKSEL0_M4STSEL_Pos)                /*!< CLK_T::CLKSEL0: M4STSEL Mask          */

#define CLK_CLKSEL0_CCAP0SEL_Pos         (12)                                              /*!< CLK_T::CLKSEL0: CCAP0SEL Position     */
#define CLK_CLKSEL0_CCAP0SEL_Msk         (0x3ul << CLK_CLKSEL0_CCAP0SEL_Pos)               /*!< CLK_T::CLKSEL0: CCAP0SEL Mask         */

#define CLK_CLKSEL0_CCAP1SEL_Pos         (14)                                              /*!< CLK_T::CLKSEL0: CCAP1SEL Position     */
#define CLK_CLKSEL0_CCAP1SEL_Msk         (0x3ul << CLK_CLKSEL0_CCAP1SEL_Pos)               /*!< CLK_T::CLKSEL0: CCAP1SEL Mask         */

#define CLK_CLKSEL0_SD0SEL_Pos           (16)                                              /*!< CLK_T::CLKSEL0: SD0SEL Position       */
#define CLK_CLKSEL0_SD0SEL_Msk           (0x3ul << CLK_CLKSEL0_SD0SEL_Pos)                 /*!< CLK_T::CLKSEL0: SD0SEL Mask           */

#define CLK_CLKSEL0_SD1SEL_Pos           (18)                                              /*!< CLK_T::CLKSEL0: SD1SEL Position       */
#define CLK_CLKSEL0_SD1SEL_Msk           (0x3ul << CLK_CLKSEL0_SD1SEL_Pos)                 /*!< CLK_T::CLKSEL0: SD1SEL Mask           */

#define CLK_CLKSEL0_DCUSEL_Pos           (24)                                              /*!< CLK_T::CLKSEL0: DCUSEL Position       */
#define CLK_CLKSEL0_DCUSEL_Msk           (0x1ul << CLK_CLKSEL0_DCUSEL_Pos)                 /*!< CLK_T::CLKSEL0: DCUSEL Mask           */

#define CLK_CLKSEL0_DCUPSEL_Pos          (25)                                              /*!< CLK_T::CLKSEL0: DCUPSEL Position       */
#define CLK_CLKSEL0_DCUPSEL_Msk          (0x1ul << CLK_CLKSEL0_DCUPSEL_Pos)                /*!< CLK_T::CLKSEL0: DCUPSEL Mask           */

#define CLK_CLKSEL0_GFXSEL_Pos           (26)                                              /*!< CLK_T::CLKSEL0: GFXSEL Position       */
#define CLK_CLKSEL0_GFXSEL_Msk           (0x1ul << CLK_CLKSEL0_GFXSEL_Pos)                 /*!< CLK_T::CLKSEL0: GFXSEL Mask           */

#define CLK_CLKSEL1_TMR0SEL_Pos          (0)                                               /*!< CLK_T::CLKSEL1: TMR0SEL Position       */
#define CLK_CLKSEL1_TMR0SEL_Msk          (0x7ul << CLK_CLKSEL1_TMR0SEL_Pos)                /*!< CLK_T::CLKSEL1: TMR0SEL Mask           */

#define CLK_CLKSEL1_TMR1SEL_Pos          (4)                                               /*!< CLK_T::CLKSEL1: TMR1SEL Position       */
#define CLK_CLKSEL1_TMR1SEL_Msk          (0x7ul << CLK_CLKSEL1_TMR1SEL_Pos)                /*!< CLK_T::CLKSEL1: TMR1SEL Mask           */

#define CLK_CLKSEL1_TMR2SEL_Pos          (8)                                               /*!< CLK_T::CLKSEL1: TMR2SEL Position       */
#define CLK_CLKSEL1_TMR2SEL_Msk          (0x7ul << CLK_CLKSEL1_TMR2SEL_Pos)                /*!< CLK_T::CLKSEL1: TMR2SEL Mask           */

#define CLK_CLKSEL1_TMR3SEL_Pos          (12)                                              /*!< CLK_T::CLKSEL1: TMR3SEL Position       */
#define CLK_CLKSEL1_TMR3SEL_Msk          (0x7ul << CLK_CLKSEL1_TMR3SEL_Pos)                /*!< CLK_T::CLKSEL1: TMR3SEL Mask           */

#define CLK_CLKSEL1_TMR4SEL_Pos          (16)                                              /*!< CLK_T::CLKSEL1: TMR4SEL Position       */
#define CLK_CLKSEL1_TMR4SEL_Msk          (0x7ul << CLK_CLKSEL1_TMR4SEL_Pos)                /*!< CLK_T::CLKSEL1: TMR4SEL Mask           */

#define CLK_CLKSEL1_TMR5SEL_Pos          (20)                                              /*!< CLK_T::CLKSEL1: TMR5SEL Position       */
#define CLK_CLKSEL1_TMR5SEL_Msk          (0x7ul << CLK_CLKSEL1_TMR5SEL_Pos)                /*!< CLK_T::CLKSEL1: TMR5SEL Mask           */

#define CLK_CLKSEL1_TMR6SEL_Pos          (24)                                              /*!< CLK_T::CLKSEL1: TMR6SEL Position       */
#define CLK_CLKSEL1_TMR6SEL_Msk          (0x7ul << CLK_CLKSEL1_TMR6SEL_Pos)                /*!< CLK_T::CLKSEL1: TMR6SEL Mask           */

#define CLK_CLKSEL1_TMR7SEL_Pos          (28)                                              /*!< CLK_T::CLKSEL1: TMR7SEL Position       */
#define CLK_CLKSEL1_TMR7SEL_Msk          (0x7ul << CLK_CLKSEL1_TMR7SEL_Pos)                /*!< CLK_T::CLKSEL1: TMR7SEL Mask           */

#define CLK_CLKSEL2_TMR8SEL_Pos          (0)                                               /*!< CLK_T::CLKSEL2: TMR8SEL Position       */
#define CLK_CLKSEL2_TMR8SEL_Msk          (0x7ul << CLK_CLKSEL2_TMR8SEL_Pos)                /*!< CLK_T::CLKSEL2: TMR8SEL Mask           */

#define CLK_CLKSEL2_TMR9SEL_Pos          (4)                                               /*!< CLK_T::CLKSEL2: TMR9SEL Position       */
#define CLK_CLKSEL2_TMR9SEL_Msk          (0x7ul << CLK_CLKSEL2_TMR9SEL_Pos)                /*!< CLK_T::CLKSEL2: TMR9SEL Mask           */

#define CLK_CLKSEL2_TMR10SEL_Pos         (8)                                               /*!< CLK_T::CLKSEL2: TMR10SEL Position      */
#define CLK_CLKSEL2_TMR10SEL_Msk         (0x7ul << CLK_CLKSEL2_TMR10SEL_Pos)               /*!< CLK_T::CLKSEL2: TMR10SEL Mask          */

#define CLK_CLKSEL2_TMR11SEL_Pos         (12)                                              /*!< CLK_T::CLKSEL2: TMR11SEL Position      */
#define CLK_CLKSEL2_TMR11SEL_Msk         (0x7ul << CLK_CLKSEL2_TMR11SEL_Pos)               /*!< CLK_T::CLKSEL2: TMR11SEL Mask          */

#define CLK_CLKSEL2_UART0SEL_Pos         (16)                                              /*!< CLK_T::CLKSEL2: UART0SEL Position      */
#define CLK_CLKSEL2_UART0SEL_Msk         (0x3ul << CLK_CLKSEL2_UART0SEL_Pos)               /*!< CLK_T::CLKSEL2: UART0SEL Mask          */

#define CLK_CLKSEL2_UART1SEL_Pos         (18)                                              /*!< CLK_T::CLKSEL2: UART1SEL Position      */
#define CLK_CLKSEL2_UART1SEL_Msk         (0x3ul << CLK_CLKSEL2_UART1SEL_Pos)               /*!< CLK_T::CLKSEL2: UART1SEL Mask          */

#define CLK_CLKSEL2_UART2SEL_Pos         (20)                                              /*!< CLK_T::CLKSEL2: UART2SEL Position      */
#define CLK_CLKSEL2_UART2SEL_Msk         (0x3ul << CLK_CLKSEL2_UART2SEL_Pos)               /*!< CLK_T::CLKSEL2: UART2SEL Mask          */

#define CLK_CLKSEL2_UART3SEL_Pos         (22)                                              /*!< CLK_T::CLKSEL2: UART3SEL Position      */
#define CLK_CLKSEL2_UART3SEL_Msk         (0x3ul << CLK_CLKSEL2_UART3SEL_Pos)               /*!< CLK_T::CLKSEL2: UART3SEL Mask          */

#define CLK_CLKSEL2_UART4SEL_Pos         (24)                                              /*!< CLK_T::CLKSEL2: UART4SEL Position      */
#define CLK_CLKSEL2_UART4SEL_Msk         (0x3ul << CLK_CLKSEL2_UART4SEL_Pos)               /*!< CLK_T::CLKSEL2: UART4SEL Mask          */

#define CLK_CLKSEL2_UART5SEL_Pos         (26)                                              /*!< CLK_T::CLKSEL2: UART5SEL Position      */
#define CLK_CLKSEL2_UART5SEL_Msk         (0x3ul << CLK_CLKSEL2_UART5SEL_Pos)               /*!< CLK_T::CLKSEL2: UART5SEL Mask          */

#define CLK_CLKSEL2_UART6SEL_Pos         (28)                                              /*!< CLK_T::CLKSEL2: UART6SEL Position      */
#define CLK_CLKSEL2_UART6SEL_Msk         (0x3ul << CLK_CLKSEL2_UART6SEL_Pos)               /*!< CLK_T::CLKSEL2: UART6SEL Mask          */

#define CLK_CLKSEL2_UART7SEL_Pos         (30)                                              /*!< CLK_T::CLKSEL2: UART7SEL Position      */
#define CLK_CLKSEL2_UART7SEL_Msk         (0x3ul << CLK_CLKSEL2_UART7SEL_Pos)               /*!< CLK_T::CLKSEL2: UART7SEL Mask          */

#define CLK_CLKSEL3_UART8SEL_Pos         (0)                                               /*!< CLK_T::CLKSEL3: UART8SEL Position      */
#define CLK_CLKSEL3_UART8SEL_Msk         (0x3ul << CLK_CLKSEL3_UART8SEL_Pos)               /*!< CLK_T::CLKSEL3: UART8SEL Mask          */

#define CLK_CLKSEL3_UART9SEL_Pos         (2)                                               /*!< CLK_T::CLKSEL3: UART9SEL Position      */
#define CLK_CLKSEL3_UART9SEL_Msk         (0x3ul << CLK_CLKSEL3_UART9SEL_Pos)               /*!< CLK_T::CLKSEL3: UART9SEL Mask          */

#define CLK_CLKSEL3_UART10SEL_Pos        (4)                                               /*!< CLK_T::CLKSEL3: UART10SEL Position     */
#define CLK_CLKSEL3_UART10SEL_Msk        (0x3ul << CLK_CLKSEL3_UART10SEL_Pos)              /*!< CLK_T::CLKSEL3: UART10SEL Mask         */

#define CLK_CLKSEL3_UART11SEL_Pos        (6)                                               /*!< CLK_T::CLKSEL3: UART11SEL Position     */
#define CLK_CLKSEL3_UART11SEL_Msk        (0x3ul << CLK_CLKSEL3_UART11SEL_Pos)              /*!< CLK_T::CLKSEL3: UART11SEL Mask         */

#define CLK_CLKSEL3_UART12SEL_Pos        (8)                                               /*!< CLK_T::CLKSEL3: UART12SEL Position     */
#define CLK_CLKSEL3_UART12SEL_Msk        (0x3ul << CLK_CLKSEL3_UART12SEL_Pos)              /*!< CLK_T::CLKSEL3: UART12SEL Mask         */

#define CLK_CLKSEL3_UART13SEL_Pos        (10)                                              /*!< CLK_T::CLKSEL3: UART13SEL Position     */
#define CLK_CLKSEL3_UART13SEL_Msk        (0x3ul << CLK_CLKSEL3_UART13SEL_Pos)              /*!< CLK_T::CLKSEL3: UART13SEL Mask         */

#define CLK_CLKSEL3_UART14SEL_Pos        (12)                                              /*!< CLK_T::CLKSEL3: UART14SEL Position     */
#define CLK_CLKSEL3_UART14SEL_Msk        (0x3ul << CLK_CLKSEL3_UART14SEL_Pos)              /*!< CLK_T::CLKSEL3: UART14SEL Mask         */

#define CLK_CLKSEL3_UART15SEL_Pos        (14)                                              /*!< CLK_T::CLKSEL3: UART15SEL Position     */
#define CLK_CLKSEL3_UART15SEL_Msk        (0x3ul << CLK_CLKSEL3_UART15SEL_Pos)              /*!< CLK_T::CLKSEL3: UART15SEL Mask         */

#define CLK_CLKSEL3_UART16SEL_Pos        (16)                                              /*!< CLK_T::CLKSEL3: UART16SEL Position     */
#define CLK_CLKSEL3_UART16SEL_Msk        (0x3ul << CLK_CLKSEL3_UART16SEL_Pos)              /*!< CLK_T::CLKSEL3: UART16SEL Mask         */

#define CLK_CLKSEL3_WDT0SEL_Pos          (20)                                              /*!< CLK_T::CLKSEL3: WDT0SEL Position       */
#define CLK_CLKSEL3_WDT0SEL_Msk          (0x3ul << CLK_CLKSEL3_WDT0SEL_Pos)                /*!< CLK_T::CLKSEL3: WDT0SEL Mask           */

#define CLK_CLKSEL3_WWDT0SEL_Pos         (22)                                              /*!< CLK_T::CLKSEL3: WWDT0SEL Position      */
#define CLK_CLKSEL3_WWDT0SEL_Msk         (0x3ul << CLK_CLKSEL3_WWDT0SEL_Pos)               /*!< CLK_T::CLKSEL3: WWDT0SEL Mask          */

#define CLK_CLKSEL3_WDT1SEL_Pos          (24)                                              /*!< CLK_T::CLKSEL3: WDT1SEL Position       */
#define CLK_CLKSEL3_WDT1SEL_Msk          (0x3ul << CLK_CLKSEL3_WDT1SEL_Pos)                /*!< CLK_T::CLKSEL3: WDT1SEL Mask           */

#define CLK_CLKSEL3_WWDT1SEL_Pos         (26)                                              /*!< CLK_T::CLKSEL3: WWDT1SEL Position      */
#define CLK_CLKSEL3_WWDT1SEL_Msk         (0x3ul << CLK_CLKSEL3_WWDT1SEL_Pos)               /*!< CLK_T::CLKSEL3: WWDT1SEL Mask          */

#define CLK_CLKSEL3_WDT2SEL_Pos          (28)                                              /*!< CLK_T::CLKSEL3: WDT2SEL Position       */
#define CLK_CLKSEL3_WDT2SEL_Msk          (0x3ul << CLK_CLKSEL3_WDT2SEL_Pos)                /*!< CLK_T::CLKSEL3: WDT2SEL Mask           */

#define CLK_CLKSEL3_WWDT2SEL_Pos         (30)                                              /*!< CLK_T::CLKSEL3: WWDT2SEL Position      */
#define CLK_CLKSEL3_WWDT2SEL_Msk         (0x3ul << CLK_CLKSEL3_WWDT2SEL_Pos)               /*!< CLK_T::CLKSEL3: WWDT2SEL Mask          */

#define CLK_CLKSEL4_SPI0SEL_Pos          (0)                                               /*!< CLK_T::CLKSEL4: SPI0SEL Position       */
#define CLK_CLKSEL4_SPI0SEL_Msk          (0x3ul << CLK_CLKSEL4_SPI0SEL_Pos)                /*!< CLK_T::CLKSEL4: SPI0SEL Mask           */

#define CLK_CLKSEL4_SPI1SEL_Pos          (2)                                               /*!< CLK_T::CLKSEL4: SPI1SEL Position       */
#define CLK_CLKSEL4_SPI1SEL_Msk          (0x3ul << CLK_CLKSEL4_SPI1SEL_Pos)                /*!< CLK_T::CLKSEL4: SPI1SEL Mask           */

#define CLK_CLKSEL4_SPI2SEL_Pos          (4)                                               /*!< CLK_T::CLKSEL4: SPI2SEL Position       */
#define CLK_CLKSEL4_SPI2SEL_Msk          (0x3ul << CLK_CLKSEL4_SPI2SEL_Pos)                /*!< CLK_T::CLKSEL4: SPI2SEL Mask           */

#define CLK_CLKSEL4_SPI3SEL_Pos          (6)                                               /*!< CLK_T::CLKSEL4: SPI3SEL Position       */
#define CLK_CLKSEL4_SPI3SEL_Msk          (0x3ul << CLK_CLKSEL4_SPI3SEL_Pos)                /*!< CLK_T::CLKSEL4: SPI3SEL Mask           */

#define CLK_CLKSEL4_QSPI0SEL_Pos         (8)                                               /*!< CLK_T::CLKSEL4: QSPI0SEL Position      */
#define CLK_CLKSEL4_QSPI0SEL_Msk         (0x3ul << CLK_CLKSEL4_QSPI0SEL_Pos)               /*!< CLK_T::CLKSEL4: QSPI0SEL Mask          */

#define CLK_CLKSEL4_QSPI1SEL_Pos         (10)                                              /*!< CLK_T::CLKSEL4: QSPI1SEL Position      */
#define CLK_CLKSEL4_QSPI1SEL_Msk         (0x3ul << CLK_CLKSEL4_QSPI1SEL_Pos)               /*!< CLK_T::CLKSEL4: QSPI1SEL Mask          */

#define CLK_CLKSEL4_I2S0SEL_Pos          (12)                                              /*!< CLK_T::CLKSEL4: I2S0SEL Position       */
#define CLK_CLKSEL4_I2S0SEL_Msk          (0x3ul << CLK_CLKSEL4_I2S0SEL_Pos)                /*!< CLK_T::CLKSEL4: I2S0SEL Mask           */

#define CLK_CLKSEL4_I2S1SEL_Pos          (14)                                              /*!< CLK_T::CLKSEL4: I2S1SEL Position       */
#define CLK_CLKSEL4_I2S1SEL_Msk          (0x3ul << CLK_CLKSEL4_I2S1SEL_Pos)                /*!< CLK_T::CLKSEL4: I2S1SEL Mask           */

#define CLK_CLKSEL4_CAN0SEL_Pos          (16)                                              /*!< CLK_T::CLKSEL4: CAN0SEL Position       */
#define CLK_CLKSEL4_CAN0SEL_Msk          (0x1ul << CLK_CLKSEL4_CAN0SEL_Pos)                /*!< CLK_T::CLKSEL4: CAN0SEL Mask           */

#define CLK_CLKSEL4_CAN1SEL_Pos          (17)                                              /*!< CLK_T::CLKSEL4: CAN1SEL Position       */
#define CLK_CLKSEL4_CAN1SEL_Msk          (0x1ul << CLK_CLKSEL4_CAN1SEL_Pos)                /*!< CLK_T::CLKSEL4: CAN1SEL Mask           */

#define CLK_CLKSEL4_CAN2SEL_Pos          (18)                                              /*!< CLK_T::CLKSEL4: CAN2SEL Position       */
#define CLK_CLKSEL4_CAN2SEL_Msk          (0x1ul << CLK_CLKSEL4_CAN2SEL_Pos)                /*!< CLK_T::CLKSEL4: CAN2SEL Mask           */

#define CLK_CLKSEL4_CAN3SEL_Pos          (19)                                              /*!< CLK_T::CLKSEL4: CAN3SEL Position       */
#define CLK_CLKSEL4_CAN3SEL_Msk          (0x1ul << CLK_CLKSEL4_CAN3SEL_Pos)                /*!< CLK_T::CLKSEL4: CAN3SEL Mask           */

#define CLK_CLKSEL4_CLKOSEL_Pos          (24)                                              /*!< CLK_T::CLKSEL4: CLKOSEL Position       */
#define CLK_CLKSEL4_CLKOSEL_Msk          (0xFul << CLK_CLKSEL4_CLKOSEL_Pos)                /*!< CLK_T::CLKSEL4: CLKOSEL Mask           */

#define CLK_CLKSEL4_SC0SEL_Pos           (28)                                              /*!< CLK_T::CLKSEL4: SC0SEL Position        */
#define CLK_CLKSEL4_SC0SEL_Msk           (0x1ul << CLK_CLKSEL4_SC0SEL_Pos)                 /*!< CLK_T::CLKSEL4: SC0SEL Mask            */

#define CLK_CLKSEL4_SC1SEL_Pos           (29)                                              /*!< CLK_T::CLKSEL4: SC1SEL Position        */
#define CLK_CLKSEL4_SC1SEL_Msk           (0x1ul << CLK_CLKSEL4_SC1SEL_Pos)                 /*!< CLK_T::CLKSEL4: SC1SEL Mask            */

#define CLK_CLKSEL4_KPISEL_Pos           (30)                                              /*!< CLK_T::CLKSEL4: KPISEL Position        */
#define CLK_CLKSEL4_KPISEL_Msk           (0x1ul << CLK_CLKSEL4_KPISEL_Pos)                 /*!< CLK_T::CLKSEL4: KPISEL Mask            */

#define CLK_CLKDIV0_SDH0DIV_Pos          (0)                                               /*!< CLK_T::CLKDIV0: SDH0DIV Position       */
#define CLK_CLKDIV0_SDH0DIV_Msk          (0x7ul << CLK_CLKDIV0_SDH0DIV_Pos)                /*!< CLK_T::CLKDIV0: SDH0DIV Mask           */

#define CLK_CLKDIV0_SDH1DIV_Pos          (8)                                               /*!< CLK_T::CLKDIV0: SDH1DIV Position       */
#define CLK_CLKDIV0_SDH1DIV_Msk          (0x7ul << CLK_CLKDIV0_SDH1DIV_Pos)                /*!< CLK_T::CLKDIV0: SDH1DIV Mask           */

#define CLK_CLKDIV0_DCUPDIV_Pos          (24)                                              /*!< CLK_T::CLKDIV0: DCUPDIV Position       */
#define CLK_CLKDIV0_DCUPDIV_Msk          (0x3ul << CLK_CLKDIV0_DCUPDIV_Pos)                /*!< CLK_T::CLKDIV0: DCUPDIV Mask           */

#define CLK_CLKDIV0_EMAC0DIV_Pos         (28)                                              /*!< CLK_T::CLKDIV0: EMAC0DIV Position     */
#define CLK_CLKDIV0_EMAC0DIV_Msk         (0x3ul << CLK_CLKDIV0_EMAC0DIV_Pos)               /*!< CLK_T::CLKDIV0: EMAC0DIV Mask         */

#define CLK_CLKDIV0_EMAC1DIV_Pos         (30)                                              /*!< CLK_T::CLKDIV0: EMAC1DIV Position     */
#define CLK_CLKDIV0_EMAC1DIV_Msk         (0x3ul << CLK_CLKDIV0_EMAC1DIV_Pos)               /*!< CLK_T::CLKDIV0: EMAC1DIV Mask         */

#define CLK_CLKDIV0_ACLK0DIV_Pos         (26)                                              /*!< CLK_T::CLKDIV0: ACLK0DIV Position       */
#define CLK_CLKDIV0_ACLK0DIV_Msk         (0x1ul << CLK_CLKDIV0_ACLK0DIV_Pos)               /*!< CLK_T::CLKDIV0: ACLK0DIV Mask           */

#define CLK_CLKDIV1_SC0DIV_Pos           (0)                                               /*!< CLK_T::CLKDIV1: SC0DIV Position        */
#define CLK_CLKDIV1_SC0DIV_Msk           (0xful << CLK_CLKDIV1_SC0DIV_Pos)                 /*!< CLK_T::CLKDIV1: SC0DIV Mask            */

#define CLK_CLKDIV1_SC1DIV_Pos           (4)                                               /*!< CLK_T::CLKDIV1: SC1DIV Position        */
#define CLK_CLKDIV1_SC1DIV_Msk           (0xful << CLK_CLKDIV1_SC1DIV_Pos)                 /*!< CLK_T::CLKDIV1: SC1DIV Mask            */

#define CLK_CLKDIV1_CCAP0DIV_Pos         (8)                                               /*!< CLK_T::CLKDIV1: CCAP0DIV Position      */
#define CLK_CLKDIV1_CCAP0DIV_Msk         (0xfful << CLK_CLKDIV1_CCAP0DIV_Pos)              /*!< CLK_T::CLKDIV1: CCAP0DIV Mask          */

#define CLK_CLKDIV1_CCAP1DIV_Pos         (12)                                              /*!< CLK_T::CLKDIV1: CCAP1DIV Position      */
#define CLK_CLKDIV1_CCAP1DIV_Msk         (0xfful << CLK_CLKDIV1_CCAP1DIV_Pos)              /*!< CLK_T::CLKDIV1: CCAP1DIV Mask          */

#define CLK_CLKDIV1_UART0DIV_Pos         (16)                                              /*!< CLK_T::CLKDIV1: UART0DIV Position      */
#define CLK_CLKDIV1_UART0DIV_Msk         (0xful << CLK_CLKDIV1_UART0DIV_Pos)               /*!< CLK_T::CLKDIV1: UART0DIV Mask          */

#define CLK_CLKDIV1_UART1DIV_Pos         (20)                                              /*!< CLK_T::CLKDIV1: UART1DIV Position      */
#define CLK_CLKDIV1_UART1DIV_Msk         (0xful << CLK_CLKDIV1_UART1DIV_Pos)               /*!< CLK_T::CLKDIV1: UART1DIV Mask          */

#define CLK_CLKDIV1_UART2DIV_Pos         (24)                                              /*!< CLK_T::CLKDIV1: UART2DIV Position      */
#define CLK_CLKDIV1_UART2DIV_Msk         (0xful << CLK_CLKDIV1_UART2DIV_Pos)               /*!< CLK_T::CLKDIV1: UART2DIV Mask          */

#define CLK_CLKDIV1_UART3DIV_Pos         (28)                                              /*!< CLK_T::CLKDIV1: UART3DIV Position      */
#define CLK_CLKDIV1_UART3DIV_Msk         (0xful << CLK_CLKDIV1_UART3DIV_Pos)               /*!< CLK_T::CLKDIV1: UART3DIV Mask          */


#define CLK_CLKDIV2_UART4DIV_Pos         (0)                                               /*!< CLK_T::CLKDIV2: UART4DIV Position      */
#define CLK_CLKDIV2_UART4DIV_Msk         (0xful << CLK_CLKDIV2_UART4DIV_Pos)               /*!< CLK_T::CLKDIV2: UART4DIV Mask          */

#define CLK_CLKDIV2_UART5DIV_Pos         (4)                                               /*!< CLK_T::CLKDIV2: UART5DIV Position      */
#define CLK_CLKDIV2_UART5DIV_Msk         (0xful << CLK_CLKDIV2_UART5DIV_Pos)               /*!< CLK_T::CLKDIV2: UART5DIV Mask          */

#define CLK_CLKDIV2_UART6DIV_Pos         (8)                                               /*!< CLK_T::CLKDIV2: UART6DIV Position      */
#define CLK_CLKDIV2_UART6DIV_Msk         (0xful << CLK_CLKDIV2_UART6DIV_Pos)               /*!< CLK_T::CLKDIV2: UART6DIV Mask          */

#define CLK_CLKDIV2_UART7DIV_Pos         (12)                                              /*!< CLK_T::CLKDIV2: UART7DIV Position      */
#define CLK_CLKDIV2_UART7DIV_Msk         (0xful << CLK_CLKDIV2_UART7DIV_Pos)               /*!< CLK_T::CLKDIV2: UART7DIV Mask          */

#define CLK_CLKDIV2_UART8DIV_Pos         (16)                                              /*!< CLK_T::CLKDIV2: UART8DIV Position      */
#define CLK_CLKDIV2_UART8DIV_Msk         (0xful << CLK_CLKDIV2_UART8DIV_Pos)               /*!< CLK_T::CLKDIV2: UART8DIV Mask          */

#define CLK_CLKDIV2_UART9DIV_Pos         (20)                                              /*!< CLK_T::CLKDIV2: UART9DIV Position      */
#define CLK_CLKDIV2_UART9DIV_Msk         (0xful << CLK_CLKDIV2_UART9DIV_Pos)               /*!< CLK_T::CLKDIV2: UART9DIV Mask          */

#define CLK_CLKDIV2_UART10DIV_Pos        (24)                                              /*!< CLK_T::CLKDIV2: UART10DIV Position     */
#define CLK_CLKDIV2_UART10DIV_Msk        (0xful << CLK_CLKDIV2_UART10DIV_Pos)              /*!< CLK_T::CLKDIV2: UART10DIV Mask         */

#define CLK_CLKDIV2_UART11DIV_Pos        (28)                                              /*!< CLK_T::CLKDIV2: UART11DIV Position     */
#define CLK_CLKDIV2_UART11DIV_Msk        (0xful << CLK_CLKDIV2_UART11DIV_Pos)              /*!< CLK_T::CLKDIV2: UART11DIV Mask         */

#define CLK_CLKDIV3_UART12DIV_Pos        (0)                                               /*!< CLK_T::CLKDIV3: UART12DIV Position      */
#define CLK_CLKDIV3_UART12DIV_Msk        (0xful << CLK_CLKDIV3_UART12DIV_Pos)              /*!< CLK_T::CLKDIV3: UART12DIV Mask          */

#define CLK_CLKDIV3_UART13DIV_Pos        (4)                                               /*!< CLK_T::CLKDIV3: UART13DIV Position      */
#define CLK_CLKDIV3_UART13DIV_Msk        (0xful << CLK_CLKDIV3_UART13DIV_Pos)              /*!< CLK_T::CLKDIV3: UART13DIV Mask          */

#define CLK_CLKDIV3_UART14DIV_Pos        (8)                                               /*!< CLK_T::CLKDIV3: UART14DIV Position      */
#define CLK_CLKDIV3_UART14DIV_Msk        (0xful << CLK_CLKDIV3_UART14DIV_Pos)              /*!< CLK_T::CLKDIV3: UART14DIV Mask          */

#define CLK_CLKDIV3_UART15DIV_Pos        (12)                                              /*!< CLK_T::CLKDIV3: UART15DIV Position      */
#define CLK_CLKDIV3_UART15DIV_Msk        (0xful << CLK_CLKDIV3_UART15DIV_Pos)              /*!< CLK_T::CLKDIV3: UART15DIV Mask          */

#define CLK_CLKDIV3_UART16DIV_Pos        (16)                                              /*!< CLK_T::CLKDIV3: UART16DIV Position      */
#define CLK_CLKDIV3_UART16DIV_Msk        (0xful << CLK_CLKDIV3_UART16DIV_Pos)              /*!< CLK_T::CLKDIV3: UART16DIV Mask          */

#define CLK_CLKDIV3_TRACEDIV_Pos         (24)                                              /*!< CLK_T::CLKDIV3: TRACEDIV Position      */
#define CLK_CLKDIV3_TRACEDIV_Msk         (0xful << CLK_CLKDIV3_TRACEDIV_Pos)               /*!< CLK_T::CLKDIV3: TRACEDIV Mask          */

#define CLK_CLKDIV3_DBGDIV_Pos           (28)                                              /*!< CLK_T::CLKDIV3: DBGDIV Position      */
#define CLK_CLKDIV3_DBGDIV_Msk           (0xful << CLK_CLKDIV3_DBGDIV_Pos)                 /*!< CLK_T::CLKDIV3: DBGDIV Mask          */

#define CLK_CLKDIV4_EADCDIV_Pos          (0)                                               /*!< CLK_T::CLKDIV4: EADCDIV Position        */
#define CLK_CLKDIV4_EADCDIV_Msk          (0xful << CLK_CLKDIV4_EADCDIV_Pos)                /*!< CLK_T::CLKDIV4: EADCDIV Mask            */

#define CLK_CLKDIV4_ADCDIV_Pos           (4)                                               /*!< CLK_T::CLKDIV4: ADCDIV Position         */
#define CLK_CLKDIV4_ADCDIV_Msk           (0x1fffful << CLK_CLKDIV4_ADCDIV_Pos)             /*!< CLK_T::CLKDIV4: ADCDIV Mask             */

#define CLK_CLKDIV4_KPIDIV_Pos           (24)                                              /*!< CLK_T::CLKDIV4: KPIDIV Position         */
#define CLK_CLKDIV4_KPIDIV_Msk           (0xfful << CLK_CLKDIV4_KPIDIV_Pos)                /*!< CLK_T::CLKDIV4: KPIDIV Mask             */

#endif /*  __DRIVERS_CLK_NUVOTON_REGS_MA35D1_CLOCK_H */
