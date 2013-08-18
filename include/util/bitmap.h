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
