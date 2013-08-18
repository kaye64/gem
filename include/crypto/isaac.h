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
