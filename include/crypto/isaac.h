#ifndef _ISAAC_H_
#define _ISAAC_H_

#include <stdbool.h>
#include <stdint.h>

#include <util/object.h>

typedef struct isaac isaac_t;

struct isaac {
	object_t object;
	uint32_t results[256];
	uint32_t state[256];
	uint32_t result_idx;
	uint32_t state_a;
	uint32_t state_b;
	uint32_t state_c;
};

extern object_proto_t isaac_proto;

void isaac_seed(isaac_t* isaac, uint32_t* seed, int seed_len);
uint32_t isaac_next(isaac_t* isaac);

#endif /* _ISAAC_H_ */
