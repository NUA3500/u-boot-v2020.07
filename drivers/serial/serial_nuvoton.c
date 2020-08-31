// SPDX-License-Identifier: GPL-2.0
/*
 * Nuvoton UART driver
 *
 * Copyright (C) 2020 Nuvoton Technology Corporation
 */

#include <clk.h>
#include <common.h>
#include <div64.h>
#include <dm.h>
#include <errno.h>
#include <log.h>
#include <serial.h>
#include <watchdog.h>
#include <asm/io.h>
#include <asm/types.h>
#include <linux/err.h>


DECLARE_GLOBAL_DATA_PTR;

void nua3500_serial_initialize(void);
int nua3500_serial_init (void);
void nua3500_serial_putc (const char ch);
void nua3500_serial_puts (const char *s);
int nua3500_serial_getc (void);
int nua3500_serial_tstc (void);
void nua3500_serial_setbrg (void);

#define TX_RX_FIFO_RESET    0x06
#define ENABLE_DLAB         0x80    // Enable Divider Latch Access
#define DISABLE_DLAB        0x7F
#define ENABLE_TIME_OUT     (0x80+0x20)
#define THR_EMPTY           0x20    // Transmit Holding Register Empty
#define RX_DATA_READY       0x01
#define RX_FIFO_EMPTY       0x4000

#define REG_COM_MSR    (0x18)   /* (R) Modem status register */

typedef struct {
	union {
		volatile unsigned int RBR;      /*!< Offset: 0x0000   Receive Buffer Register           */
		volatile unsigned int THR;      /*!< Offset: 0x0000   Transmit Holding Register         */
	} x;
	volatile unsigned int IER;              /*!< Offset: 0x0004   Interrupt Enable Register         */
	volatile unsigned int FCR;              /*!< Offset: 0x0008   FIFO Control Register             */
	volatile unsigned int LCR;              /*!< Offset: 0x000C   Line Control Register             */
	volatile unsigned int MCR;              /*!< Offset: 0x0010   Modem Control Register            */
	volatile unsigned int MSR;              /*!< Offset: 0x0014   Modem Status Register             */
	volatile unsigned int FSR;              /*!< Offset: 0x0018   FIFO Status Register              */
	volatile unsigned int ISR;              /*!< Offset: 0x001C   Interrupt Status Register         */
	volatile unsigned int TOR;              /*!< Offset: 0x0020   Time Out Register                 */
	volatile unsigned int BAUD;             /*!< Offset: 0x0024   Baud Rate Divisor Register        */
	volatile unsigned int IRCR;             /*!< Offset: 0x0028   IrDA Control Register             */
	volatile unsigned int ALTCON;           /*!< Offset: 0x002C   Alternate Control/Status Register */
	volatile unsigned int FUNSEL;           /*!< Offset: 0x0030   Function Select Register          */
} UART_TypeDef;

#define SYS_GPE_MFPH	0x404600A4
#define UART0_BASE	0x40700000 /* UART0 Control (High-Speed UART) */
#define UART0           ((UART_TypeDef *)UART0_BASE)
/*
 * Initialise the serial port with the given baudrate. The settings are always 8n1.
 */


int nua3500_serial_init (void)
{
	__raw_writel(__raw_readl(0X4046020C) | (1 << 12), 0X4046020C);  // Uart CLK @ APBCLK0
	__raw_writel(__raw_readl(0X40460220) & ~(3 << 16), 0X40460220);      // Uart CLK from HXT

	/* UART0 line configuration for (9600,n,8,1) */
	UART0->LCR |=0x07;
	UART0->BAUD = 0x3000000E;       /* 24MHz reference clock input, 9600 */
	UART0->FCR |=0x02;              // Reset UART0 Rx FIFO

	__raw_writel((__raw_readl(SYS_GPE_MFPH) & ~0xff000000) | 0x11000000, SYS_GPE_MFPH); // UART0 multi-function

	printf("NUA3500 UART ok\n");
	return 0;
}

void nua3500_serial_putc (const char ch)
{
	while ((UART0->FSR & 0x800000)); //waits for TX_FULL bit is clear
	UART0->x.THR = ch;
	if(ch == '\n') {
		while((UART0->FSR & 0x800000)); //waits for TX_FULL bit is clear
		UART0->x.THR = '\r';
	}
}

void nua3500_serial_puts (const char *s)
{
	while (*s) {
		nua3500_serial_putc (*s++);
	}
}

int nua3500_serial_getc (void)
{
	while (1) {
		if (!(UART0->FSR & (1 << 14))) {
			return (UART0->x.RBR);
		}
		//WATCHDOG_RESET();
	}
}

int nua3500_serial_tstc (void)
{
	return (!((__raw_readl(UART0_BASE + REG_COM_MSR) & RX_FIFO_EMPTY)>>14));
}

void nua3500_serial_setbrg (void)
{

	return;
}

/* TODO : CWWeng 2020.8.31 : replace this driver with driver model */
static struct serial_device nua3500_serial_drv = {
	.name   = "nua3500_serial",
	.start  = nua3500_serial_init,
	.stop   = NULL,
	.setbrg = nua3500_serial_setbrg,
	.putc   = nua3500_serial_putc,
	.puts   = nua3500_serial_puts,
	.getc   = nua3500_serial_getc,
	.tstc   = nua3500_serial_tstc,
};

void nua3500_serial_initialize(void)
{
	serial_register(&nua3500_serial_drv);
}

__weak struct serial_device *default_serial_console(void) {
	return &nua3500_serial_drv;
}

