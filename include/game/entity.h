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

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <stdint.h>

#include <util/object.h>
#include <game/location.h>

typedef struct entity entity_t;
typedef uint16_t entity_index;
#define ENTITY_INDEX_MAX UINT16_MAX // (65k)

#define ENTITY_OBSERVE_RADIUS 15

struct entity {
	object_t object;
	entity_index index;
	location_t position;
	sector_t known_sector;
};

extern object_proto_t entity_proto;

void entity_warp_to(entity_t* entity, location_t position);
location_t entity_position(entity_t* entity);

#endif /* _ENTITY_H_ */
