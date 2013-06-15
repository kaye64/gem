#ifndef _ISAAC_H_
#define _ISAAC_H_

#include <stdbool.h>
#include <stdint.h>

struct isaac {
	uint32_t results[256];
	uint32_t state[256];
	uint32_t result_idx;
	uint32_t state_a;
	uint32_t state_b;
	uint32_t state_c;
	bool must_free;
};
typedef struct isaac isaac_t;

isaac_t* isaac_create(isaac_t* isaac, uint32_t* seed, int seed_len);
void isaac_free(isaac_t* isaac);

uint32_t isaac_next(isaac_t* isaac);

#endif /* _ISAAC_H_ */
