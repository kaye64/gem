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

#ifndef _RSA_H_
#define _RSA_H_

#include <stdbool.h>
#include <gmp.h>
#include <runite/util/object.h>

typedef struct rsa rsa_t;
typedef struct codec codec_t;

struct rsa {
	object_t object;
	mpz_t modulus;
	mpz_t public_exp;
	mpz_t private_exp;
};

extern object_proto_t rsa_proto;

void rsa_load_key(rsa_t* rsa, const char* modulus, const char* public_exp, const char* private_exp);
void rsa_encrypt(rsa_t* rsa, unsigned char* in, int in_len, unsigned char* out, int* out_len);
void rsa_decrypt(rsa_t* rsa, unsigned char* in, int in_len, unsigned char* out, int* out_len);

void rsa_block_encrypt(rsa_t* rsa, codec_t* codec);
void rsa_block_decrypt(rsa_t* rsa, codec_t* codec);

#endif /* _RSA_H_ */
