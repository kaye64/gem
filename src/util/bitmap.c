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
 * bitmap.c
 *
 * A memory efficient bitmap implementation
 */
#include <util/bitmap.h>

#include <stdlib.h>
#include <assert.h>

/**
 * Initializes a bitmap_t
 */
static void bitmap_init(bitmap_t* bitmap)
{
	bitmap->bitmap = NULL;
	bitmap->bit_size = 0;
}

/**
 * Frees a bitmap_t
 */
static void bitmap_free(bitmap_t* bitmap)
{
	if (bitmap->bitmap != NULL) {
		free(bitmap->bitmap);
	}
}

/**
 * Allocates size for a bitmap_t
 */
void bitmap_alloc(bitmap_t* bitmap, int bit_size)
{
	bitmap->bit_size = (bit_size / 8) + 1;
	bitmap->bitmap = (uint8_t*)malloc(bitmap->bit_size);
}

/**
 * Clears all bits in a bitmap_t to 0
 */
void bitmap_clear(bitmap_t* bitmap)
{
	int byte_pos = 0;
	while (byte_pos < (bitmap->bit_size/8)) {
		bitmap->bitmap[byte_pos++] = 0;
	}
}

/**
 * Sets a bit in a bitmap_t
 */
void bitmap_set(bitmap_t* bitmap, int bit, bool val)
{
	assert(bit <= bitmap->bit_size);
	int byte_pos = bit / 8;
	int bit_pos = bit % 8;
	if (val) {
		bitmap->bitmap[byte_pos] |= (1 << bit_pos);
	} else {
		bitmap->bitmap[byte_pos] &= ~(1 << bit_pos);
	}
}

/**
 * Tests a bit in a bitmap_t
 */
bool bitmap_get(bitmap_t* bitmap, int bit)
{
	assert(bit <= bitmap->bit_size);
	int byte_pos = bit / 8;
	int bit_pos = bit % 8;
	return bitmap->bitmap[byte_pos] & (1 << bit_pos);
}

/**
 * Finds the next bit matching val, starting from start_bit
 * returns -1 if none are found
 */
int bitmap_find_next(bitmap_t* bitmap, int start_bit, bool val)
{
	assert(start_bit <= bitmap->bit_size);
	int byte_pos = start_bit / 8;
	int bit_pos = start_bit % 8;
	while (byte_pos < (bitmap->bit_size/8)) {
		if (bitmap->bitmap[byte_pos] == (val ? 0x0 : 0xFF)) { // whole byte is invalid
			byte_pos++;
			bit_pos = 0;
			continue;
		}
		if ((bitmap->bitmap[byte_pos] & (1 << bit_pos)) == val) { // valid bit
			return (byte_pos * 8) + bit_pos;
		}
		if (++bit_pos > 7) { // next byte
			byte_pos++;
			bit_pos = 0;
		}
	}
	return -1;
}

object_proto_t bitmap_proto = {
	.init = (object_init_t)bitmap_init,
	.free = (object_free_t)bitmap_free
};
