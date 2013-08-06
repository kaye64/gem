#ifndef _STREAM_CODEC_H_
#define _STREAM_CODEC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <util/config.h>
#include <util/object.h>
#include <net/buffer.h>
#include <crypto/rsa.h>

typedef struct codec codec_t;

struct codec {
	object_t object;
	unsigned char data[DEFAULT_BUFFER_SIZE];
	size_t caret;
	bool bit_access_mode;
	size_t bit_caret;
};

extern object_proto_t codec_proto;

// Modifier flags
#define CODEC_NEGATIVE (1 << 0)
#define CODEC_INV128   (1 << 1)
#define CODEC_OFS128   (1 << 2)
#define CODEC_LITTLE   (1 << 3)
#define CODEC_MIDDLE_A  (1 << 4)
#define CODEC_MIDDLE_B  (1 << 5)
#define CODEC_JSTRING  (1 << 6)

void codec_seek(codec_t* codec, size_t caret);
size_t codec_len(codec_t* codec);

/* buffer operations */
bool codec_buffer_write(codec_t* codec, buffer_t* buffer);
bool codec_buffer_read(codec_t* codec, buffer_t* buffer, size_t len);

/* rsa operations */
void codec_block_encrypt(codec_t* codec, rsa_t* rsa);
void codec_block_decrypt(codec_t* codec, rsa_t* rsa);

/* bit mode operations */
void codec_set_bit_access_mode(codec_t* codec, bool bit_mode);
void codec_put_bits(codec_t* codec, int nbits, uint32_t i);
uint32_t codec_get_bits(codec_t* codec, int nbits);

/* byte mode operations */
void codec_put8(codec_t* codec, uint8_t i);
void codec_put16(codec_t* codec, uint16_t i);
void codec_put24(codec_t* codec, uint32_t i);
void codec_put32(codec_t* codec, uint32_t i);
void codec_put64(codec_t* codec, uint64_t i);
void codec_putn(codec_t* codec, unsigned char* data, size_t len);
void codec_concat(codec_t* codec, codec_t* other);

void codec_put8f(codec_t* codec, uint8_t i, uint8_t flags);
void codec_put16f(codec_t* codec, uint16_t i, uint8_t flags);
void codec_put24f(codec_t* codec, uint32_t i, uint8_t flags);
void codec_put32f(codec_t* codec, uint32_t i, uint8_t flags);
void codec_put64f(codec_t* codec, uint64_t i, uint8_t flags);
void codec_puts(codec_t* codec, char* s, int len, uint8_t flags);

uint8_t codec_get8(codec_t* codec);
uint16_t codec_get16(codec_t* codec);
uint32_t codec_get24(codec_t* codec);
uint32_t codec_get32(codec_t* codec);
uint64_t codec_get64(codec_t* codec);
unsigned char* codec_getn(codec_t* codec, unsigned char* data, size_t len);

uint8_t codec_get8fp(codec_t* codec, uint8_t* i, uint8_t flags);
uint16_t codec_get16fp(codec_t* codec, uint16_t* i, uint8_t flags);
uint32_t codec_get24fp(codec_t* codec, uint32_t* i, uint8_t flags);
uint32_t codec_get32fp(codec_t* codec, uint32_t* i, uint8_t flags);
uint64_t codec_get64fp(codec_t* codec, uint64_t* i, uint8_t flags);
char* codec_gets(codec_t* codec, char* s, int len, uint8_t flags);

#endif /* _STREAM_CODEC_H_ */
