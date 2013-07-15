#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <stdbool.h>
#include <stdint.h>

#include <util/object.h>

typedef struct bitmap bitmap_t;

struct bitmap {
	object_t object;
	uint8_t* bitmap;
	int bit_size;
};

extern object_proto_t bitmap_proto;

void bitmap_alloc(bitmap_t* bitmap, int bit_size);
void bitmap_set(bitmap_t* bitmap, int bit, bool val);
bool bitmap_get(bitmap_t* bitmap, int bit);
int bitmap_find_next(bitmap_t* bitmap, int start_bit, bool val);
void bitmap_clear(bitmap_t* bitmap);

#endif /* _BITMAP_H_ */
