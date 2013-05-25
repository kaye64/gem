#ifndef _IO_BUFFER_H_
#define _IO_BUFFER_H_

#include <stddef.h>
#include <stdint.h>

/* A circular buffer designed for use in network IO. */
struct buffer {
	char* buffer;
	int read_ptr;
	int write_ptr;
	int prev_read_ptr;
	int prev_write_ptr;
	size_t real_size;
	int must_free;
};
typedef struct buffer buffer_t;

buffer_t* buffer_create(buffer_t* buffer, size_t size);
void buffer_free(buffer_t* buffer);

size_t buffer_read_avail(buffer_t* buffer);
size_t buffer_write_avail(buffer_t* buffer);
size_t buffer_read(buffer_t* buffer, char* buf, size_t len);
size_t buffer_write(buffer_t* buffer, const char* buf, size_t len);

void buffer_pushw(buffer_t* buffer);
void buffer_pushr(buffer_t* buffer);
void buffer_popw(buffer_t* buffer);
void buffer_popr(buffer_t* buffer);

#endif /* _IO_BUFFER_H_ */
