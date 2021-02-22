// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2014 Marek Vasut <marex@denx.de>
 *
 * Command for en/de-crypting block of memory with AES-[128/192/256]-CBC cipher.
 */

#include <common.h>
#include <command.h>
#include <uboot_aes.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <linux/compiler.h>
#include <mapmem.h>

#include <nuvoton/nuvoton-crypto.h>

/**
 * Nuvton AES hardware accelerator decrypt AES-256 CFB mode encrypted image with
 * AES key from Key Store OTP.
 * nu_aes_decrypt() - Handle the "nu_aes" command-line command
 * @cmdtp:	Command data struct pointer
 * @flag:	Command flag
 * @argc:	Command-line argument count
 * @argv:	Array of command-line arguments
 *
 * Returns zero on success, CMD_RET_USAGE in case of misuse and negative
 * on error.
 */
static int nu_aes_decrypt(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	u32 keynum, src_addr, dst_addr, len;

	keynum = simple_strtoul(argv[1], NULL, 10);
	src_addr = simple_strtoul(argv[2], NULL, 16);
	dst_addr = simple_strtoul(argv[3], NULL, 16);
	len = simple_strtoul(argv[4], NULL, 16);

	return nma35d1_aes_decrypt(keynum, src_addr, dst_addr, len);
}

/***************************************************/
static char aes_help_text[] =
	"keynum src dst len - Decrypt image data $len bytes long at address\n"
	"                           $src using key from Key Store OTP key $keynum.\n"
	"                           Store the result at address $dst.\n"
	"                           The $len size must be multiple of 16 bytes.\n";

U_BOOT_CMD(nu_aes, 5, 1, nu_aes_decrypt,
	   "Nuvton AES256 CFB decryption",
	   aes_help_text
);

