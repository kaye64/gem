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

#ifndef _WORLD_H_
#define _WORLD_H_

#include <util/object.h>
#include <util/list.h>
#include <game/location.h>

/* The cache contains 60x162 map chunks, each containing 8 sectors */
#define NUM_SECTORS_X 60*8
#define NUM_SECTORS_Y 162*8

#define NUM_OBSERVED_SECTORS 25

typedef struct player player_t;
typedef struct world_sector world_sector_t;
typedef struct world world_t;

struct world_sector {
	sector_t sector;
	list_t players;
};

struct world {
	object_t object;
	world_sector_t* sectors[NUM_SECTORS_X][NUM_SECTORS_Y][4];
};

extern object_proto_t world_proto;

world_sector_t** world_get_observed_sectors(world_t* world, sector_t center);
world_sector_t* world_get_sector(world_t* world, sector_t sector);
void world_gc(world_t* world);

void sector_register_player(world_t* world, world_sector_t* sector, player_t* player);
void sector_unregister_player(world_t* world, world_sector_t* sector, player_t* player);

#endif /* _WORLD_H_ */
