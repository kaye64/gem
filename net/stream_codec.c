/**
 * stream_codec.c
 *
 * Defines a codec matching that of the client.
 * Assumes little endian native order.
 */

#include <net/stream_codec.h>

#include <util/log.h>

#define LOG_TAG "codec"

/**
 * codec_create
 *
 * Initializes a new codec
 *  - codec: Some preallocated memory, or null to put on heap
 * returns: The initialized codec
 */
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

/**
 * codec_create
 *
 * Initializes a new codec and fills it with some data
 *  - codec: Some preallocated memory, or null to put on heap
 *  - data: Some data
 *  - len: The length of the data
 * returns: The initialized codec
 */
stream_codec_t* codec_create_buf(stream_codec_t* codec, unsigned char* data, int len)
{
	codec = codec_create(codec);
	memcpy(codec->data, data, len);
	return codec;
}

/**
 * codec_free
 *
 * Properly frees a stream_codec_t
 *  - codec: The codec to free
 */
void codec_free(stream_codec_t* codec)
{
	if (codec->must_free) {
		free(codec);
	}
}

/**
 * codec_seek
 *
 * Seek to a given position in the codec
 *  - codec: The codec
 *  - caret: The position
 */
void codec_seek(stream_codec_t* codec, size_t caret)
{
	if (caret > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to seek past end of buffer");
		return;
	}
	codec->caret = caret;
}

/**
 * codec_len
 *
 * Returns the amount of valid data in the codec
 *  - codec: The codec
 * returns: The amount of valid data in the codec
 */
size_t codec_len(stream_codec_t* codec)
{
	// We could do with a more reliable way to keep track of
	// the valid data in the buffer.
	return codec->caret;
}

/**
 * codec_buffer_write
 *
 * Writes the entire contents (and only the entire contents)
 * of the codec to a buffer_t
 *  - codec: The codec
 *  - buffer: The buffer
 * returns: Whether the operation was a success
 */
bool codec_buffer_write(stream_codec_t* codec, buffer_t* buffer)
{
	buffer_pushp(buffer);
	if (buffer_write(buffer, codec->data, codec_len(codec)) < codec_len(codec)) {
		buffer_popp(buffer);
		return false;
	}
	return true;
}

/**
 * codec_buffer_read
 *
 * Reads a given amount from a buffer_t into the codec
 *  - codec: The codec
 *  - buffer: The buffer
 *  - len: The amount of data to read
 * returns: Whether the operation was a success
 */
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

/**
 * codec_put8
 *
 * Puts 8 bits to the codec
 *  - codec: The codec
 *  - i: The value
 *  - flags: Modifier flags
 */
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

/**
 * codec_put16
 *
 * Puts 16 bits to the codec
 *  - codec: The codec
 *  - i: The value
 *  - flags: Modifier flags
 */
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
		val[0] = ((unsigned char*)&tmp)[1];
		val[1] = ((unsigned char*)&tmp)[0];
	}

	codec->data[codec->caret++] = val[1];
	codec->data[codec->caret++] = val[0];
}

/**
 * codec_put24
 *
 * Puts 24 bits to the codec
 *  - codec: The codec
 *  - i: The value
 *  - flags: Modifier flags
 */
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
		val[0] = ((unsigned char*)&tmp)[2];
		val[1] = ((unsigned char*)&tmp)[1];
		val[2] = ((unsigned char*)&tmp)[0];
	}

	codec->data[codec->caret++] = val[2];
	codec->data[codec->caret++] = val[1];
	codec->data[codec->caret++] = val[0];
}

/**
 * codec_put32
 *
 * Puts 32 bits to the codec
 *  - codec: The codec
 *  - i: The value
 *  - flags: Modifier flags
 */
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
		val[0] = ((unsigned char*)&tmp)[3];
		val[1] = ((unsigned char*)&tmp)[2];
		val[2] = ((unsigned char*)&tmp)[1];
		val[3] = ((unsigned char*)&tmp)[0];
	}

	codec->data[codec->caret++] = val[3];
	codec->data[codec->caret++] = val[2];
	codec->data[codec->caret++] = val[1];
	codec->data[codec->caret++] = val[0];
}

/**
 * codec_put64
 *
 * Puts 64 bits to the codec
 *  - codec: The codec
 *  - i: The value
 *  - flags: Modifier flags
 */
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
		val[0] = ((unsigned char*)&tmp)[7];
		val[1] = ((unsigned char*)&tmp)[6];
		val[2] = ((unsigned char*)&tmp)[5];
		val[3] = ((unsigned char*)&tmp)[4];
		val[4] = ((unsigned char*)&tmp)[3];
		val[5] = ((unsigned char*)&tmp)[2];
		val[6] = ((unsigned char*)&tmp)[1];
		val[7] = ((unsigned char*)&tmp)[0];
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

/**
 * codec_putn
 *
 * Puts a given number of bytes to the codec
 *  - codec: The codec
 *  - data: The data
 *  - len: The length of the data
 */
void codec_putn(stream_codec_t* codec, unsigned char* data, size_t len)
{
	if (codec->caret+len > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return;
	}

	memcpy(&codec->data[codec->caret], data, len);
	codec->caret += len;
}

/**
 * codec_puts
 *
 * Puts a string to the codec
 *  - codec: The codec
 *  - s: The string
 *  - len: The length of the string
 *  - flags: Can be CODEC_JSTRING for 0xA terminated string
 */
void codec_puts(stream_codec_t* codec, char* s, int len, uint8_t flags)
{
	if (codec->caret+len > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return;
	}

	codec_putn(codec, (unsigned char*)s, len);
	if (flags & CODEC_JSTRING) {
		codec_put8(codec, 10, 0);
	} else {
		codec_put8(codec, 0, 0);
	}
}

/**
 * codec_get8
 *
 * Gets 8 bits from the codec
 *  - codec: The codec
 *  - i: Location to store the value, or NULL
 *  - flags: Modifier flags
 * returns: The value
 */
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

/**
 * codec_get16
 *
 * Gets 16 bits from the codec
 *  - codec: The codec
 *  - i: Location to store the value, or NULL
 *  - flags: Modifier flags
 * returns: The value
 */
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
		val[0] = ((unsigned char*)&tmp)[1];
		val[1] = ((unsigned char*)&tmp)[0];
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

/**
 * codec_get24
 *
 * Gets 24 bits from the codec
 *  - codec: The codec
 *  - i: Location to store the value, or NULL
 *  - flags: Modifier flags
 * returns: The value
 */
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
		val[0] = ((unsigned char*)&tmp)[2];
		val[1] = ((unsigned char*)&tmp)[1];
		val[2] = ((unsigned char*)&tmp)[0];
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

/**
 * codec_get32
 *
 * Gets 32 bits from the codec
 *  - codec: The codec
 *  - i: Location to store the value, or NULL
 *  - flags: Modifier flags
 * returns: The value
 */
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
		val[0] = ((unsigned char*)&tmp)[3];
		val[1] = ((unsigned char*)&tmp)[2];
		val[2] = ((unsigned char*)&tmp)[1];
		val[3] = ((unsigned char*)&tmp)[0];
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


/**
 * codec_get64
 *
 * Gets 64 bits from the codec
 *  - codec: The codec
 *  - i: Location to store the value, or NULL
 *  - flags: Modifier flags
 * returns: The value
 */
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
		val[0] = ((unsigned char*)&tmp)[7];
		val[1] = ((unsigned char*)&tmp)[6];
		val[2] = ((unsigned char*)&tmp)[5];
		val[3] = ((unsigned char*)&tmp)[4];
		val[4] = ((unsigned char*)&tmp)[3];
		val[5] = ((unsigned char*)&tmp)[2];
		val[6] = ((unsigned char*)&tmp)[1];
		val[7] = ((unsigned char*)&tmp)[0];
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

/**
 * codec_getn
 *
 * Gets a given number of bytes from the codec. If data is NULL,
 * space is created on the heap. Caller is responsible for freeing
 * this memory.
 *  - codec: The codec
 *  - data: A place to store the data, or NULL
 *  - len: The length of the data
 * returns: A pointer to the data read
 */
unsigned char* codec_getn(stream_codec_t* codec, unsigned char* data, size_t len)
{
	if (codec->caret+len > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return 0;
	}

	if (data == NULL) {
		data = (unsigned char*)malloc(len); // Caller is responsible for freeing
	}

	memcpy(data, &codec->data[codec->caret], len);
	codec->caret += len;
	return data;
}

/**
 * codec_gets
 *
 * Gets a string from the codec
 *  - codec: The codec
 *  - s: The output buffer
 *  - len: The length of the output buffer. Set to the length of the string
 *  - flags: Can be CODEC_JSTRING for 0xA terminated string
 * returns: A pointer to the output buffer
 */
char* codec_gets(stream_codec_t* codec, char* s, int* len, uint8_t flags)
{
	if (codec->caret+(*len) > DEFAULT_BUFFER_SIZE) {
		ERROR("tried to put past end of buffer");
		return NULL;
	}

	char terminator = 0;
	if (flags & CODEC_JSTRING) {
		terminator = 10;
	}
	bool valid = false;
	for (int i = 0; i < (*len)-1; i++) {
		if (codec->data[codec->caret+i] == terminator) {
			*len = i;
			valid = true;
			break;
		}
	}
	if (!valid) {
		ERROR("No valid string found");
		return NULL;
	}
	codec_getn(codec, (unsigned char*)s, *len);
	s[*len] = 0;
	return s;
}
