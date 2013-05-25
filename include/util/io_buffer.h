#ifndef _IO_BUFFER_H_
#define _IO_BUFFER_H_

#include <stddef.h>
#include <stdint.h>

/* A circular buffer designed for use in network IO. */
struct io_buffer {
	char* buffer;
	int read_ptr;
	int write_ptr;
	int prev_read_ptr;
	int prev_write_ptr;
	size_t real_size;
	int must_free;
};
typedef struct io_buffer io_buffer_t;

io_buffer_t* io_buf_create(io_buffer_t* buffer, size_t size);
void io_buf_free(io_buffer_t* buffer);

size_t io_buf_read_avail(io_buffer_t* buffer);
size_t io_buf_write_avail(io_buffer_t* buffer);
size_t io_buf_read(io_buffer_t* buffer, char* buf, size_t len);
size_t io_buf_write(io_buffer_t* buffer, const char* buf, size_t len);

void io_buf_pushw(io_buffer_t* buffer);
void io_buf_pushr(io_buffer_t* buffer);
void io_buf_popw(io_buffer_t* buffer);
void io_buf_popr(io_buffer_t* buffer);

#endif /* _IO_BUFFER_H_ */
