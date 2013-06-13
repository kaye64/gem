/**
 * buffer.c
 *
 * A circular buffer implementation, intended for io buffering
 */
#include <net/buffer.h>

#include <string.h>
#include <stdlib.h>

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
		buffer->must_free = 1;
	} else {
		buffer->must_free = 0;
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
	size_t total_read = to_read;
	int write_ofs = 0;
	if (buffer->read_ptr + to_read > buffer->real_size) {
		// We have to wrap around halfway through
		size_t partial_read_amt = buffer->real_size - buffer->read_ptr;
		buffer_read(buffer, buf, partial_read_amt);
		buffer->read_ptr = 0;
		write_ofs = partial_read_amt;
	}
	memcpy(buf+write_ofs, buffer->data+buffer->read_ptr, to_read-write_ofs);
	buffer->read_avail -= to_read;
	buffer->read_ptr += to_read;
	return total_read;
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
	size_t total_write = to_write;
	int write_ptr = buffer->read_ptr + buffer->read_avail;
	int read_ofs = 0;
	if (write_ptr + to_write > buffer->real_size) {
		// We have to wrap around halfway through
		size_t partial_write_amt = buffer->real_size - write_ptr;
		buffer_write(buffer, buf, partial_write_amt);
		write_ptr = 0;
		read_ofs = partial_write_amt;
	}
	memcpy(buffer->data+write_ptr, buf+read_ofs, to_write-read_ofs);
	buffer->read_avail += to_write;
	return total_write;
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
