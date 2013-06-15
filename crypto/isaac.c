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

void isaac_init(isaac_t* isaac);
void isaac_generate_set(isaac_t* isaac);

/**
 * isaac_create
 *
 * Initializes a new isaac cipher
 *	- isaac: Some preallocated memory, or null to put on heap
 *	- seed: A set of values to seed the cipher
 *	- seed_len: The length of the seed values
 * returns: The initialized isaac cipher
 */
isaac_t* isaac_create(isaac_t* isaac, uint32_t* seed, int seed_len)
{
	if (isaac == NULL) {
		isaac = (isaac_t*)malloc(sizeof(isaac_t));
		isaac->must_free = true;
	} else {
		isaac->must_free = false;
	}
	// Init to zero
	memset(&isaac->results, 0, 256);
	memset(&isaac->state, 0, 256);
	isaac->result_idx = 0;
	isaac->state_a = 0;
	isaac->state_b = 0;
	isaac->state_c = 0;

	// Seed
	for (int i = 0; i < seed_len; i++) {
		isaac->results[i] = seed[i];
	}

	// Generate first result set
	isaac_init(isaac);
	return isaac;
}

/**
 * isaac_free
 *
 * Properly frees an isaac cipher
 *	- isaac: The isaac cipher
 */
void isaac_free(isaac_t* isaac)
{
	if (isaac->must_free) {
		free(isaac);
	}
}

/**
 * isaac_init
 *
 * Initializes the cipher and generates the first 256 values
 *	- isaac: The isaac cipher
 */
void isaac_init(isaac_t* isaac)
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
 * isaac_generate_set
 *
 * Generates a set of 256 values
 *	- isaac: The isaac cipher
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
 * isaac_next
 *
 * Returns the next value, generating a new set if necessary
 *	- isaac: The isaac cipher
 * returns: The next cipher value
 */
uint32_t isaac_next(isaac_t* isaac)
{
	if (isaac->result_idx-- == 0) {
		isaac_generate_set(isaac);
		isaac->result_idx = 255;
	}
	return isaac->results[isaac->result_idx];
}
