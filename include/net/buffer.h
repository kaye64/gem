#ifndef _IO_BUFFER_H_
#define _IO_BUFFER_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <util/stack.h>

#include <util/object.h>

typedef struct buffer buffer_t;

/* A circular buffer designed for use in network IO. */
struct buffer {
	object_t object;
	unsigned char* data;
	int read_ptr;
	int read_avail;
	int_stack_t ptr_stack;
	size_t real_size;
};

extern object_proto_t buffer_proto;

void buffer_realloc(buffer_t* buffer, size_t size);

size_t buffer_read_avail(buffer_t* buffer);
size_t buffer_write_avail(buffer_t* buffer);
size_t buffer_read(buffer_t* buffer, unsigned char* buf, size_t len);
size_t buffer_write(buffer_t* buffer, const unsigned char* buf, size_t len);

void buffer_print(buffer_t* buffer);

void buffer_pushp(buffer_t* buffer);
void buffer_popp(buffer_t* buffer);
void buffer_dropp(buffer_t* buffer);

#endif /* _IO_BUFFER_H_ */
