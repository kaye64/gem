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
 * buffer.c
 *
 * A circular buffer implementation, intended for io buffering
 */
#include <net/buffer.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <runite/util/math.h>
#include <runite/util/codec.h>

#include <util/config.h>

/**
 * Initializes a new buffer
 */
static void buffer_init(buffer_t* buffer)
{
	buffer->data = (unsigned char*)malloc(sizeof(unsigned char)*DEFAULT_BUFFER_SIZE);
	buffer->real_size = DEFAULT_BUFFER_SIZE;
	buffer->read_ptr = 0;
	buffer->read_avail = 0;
	object_init(int_stack, &buffer->ptr_stack);
}

/**
 * Properly frees a buffer
 */
static void buffer_free(buffer_t* buffer)
{
	free(buffer->data);
	object_free(&buffer->ptr_stack);
}

/**
 * Reallocates the internal buffer to a new size
 */
void buffer_realloc(buffer_t* buffer, size_t size)
{
	free(buffer->data);
	buffer->data = (unsigned char*)malloc(sizeof(unsigned char)*size);
	buffer->real_size = size;
	buffer->read_ptr = 0;
	buffer->read_avail = 0;
}

/**
 * Returns the number of bytes available to read in a buffer
 */
size_t buffer_read_avail(buffer_t* buffer)
{
	return buffer->read_avail;
}

/**
 * Returns the number of bytes available to write in a buffer
 */
size_t buffer_write_avail(buffer_t* buffer)
{
	return buffer->real_size - buffer->read_avail;
}

/**
 * Reads up to a given number of bytes into a local buffer
 * returns: The amount of bytes read
 */
size_t buffer_read(buffer_t* buffer, unsigned char* buf, size_t len)
{
	size_t to_read = min(len, buffer_read_avail(buffer));
	for (size_t i = 0; i < to_read; i++) {
		buf[i] = buffer->data[(buffer->read_ptr + i) % buffer->real_size];
	}
	buffer->read_ptr = (buffer->read_ptr + to_read) % buffer->real_size;
	buffer->read_avail -= to_read;
	return to_read;
}

/**
 * Writes up to a given number of bytes from a local buffer
 * returns: The amount of bytes written
 */
size_t buffer_write(buffer_t* buffer, const unsigned char* buf, size_t len)
{
	size_t to_write = min(len, buffer_write_avail(buffer));
	int write_ptr = (buffer->read_ptr + buffer->read_avail) % buffer->real_size;
	for (size_t i = 0; i < to_write; i++) {
		buffer->data[(write_ptr + i) % buffer->real_size] = buf[i];
	}
	buffer->read_avail += to_write;
	return to_write;
}

/**
 * Writes the entire contents (and only the entire contents)
 * of a codec to a buffer_t
 * returns: Whether the operation was a success
 */
bool codec_buffer_write(codec_t* codec, buffer_t* buffer)
{
	buffer_pushp(buffer);
	if (buffer_write(buffer, codec->data, codec_len(codec)) < codec_len(codec)) {
		buffer_popp(buffer);
		return false;
	}
	buffer_dropp(buffer);
	return true;
}

/**
 * Reads a given amount from a buffer_t into a codec
 * returns: Whether the operation was a success
 */
bool codec_buffer_read(codec_t* codec, buffer_t* buffer, size_t len)
{
	if (codec->caret+len > DEFAULT_BUFFER_SIZE) {
		return false;
	}

	buffer_pushp(buffer);
	if (buffer_read(buffer, codec->data+codec->caret, len) < len) {
		buffer_popp(buffer);
		return false;
	}
	buffer_dropp(buffer);
	return true;
}

/**
 * Print debugging info for a given buffer
 */
void buffer_print(buffer_t* buffer)
{
	int write_ptr = (buffer->read_ptr + buffer->read_avail) % buffer->real_size;
	printf("read_ptr: %d, write_ptr: %d, read_avail: %d, write_avail: %d\n", buffer->read_ptr, write_ptr, (int)buffer_read_avail(buffer), (int)buffer_write_avail(buffer));
}

/**
 * Stores the current read and write pointers
 */
void buffer_pushp(buffer_t* buffer)
{
	stack_push(&buffer->ptr_stack, buffer->read_ptr);
	stack_push(&buffer->ptr_stack, buffer->read_avail);
}

/**
 * Restores the stored read and write pointers
 */
void buffer_popp(buffer_t* buffer)
{
	buffer->read_avail = stack_pop(&buffer->ptr_stack);
	buffer->read_ptr = stack_pop(&buffer->ptr_stack);
}

/**
 * Drops the previous set of read and write pointers
 */
void buffer_dropp(buffer_t* buffer)
{
	stack_pop(&buffer->ptr_stack);
	stack_pop(&buffer->ptr_stack);
}

object_proto_t buffer_proto = {
	.init = (object_init_t)buffer_init,
	.free = (object_free_t)buffer_free
};
