#ifndef _STREAM_CODEC_H_
#define _STREAM_CODEC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <util/config.h>
#include <net/buffer.h>

struct stream_codec {
	char data[DEFAULT_BUFFER_SIZE];
	size_t caret;
	bool must_free;
};
typedef struct stream_codec stream_codec_t;

// Modifier flags
#define CODEC_NEGATIVE (1 << 0)
#define CODEC_INV128   (1 << 1)
#define CODEC_OFS128   (1 << 2)
#define CODEC_LITTLE   (1 << 3)

stream_codec_t* codec_create(stream_codec_t* codec);
stream_codec_t* codec_create_buf(stream_codec_t* codec, char* data, int len);
void codec_free(stream_codec_t* codec);

void codec_seek(stream_codec_t* codec, size_t caret);
size_t codec_len(stream_codec_t* codec);

bool codec_buffer_write(stream_codec_t* codec, buffer_t* buffer);
bool codec_buffer_read(stream_codec_t* codec, buffer_t* buffer, size_t len);

void codec_put8(stream_codec_t* codec, uint8_t i, uint8_t flags);
void codec_put16(stream_codec_t* codec, uint16_t i, uint8_t flags);
void codec_put24(stream_codec_t* codec, uint32_t i, uint8_t flags);
void codec_put32(stream_codec_t* codec, uint32_t i, uint8_t flags);
void codec_put64(stream_codec_t* codec, uint64_t i, uint8_t flags);
void codec_putn(stream_codec_t* codec, char* data, size_t len);

uint8_t codec_get8(stream_codec_t* codec, uint8_t* i, uint8_t flags);
uint16_t codec_get16(stream_codec_t* codec, uint16_t* i, uint8_t flags);
uint32_t codec_get24(stream_codec_t* codec, uint32_t* i, uint8_t flags);
uint32_t codec_get32(stream_codec_t* codec, uint32_t* i, uint8_t flags);
uint64_t codec_get64(stream_codec_t* codec, uint64_t* i, uint8_t flags);
char* codec_getn(stream_codec_t* codec, char* data, size_t len);

#endif /* _STREAM_CODEC_H_ */