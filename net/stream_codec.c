/**
 * stream_codec.c
 *
 * Defines a codec matching that of the client.
 * Assumes little endian native order.
 */

#include <net/stream_codec.h>

#include <util/log.h>

#define LOG_TAG "codec"

stream_codec_t* codec_create(stream_codec_t* codec)
{
	if (codec == NULL) {
		codec = (stream_codec_t*)malloc(sizeof(stream_codec_t));
		codec->must_free = true;
	} else {
		codec->must_free = false;
	}
	memset(codec->data, 0, DEFAULT_BUFFER_SIZE);
	codec->caret = 0;
	return codec;
}

stream_codec_t* codec_create_buf(stream_codec_t* codec, char* data, int len)
{
	codec = codec_create(codec);
	memcpy(codec->data, data, len);
	return codec;
}

void codec_free(stream_codec_t* codec)
{
	if (codec->must_free) {
		free(codec);
	}
}

void codec_seek(stream_codec_t* codec, size_t caret)
{
	if (caret > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to seek past end of buffer");
		return;
	}
	codec->caret = caret;
}

size_t codec_len(stream_codec_t* codec)
{
	// We could do with a more reliable way to keep track of
	// the valid data in the buffer.
	return codec->caret;
}

bool codec_buffer_write(stream_codec_t* codec, buffer_t* buffer)
{
	buffer_pushp(buffer);
	if (buffer_write(buffer, codec->data, codec_len(codec)) < codec_len(codec)) {
		buffer_popp(buffer);
		return false;
	}
	return true;
}

bool codec_buffer_read(stream_codec_t* codec, buffer_t* buffer, size_t len)
{
	if (codec->caret+len > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return;
	}

	buffer_pushp(buffer);
	if (buffer_read(buffer, codec->data+codec->caret, len) < len) {
		buffer_popp(buffer);
		return false;
	}
	return true;
}

void codec_put8(stream_codec_t* codec, uint8_t i, uint8_t flags)
{
	if (codec->caret+1 > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return;
	}

	uint8_t* val = (uint8_t*)&i;
	if (flags & CODEC_NEGATIVE) {
		val[0] = -val[0];
	}
	if (flags & CODEC_INV128) {
		val[0] = 128 - val[0];
	}
	if (flags & CODEC_OFS128) {
		val[0] = val[0] + 128;
	}

	codec->data[codec->caret++] = val[0];
}

void codec_put16(stream_codec_t* codec, uint16_t i, uint8_t flags)
{
	if (codec->caret+2 > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return;
	}

	uint8_t* val = (uint8_t*)&i;
	if (flags & CODEC_NEGATIVE) {
		val[0] = -val[0];
	}
	if (flags & CODEC_INV128) {
		val[0] = 128 - val[0];
	}
	if (flags & CODEC_OFS128) {
		val[0] = val[0] + 128;
	}
	if (flags & CODEC_LITTLE) {
		uint16_t tmp = i;
		val[0] = ((char*)&tmp)[1];
		val[1] = ((char*)&tmp)[0];
	}

	codec->data[codec->caret++] = val[1];
	codec->data[codec->caret++] = val[0];
}
void codec_put24(stream_codec_t* codec, uint32_t i, uint8_t flags)
{
	if (codec->caret+3 > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return;
	}

	uint8_t* val = (uint8_t*)&i;
	val[3] = 0;
	if (flags & CODEC_NEGATIVE) {
		val[0] = -val[0];
	}
	if (flags & CODEC_INV128) {
		val[0] = 128 - val[0];
	}
	if (flags & CODEC_OFS128) {
		val[0] = val[0] + 128;
	}
	if (flags & CODEC_LITTLE) {
		uint32_t tmp = i;
		val[0] = ((char*)&tmp)[2];
		val[1] = ((char*)&tmp)[1];
		val[2] = ((char*)&tmp)[0];
	}

	codec->data[codec->caret++] = val[2];
	codec->data[codec->caret++] = val[1];
	codec->data[codec->caret++] = val[0];
}

void codec_put32(stream_codec_t* codec, uint32_t i, uint8_t flags)
{
	if (codec->caret+4 > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return;
	}

	uint8_t* val = (uint8_t*)&i;
	if (flags & CODEC_NEGATIVE) {
		val[0] = -val[0];
	}
	if (flags & CODEC_INV128) {
		val[0] = 128 - val[0];
	}
	if (flags & CODEC_OFS128) {
		val[0] = val[0] + 128;
	}
	if (flags & CODEC_LITTLE) {
		uint32_t tmp = i;
		val[0] = ((char*)&tmp)[3];
		val[1] = ((char*)&tmp)[2];
		val[2] = ((char*)&tmp)[1];
		val[3] = ((char*)&tmp)[0];
	}

	codec->data[codec->caret++] = val[3];
	codec->data[codec->caret++] = val[2];
	codec->data[codec->caret++] = val[1];
	codec->data[codec->caret++] = val[0];
}

void codec_put64(stream_codec_t* codec, uint64_t i, uint8_t flags)
{
	if (codec->caret+8 > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return;
	}

	uint8_t* val = (uint8_t*)&i;
	if (flags & CODEC_NEGATIVE) {
		val[0] = -val[0];
	}
	if (flags & CODEC_INV128) {
		val[0] = 128 - val[0];
	}
	if (flags & CODEC_OFS128) {
		val[0] = val[0] + 128;
	}
	if (flags & CODEC_LITTLE) {
		uint64_t tmp = i;
		val[0] = ((char*)&tmp)[7];
		val[1] = ((char*)&tmp)[6];
		val[2] = ((char*)&tmp)[5];
		val[3] = ((char*)&tmp)[4];
		val[4] = ((char*)&tmp)[3];
		val[5] = ((char*)&tmp)[2];
		val[6] = ((char*)&tmp)[1];
		val[7] = ((char*)&tmp)[0];
	}

	codec->data[codec->caret++] = val[7];
	codec->data[codec->caret++] = val[6];
	codec->data[codec->caret++] = val[5];
	codec->data[codec->caret++] = val[4];
	codec->data[codec->caret++] = val[3];
	codec->data[codec->caret++] = val[2];
	codec->data[codec->caret++] = val[1];
	codec->data[codec->caret++] = val[0];
}

void codec_putn(stream_codec_t* codec, char* data, size_t len)
{
	if (codec->caret+len > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return;
	}

	memcpy(&codec->data[codec->caret], data, len);
	codec->caret += len;
}

uint8_t codec_get8(stream_codec_t* codec, uint8_t* i, uint8_t flags)
{
	if (codec->caret+1 > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to get past end of buffer");
		return 0;
	}

	uint8_t x;
	if (i == NULL) {
		i = &x;
	}
	uint8_t* val = (uint8_t*)i;

	val[0] = codec->data[codec->caret++];

	if (flags & CODEC_NEGATIVE) {
		val[0] = -val[0];
	}
	if (flags & CODEC_INV128) {
		val[0] = 128 - val[0];
	}
	if (flags & CODEC_OFS128) {
		val[0] = val[0] - 128;
	}
	return *i;
}

uint16_t codec_get16(stream_codec_t* codec, uint16_t* i, uint8_t flags)
{
	if (codec->caret+2 > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to get past end of buffer");
		return 0;
	}

	uint16_t x;
	if (i == NULL) {
		i = &x;
	}
	uint8_t* val = (uint8_t*)i;

	val[1] = codec->data[codec->caret++];
	val[0] = codec->data[codec->caret++];

	if (flags & CODEC_LITTLE) {
		uint16_t tmp = *i;
		val[0] = ((char*)&tmp)[1];
		val[1] = ((char*)&tmp)[0];
	}
	if (flags & CODEC_NEGATIVE) {
		val[0] = -val[0];
	}
	if (flags & CODEC_INV128) {
		val[0] = 128 - val[0];
	}
	if (flags & CODEC_OFS128) {
		val[0] = val[0] - 128;
	}
	return *i;
}

uint32_t codec_get24(stream_codec_t* codec, uint32_t* i, uint8_t flags)
{
	if (codec->caret+3 > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to get past end of buffer");
		return 0;
	}

	uint32_t x;
	if (i == NULL) {
		i = &x;
	}
	uint8_t* val = (uint8_t*)i;

	val[3] = 0;
	val[2] = codec->data[codec->caret++];
	val[1] = codec->data[codec->caret++];
	val[0] = codec->data[codec->caret++];

	if (flags & CODEC_LITTLE) {
		uint32_t tmp = *i;
		val[0] = ((char*)&tmp)[2];
		val[1] = ((char*)&tmp)[1];
		val[2] = ((char*)&tmp)[0];
	}
	if (flags & CODEC_NEGATIVE) {
		val[0] = -val[0];
	}
	if (flags & CODEC_INV128) {
		val[0] = 128 - val[0];
	}
	if (flags & CODEC_OFS128) {
		val[0] = val[0] - 128;
	}
	return *i;
}

uint32_t codec_get32(stream_codec_t* codec, uint32_t* i, uint8_t flags)
{
	if (codec->caret+4 > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to get past end of buffer");
		return 0;
	}

	uint32_t x;
	if (i == NULL) {
		i = &x;
	}
	uint8_t* val = (uint8_t*)i;

	val[3] = codec->data[codec->caret++];
	val[2] = codec->data[codec->caret++];
	val[1] = codec->data[codec->caret++];
	val[0] = codec->data[codec->caret++];

	if (flags & CODEC_LITTLE) {
		uint32_t tmp = *i;
		val[0] = ((char*)&tmp)[3];
		val[1] = ((char*)&tmp)[2];
		val[2] = ((char*)&tmp)[1];
		val[3] = ((char*)&tmp)[0];
	}
	if (flags & CODEC_NEGATIVE) {
		val[0] = -val[0];
	}
	if (flags & CODEC_INV128) {
		val[0] = 128 - val[0];
	}
	if (flags & CODEC_OFS128) {
		val[0] = val[0] - 128;
	}
	return *i;
}
uint64_t codec_get64(stream_codec_t* codec, uint64_t* i, uint8_t flags)
{
	if (codec->caret+8 > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to get past end of buffer");
		return 0;
	}

	uint64_t x;
	if (i == NULL) {
		i = &x;
	}
	uint8_t* val = (uint8_t*)i;

	val[7] = codec->data[codec->caret++];
	val[6] = codec->data[codec->caret++];
	val[5] = codec->data[codec->caret++];
	val[4] = codec->data[codec->caret++];
	val[3] = codec->data[codec->caret++];
	val[2] = codec->data[codec->caret++];
	val[1] = codec->data[codec->caret++];
	val[0] = codec->data[codec->caret++];

	if (flags & CODEC_LITTLE) {
		uint64_t tmp = *i;
		val[0] = ((char*)&tmp)[7];
		val[1] = ((char*)&tmp)[6];
		val[2] = ((char*)&tmp)[5];
		val[3] = ((char*)&tmp)[4];
		val[4] = ((char*)&tmp)[3];
		val[5] = ((char*)&tmp)[2];
		val[6] = ((char*)&tmp)[1];
		val[7] = ((char*)&tmp)[0];
	}
	if (flags & CODEC_NEGATIVE) {
		val[0] = -val[0];
	}
	if (flags & CODEC_INV128) {
		val[0] = 128 - val[0];
	}
	if (flags & CODEC_OFS128) {
		val[0] = val[0] - 128;
	}
	return *i;
}

char* codec_getn(stream_codec_t* codec, char* data, size_t len)
{
	if (codec->caret+len > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return 0;
	}

	if (data == NULL) {
		data = (char*)malloc(len); // Caller is responsible for freeing
	}

	memcpy(&codec->data[codec->caret], data, len);
	codec->caret += len;
	return data;
}
