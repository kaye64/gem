/**
 *  This file is part of Gem.
 *
 *  Gem is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gem is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gem.  If not, see <http://www.gnu.org/licenses/\>.
 */

/**
 * rsa.c
 *
 * Defines the rsa encryption algorithm
 */
#include <crypto/rsa.h>

#include <stdio.h>
#include <stdlib.h>
#include <runite/util/codec.h>

#include <util/log.h>

#define LOG_TAG "rsa"

/**
 * Initializes an rsa_t
 */
static void rsa_init(rsa_t* rsa)
{

}

/**
 * Properly frees an rsa_t
 */
static void rsa_free(rsa_t* rsa)
{
	mpz_clear(rsa->modulus);
	mpz_clear(rsa->public_exp);
	mpz_clear(rsa->private_exp);
}

/**
 * Initializes the RSA keypair
 */
void rsa_load_key(rsa_t* rsa, const char* modulus, const char* public_exp, const char* private_exp)
{
	mpz_init_set_str(rsa->modulus, modulus, 10);
	mpz_init_set_str(rsa->public_exp, public_exp, 10);
	mpz_init_set_str(rsa->private_exp, private_exp, 10);
}

/**
 * Encrypts an rsa block
 */
void rsa_encrypt(rsa_t* rsa, unsigned char* in, int in_len, unsigned char* out, int* out_len)
{
#ifdef RSA_DISABLE
	memcpy(out, in, in_len);
	*out_len = in_len;
#else
	mpz_t message; mpz_init(message);
	mpz_t encrypted; mpz_init(encrypted);

	mpz_import(message, in_len, 1, 1, 0, 0, in);
	mpz_powm(encrypted, message, rsa->public_exp, rsa->modulus);
	mpz_export(out, (size_t*)out_len, 1, 1, 0, 0, encrypted);

	mpz_clear(message);
	mpz_clear(encrypted);
#endif
}

/**
 * Decrypts an rsa block
 */
void rsa_decrypt(rsa_t* rsa, unsigned char* in, int in_len, unsigned char* out, int* out_len)
{
#ifdef RSA_DISABLE
	memcpy(out, in, in_len);
	*out_len = in_len;
#else
	mpz_t encrypted; mpz_init(encrypted);
	mpz_t message; mpz_init(message);

	mpz_import(encrypted, in_len, 1, 1, 0, 0, in);
	mpz_powm(message, encrypted, rsa->private_exp, rsa->modulus);
	mpz_export(out, (size_t*)out_len, 1, 1, 0, 0, message);

	mpz_clear(encrypted);
	mpz_clear(message);
#endif
}

/**
 * Encrypt an rsa block from 0 to the current caret
 *  - rsa: The RSA keypair to encrypt with
 */
void rsa_block_encrypt(rsa_t* rsa, codec_t* codec)
{
	int in_len = codec->caret;
	int out_len = DEFAULT_BUFFER_SIZE;
	unsigned char message[in_len];
	memcpy(message, codec->data, in_len);
	rsa_encrypt(rsa, message, in_len, &codec->data[1], &out_len);
	codec->data[0] = out_len;
	codec->caret = out_len+1;
}

/**
 * Decrypt an rsa block from the current caret
 *  - rsa: The RSA keypair to decrypt with
 */
void rsa_block_decrypt(rsa_t* rsa, codec_t* codec)
{
	int enc_len = codec->data[codec->caret];
	int out_len = DEFAULT_BUFFER_SIZE-codec->caret;
	unsigned char message_enc[enc_len];
	memcpy(message_enc, &codec->data[codec->caret+1], enc_len);
	rsa_decrypt(rsa, message_enc, enc_len, &codec->data[codec->caret], &out_len);
}


object_proto_t rsa_proto = {
	.init = (object_init_t)rsa_init,
	.free = (object_free_t)rsa_free
};
