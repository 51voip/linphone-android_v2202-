/*
  Copyright (C) 2006-2007 Werner Dittmann

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * Authors: Werner Dittmann <Werner.Dittmann@t-online.de>
 */

#ifndef _AESCFB_H__
#define _AESCFB_H__

#include <stdint.h>

/**
 * @file aesCFB.h
 * @brief Function that provide AES CFB mode support
 * 
 * @ingroup GNU_ZRTP
 * @{
 */

#ifndef AES_BLOCK_SIZE
#define AES_BLOCK_SIZE 16
#endif

/**
 * Encrypt data with AES CFB mode, full block feedback size.
 *
 * This functions takes one data chunk and encrypts it with
 * AES CFB mode. The lenght of the data may be arbitrary and
 * it is not needed to be a multiple of AES blocksize.
 *
 * @param key
 *    Points to the key bytes.
 * @param keyLength
 *    Length of the key in bytes
 * @param IV
 *    The initialization vector which must be AES_BLOCKSIZE (16) bytes.
 * @param data
 *    Points to a buffer that contains and receives the computed
 *    the data (in-place encryption).
 * @param dataLength
 *    Length of the data in bytes
 */

void aesCfbEncrypt(uint8_t* key, int32_t keyLength, uint8_t* IV, uint8_t *data,
                   int32_t dataLength);

/**
 * Decrypt data with AES CFB mode, full block feedback size.
 *
 * This functions takes one data chunk and decrypts it with
 * AES CFB mode. The lenght of the data may be arbitrary and
 * it is not needed to be a multiple of AES blocksize.
 *
 * @param key
 *    Points to the key bytes.
 * @param keyLength
 *    Length of the key in bytes
 * @param IV
 *    The initialization vector which must be AES_BLOCKSIZE (16) bytes.
 * @param data
 *    Points to a buffer that contains and receives the computed
 *    the data (in-place decryption).
 * @param dataLength
 *    Length of the data in bytes
 */

void aesCfbDecrypt(uint8_t* key, int32_t keyLength, const uint8_t* IV, uint8_t *data,
                   int32_t dataLength);
/**
 * @}
 */
#endif
