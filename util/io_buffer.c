#include <util/io_buffer.h>

#include <string.h>
#include <stdlib.h>

#define min(a, b) ((a) > (b) ? (b) : (a))

io_buffer_t* io_buf_create(io_buffer_t* buffer, size_t size)
{
	if (buffer == NULL) {
		buffer = (io_buffer_t*)malloc(sizeof(io_buffer_t));
		buffer->must_free = 1;
	} else {
		buffer->must_free = 0;
	}
	buffer->buffer = (char*)malloc(sizeof(char)*size);
	buffer->real_size = size;
	buffer->read_ptr = buffer->prev_read_ptr = 0;
	buffer->write_ptr = buffer->prev_write_ptr = 0;
	return buffer;
}

void io_buf_free(io_buffer_t* buffer)
{
	free(buffer->buffer);
	if (buffer->must_free) {
		free(buffer);
	}
}

size_t io_buf_read_avail(io_buffer_t* buffer)
{
	if (buffer->read_ptr > buffer->write_ptr) {
		return buffer->real_size - (buffer->read_ptr - buffer->write_ptr);
	}
	return buffer->write_ptr - buffer->read_ptr;
}

size_t io_buf_write_avail(io_buffer_t* buffer)
{
	if (buffer->write_ptr >= buffer->read_ptr) {
		return buffer->real_size - (buffer->write_ptr - buffer->read_ptr);
	}
	return buffer->read_ptr - buffer->write_ptr;
}

size_t io_buf_read(io_buffer_t* buffer, char* buf, size_t len)
{
	size_t read_total = min(len, io_buf_read_avail(buffer));
	size_t amount_end_of_buffer = buffer->real_size - buffer->read_ptr;
	size_t amount_read = 0;
	if (read_total > amount_end_of_buffer) {
		memcpy(buf+amount_read, buffer->buffer+buffer->read_ptr, amount_end_of_buffer);
		amount_read += amount_end_of_buffer;
		buffer->read_ptr = 0;
	}
	memcpy(buf+amount_read, buffer->buffer+buffer->read_ptr, read_total-amount_read);
	buffer->read_ptr += (read_total-amount_read);
	return read_total;
}

size_t io_buf_write(io_buffer_t* buffer, const char* buf, size_t len)
{
	size_t write_total = min(len, io_buf_write_avail(buffer));
	size_t amount_end_of_buffer = buffer->real_size - buffer->write_ptr;
	size_t amount_written = 0;
	if (write_total > amount_end_of_buffer) {
		memcpy(buffer->buffer+buffer->write_ptr, buf+amount_written, amount_end_of_buffer);
		amount_written += amount_end_of_buffer;
		buffer->write_ptr = 0;
	}
	memcpy(buffer->buffer+buffer->write_ptr, buf+amount_written, write_total-amount_written);
	buffer->write_ptr += (write_total-amount_written);
	return write_total;
}

void io_buf_pushw(io_buffer_t* buffer)
{
	buffer->prev_write_ptr = buffer->write_ptr;
}

void io_buf_pushr(io_buffer_t* buffer)
{
	buffer->prev_read_ptr = buffer->read_ptr;
}

void io_buf_popw(io_buffer_t* buffer)
{
	buffer->write_ptr = buffer->prev_write_ptr;
}

void io_buf_popr(io_buffer_t* buffer)
{
	buffer->read_ptr = buffer->prev_read_ptr;
}
