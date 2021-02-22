/* SPDX-License-Identifier: GPL-2.0+  */
/*
 * (C) Copyright 2020 Nuvoton Technology Corp.
 *
 */

#ifndef _NUVOTON_CRYPTO_H_
#define _NUVOTON_CRYPTO_H_

/**
 * Nuvton AES hardware accelerator decrypt AES-256 CFB mode encrypted image with
 * AES key from Key Store OTP.
 *
 * @keynum		The Key Store OTP key number that stored the AES key.
 * @src_addr		Source address of cypher text
 * @dst_addr		Destination address of plain text output
 * @data_len		Number bytes to decrypt
 */
int ma35d1_aes_decrypt(u32 keynum, u32 src_addr,
			u32 dst_addr, u32 data_len);

#endif
