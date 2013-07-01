#ifndef _RSA_H_
#define _RSA_H_

#include <stdbool.h>
#include <gmp.h>

#include <util/object.h>

typedef struct rsa rsa_t;

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

#endif /* _RSA_H_ */
