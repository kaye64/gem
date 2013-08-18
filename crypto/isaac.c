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
 * isaac.c
 *
 * Defines the isaac cipher
 */
#include <crypto/isaac.h>

#include <stdlib.h>
#include <string.h>

#define mix(a, b, c, d, e, f, g, h)				\
	{											\
		h ^= a << 11; c += h; a += b;			\
		a ^= b >> 2; d += a; b += c;			\
		b ^= c << 8; e += b; c += d;			\
		c ^= d >> 16; f += c; d += e;			\
		d ^= e << 10; g += d; e += f;			\
		e ^= f >> 4; h += e; f += g;			\
		f ^= g << 8; a += f; g += h;			\
		g ^= h >> 9; b += g; h += a;			\
	}											\

void isaac_init_cipher(isaac_t* isaac);
void isaac_generate_set(isaac_t* isaac);

/**
 * Initializes a new isaac cipher
 */
static void isaac_init(isaac_t* isaac)
{
	// Init to zero
	memset(&isaac->results, 0, 256*sizeof(uint32_t));
	memset(&isaac->state, 0, 256*sizeof(uint32_t));
	isaac->result_idx = 0;
	isaac->state_a = 0;
	isaac->state_b = 0;
	isaac->state_c = 0;
}

/**
 * Properly frees an isaac cipher
 */
static void isaac_free(isaac_t* isaac)
{

}

/**
 * Seeds an isaac cipher
 */
void isaac_seed(isaac_t* isaac, uint32_t* seed, int seed_len)
{
	// Seed
	for (int i = 0; i < seed_len; i++) {
		isaac->results[i] = seed[i];
	}

	// Generate first result set
	isaac_init_cipher(isaac);
}

/**
 * Initializes the cipher and generates the first 256 values
 */
void isaac_init_cipher(isaac_t* isaac)
{
	uint32_t a, b, c, d, e, f, g, h;
	a = b = c = d = e = f = g = h = 0x9e3779b9;
	for(int i = 0; i < 4; i++)
	{
		mix(a,b,c,d,e,f,g,h);
	}

	for(int j = 0; j < 256; j += 8)
	{
		h += isaac->results[j];
		a += isaac->results[j + 1];
		b += isaac->results[j + 2];
		c += isaac->results[j + 3];
		d += isaac->results[j + 4];
		e += isaac->results[j + 5];
		f += isaac->results[j + 6];
		g += isaac->results[j + 7];
		mix(a,b,c,d,e,f,g,h);
		isaac->state[j] = h;
		isaac->state[j + 1] = a;
		isaac->state[j + 2] = b;
		isaac->state[j + 3] = c;
		isaac->state[j + 4] = d;
		isaac->state[j + 5] = e;
		isaac->state[j + 6] = f;
		isaac->state[j + 7] = g;
	}

	for(int k = 0; k < 256; k += 8)
	{
		h += isaac->state[k];
		a += isaac->state[k + 1];
		b += isaac->state[k + 2];
		c += isaac->state[k + 3];
		d += isaac->state[k + 4];
		e += isaac->state[k + 5];
		f += isaac->state[k + 6];
		g += isaac->state[k + 7];
		mix(a,b,c,d,e,f,g,h);
		isaac->state[k] = h;
		isaac->state[k + 1] = a;
		isaac->state[k + 2] = b;
		isaac->state[k + 3] = c;
		isaac->state[k + 4] = d;
		isaac->state[k + 5] = e;
		isaac->state[k + 6] = f;
		isaac->state[k + 7] = g;
	}

	isaac_generate_set(isaac);
	isaac->result_idx = 256;
}

/**
 * Generates a set of 256 values
 */
void isaac_generate_set(isaac_t* isaac)
{
	uint32_t x, y;
	isaac->state_b += ++isaac->state_c;
	for(int i = 0; i < 256; i++) {
		x = isaac->state[i];
		switch (i & 3) {
		case 0:	isaac->state_a ^= isaac->state_a << 13;	break;
		case 1:	isaac->state_a ^= isaac->state_a >> 6;	break;
		case 2:	isaac->state_a ^= isaac->state_a << 2;	break;
		case 3:	isaac->state_a ^= isaac->state_a >> 16;	break;
		}
		isaac->state_a += isaac->state[(i + 128) % 256];
		isaac->state[i] = y = isaac->state[(x >> 2) % 256] + isaac->state_a + isaac->state_b;
		isaac->results[i] = isaac->state_b = isaac->state[(y >> 10) % 256] + x;
	}
}

/**
 * Returns the next value, generating a new set if necessary
 */
uint32_t isaac_next(isaac_t* isaac)
{
	if (isaac->result_idx-- == 0) {
		isaac_generate_set(isaac);
		isaac->result_idx = 255;
	}
	return isaac->results[isaac->result_idx];
}

object_proto_t isaac_proto = {
	.init = (object_init_t)isaac_init,
	.free = (object_free_t)isaac_free
};
