/**
 * rsa.c
 *
 * Defines the rsa encryption algorithm
 */
#include <crypto/rsa.h>

#include <stdio.h>
#include <stdlib.h>

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

object_proto_t rsa_proto = {
	.init = (object_init_t)rsa_init,
	.free = (object_free_t)rsa_free
};
