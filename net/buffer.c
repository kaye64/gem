#include <net/buffer.h>

#include <string.h>
#include <stdlib.h>

#define min(a, b) ((a) > (b) ? (b) : (a))

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

void buffer_free(buffer_t* buffer)
{
	free(buffer->data);
	if (buffer->must_free) {
		free(buffer);
	}
}

size_t buffer_read_avail(buffer_t* buffer)
{
	return buffer->read_avail;
}

size_t buffer_write_avail(buffer_t* buffer)
{
	return buffer->real_size - buffer->read_avail;
}

size_t buffer_read(buffer_t* buffer, char* buf, size_t len)
{
	size_t to_read = min(len, buffer_read_avail(buffer));
	size_t total_read = to_read;
	if (buffer->read_ptr + to_read > buffer->real_size) {
		// We have to wrap around halfway through
		size_t partial_read_amt = buffer->real_size - buffer->read_ptr;
		buffer_read(buffer, buf, partial_read_amt);
		to_read -= partial_read_amt;
		buf += partial_read_amt;
		buffer->read_ptr = 0;
	}
	memcpy(buf, buffer->data+buffer->read_ptr, to_read);
	buffer->read_avail -= to_read;
	buffer->read_ptr += to_read;
	return total_read;
}

size_t buffer_write(buffer_t* buffer, const char* buf, size_t len)
{
	size_t to_write = min(len, buffer_write_avail(buffer));
	size_t total_write = to_write;
	int write_ptr = buffer->read_ptr + buffer->read_avail;
	if (write_ptr + to_write > buffer->real_size) {
		// We have to wrap around halfway through
		size_t partial_write_amt = buffer->real_size - write_ptr;
		buffer_write(buffer, buf, partial_write_amt);
		write_ptr = 0;
		buf += partial_write_amt;
		to_write -= partial_write_amt;
	}
	memcpy(buffer->data+write_ptr, buf, to_write);
	buffer->read_avail += to_write;
	return total_write;
}

void buffer_pushp(buffer_t* buffer)
{
	buffer->prev_read_ptr = buffer->read_ptr;
	buffer->prev_read_avail = buffer->read_avail;
}

void buffer_popp(buffer_t* buffer)
{
	buffer->read_ptr = buffer->prev_read_ptr;
	buffer->read_avail = buffer->prev_read_avail;
}
