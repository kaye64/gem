/**
 * buffer.c
 *
 * A circular buffer implementation, intended for io buffering
 */
#include <net/buffer.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <util/math.h>

/**
 * buffer_create
 *
 * Initializes a new buffer
 *  - buffer: Some preallocated memory, or null to put on heap
 *  - size: The size of the buffer
 * returns: The initialized buffer
 */
buffer_t* buffer_create(buffer_t* buffer, size_t size)
{
	if (buffer == NULL) {
		buffer = (buffer_t*)malloc(sizeof(buffer_t));
		buffer->must_free = true;
	} else {
		buffer->must_free = false;
	}
	buffer->data = (char*)malloc(sizeof(char)*size);
	buffer->real_size = size;
	buffer->read_ptr = buffer->prev_read_ptr = 0;
	buffer->read_avail = buffer->prev_read_avail = 0;
	return buffer;
}

/**
 * buffer_free
 *
 * Properly frees a buffer
 *  - buffer: The buffer to free
 */
void buffer_free(buffer_t* buffer)
{
	free(buffer->data);
	if (buffer->must_free) {
		free(buffer);
	}
}

/**
 * buffer_read_avail
 *
 * Returns the number of bytes available to read in a buffer
 *  - buffer: The buffer
 * returns: The number of bytes
 */
size_t buffer_read_avail(buffer_t* buffer)
{
	return buffer->read_avail;
}

/**
 * buffer_write_avail
 *
 * Returns the number of bytes available to write in a buffer
 *  - buffer: The buffer
 * returns: The number of bytes
 */
size_t buffer_write_avail(buffer_t* buffer)
{
	return buffer->real_size - buffer->read_avail;
}

/**
 * buffer_read
 *
 * Reads up to a given number of bytes into a local buffer
 *  - buffer: The buffer to read from
 *  - buf: The local buffer to read the data into
 *  - len: The amount of bytes to read up to
 * returns: The amount of bytes read
 */
size_t buffer_read(buffer_t* buffer, char* buf, size_t len)
{
	size_t to_read = min(len, buffer_read_avail(buffer));
	for (int i = 0; i < to_read; i++) {
		buf[i] = buffer->data[(buffer->read_ptr + i) % buffer->real_size];
	}
	buffer->read_ptr = (buffer->read_ptr + to_read) % buffer->real_size;
	buffer->read_avail -= to_read;
	return to_read;
}

/**
 * buffer_write
 *
 * Writes up to a given number of bytes from a local buffer
 *  - buffer: The buffer to write to
 *  - buf: The local buffer to write the data from
 *  - len: The amount of bytes to write up to
 * returns: The amount of bytes written
 */
size_t buffer_write(buffer_t* buffer, const char* buf, size_t len)
{
	size_t to_write = min(len, buffer_write_avail(buffer));
	int write_ptr = (buffer->read_ptr + buffer->read_avail) % buffer->real_size;
	for (int i = 0; i < to_write; i++) {
		buffer->data[(write_ptr + i) % buffer->real_size] = buf[i];
	}
	buffer->read_avail += to_write;
	return to_write;
}

/**
 * buffer_print
 *
 * Print debugging info for a given buffer
 *  - buffer: The buffer
 */
void buffer_print(buffer_t* buffer)
{
	int write_ptr = (buffer->read_ptr + buffer->read_avail) % buffer->real_size;
	printf("read_ptr: %d, write_ptr: %d, read_avail: %d, write_avail: %d\n", buffer->read_ptr, write_ptr, (int)buffer_read_avail(buffer), (int)buffer_write_avail(buffer));
}

/**
 * buffer_pushp
 *
 * Stores the current read and write pointers
 *  - buffer: The buffer
 */
void buffer_pushp(buffer_t* buffer)
{
	buffer->prev_read_ptr = buffer->read_ptr;
	buffer->prev_read_avail = buffer->read_avail;
}

/**
 * buffer_popp
 *
 * Restores the stored read and write pointers
 *  - buffer: The buffer
 */
void buffer_popp(buffer_t* buffer)
{
	buffer->read_ptr = buffer->prev_read_ptr;
	buffer->read_avail = buffer->prev_read_avail;
}
