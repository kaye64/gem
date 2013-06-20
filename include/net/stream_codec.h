#ifndef _STREAM_CODEC_H_
#define _STREAM_CODEC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <util/config.h>
#include <net/buffer.h>
#include <crypto/rsa.h>

struct stream_codec {
	unsigned char data[DEFAULT_BUFFER_SIZE];
	size_t caret;
	bool must_free;
};
typedef struct stream_codec stream_codec_t;

// Modifier flags
#define CODEC_NEGATIVE (1 << 0)
#define CODEC_INV128   (1 << 1)
#define CODEC_OFS128   (1 << 2)
#define CODEC_LITTLE   (1 << 3)
#define CODEC_JSTRING  (1 << 4)

stream_codec_t* codec_create(stream_codec_t* codec);
stream_codec_t* codec_create_buf(stream_codec_t* codec, unsigned char* data, int len);
void codec_free(stream_codec_t* codec);

void codec_seek(stream_codec_t* codec, size_t caret);
size_t codec_len(stream_codec_t* codec);

bool codec_buffer_write(stream_codec_t* codec, buffer_t* buffer);
bool codec_buffer_read(stream_codec_t* codec, buffer_t* buffer, size_t len);

void codec_block_encrypt(stream_codec_t* codec, rsa_t* rsa);
void codec_block_decrypt(stream_codec_t* codec, rsa_t* rsa);

void codec_put8(stream_codec_t* codec, uint8_t i);
void codec_put16(stream_codec_t* codec, uint16_t i);
void codec_put24(stream_codec_t* codec, uint32_t i);
void codec_put32(stream_codec_t* codec, uint32_t i);
void codec_put64(stream_codec_t* codec, uint64_t i);
void codec_putn(stream_codec_t* codec, unsigned char* data, size_t len);
void codec_concat(stream_codec_t* codec, stream_codec_t* other);

void codec_put8f(stream_codec_t* codec, uint8_t i, uint8_t flags);
void codec_put16f(stream_codec_t* codec, uint16_t i, uint8_t flags);
void codec_put24f(stream_codec_t* codec, uint32_t i, uint8_t flags);
void codec_put32f(stream_codec_t* codec, uint32_t i, uint8_t flags);
void codec_put64f(stream_codec_t* codec, uint64_t i, uint8_t flags);
void codec_puts(stream_codec_t* codec, char* s, int len, uint8_t flags);

uint8_t codec_get8(stream_codec_t* codec);
uint16_t codec_get16(stream_codec_t* codec);
uint32_t codec_get24(stream_codec_t* codec);
uint32_t codec_get32(stream_codec_t* codec);
uint64_t codec_get64(stream_codec_t* codec);
unsigned char* codec_getn(stream_codec_t* codec, unsigned char* data, size_t len);

uint8_t codec_get8fp(stream_codec_t* codec, uint8_t* i, uint8_t flags);
uint16_t codec_get16fp(stream_codec_t* codec, uint16_t* i, uint8_t flags);
uint32_t codec_get24fp(stream_codec_t* codec, uint32_t* i, uint8_t flags);
uint32_t codec_get32fp(stream_codec_t* codec, uint32_t* i, uint8_t flags);
uint64_t codec_get64fp(stream_codec_t* codec, uint64_t* i, uint8_t flags);
char* codec_gets(stream_codec_t* codec, char* s, int len, uint8_t flags);

#endif /* _STREAM_CODEC_H_ */
