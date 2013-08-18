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
 * entity.c
 *
 * An entity has a unique index, a position, and 'known' sector (the
 * sector the entity was last registered in)
 */
#include <game/entity.h>

/**
 * Creates and initializes an entity_t
 */
static void entity_init(entity_t* entity)
{
	entity_warp_to(entity, absolute_coord(0, 0, 0));
}

/**
 * Frees an entity_t
 */
static void entity_free(entity_t* entity)
{

}

/**
 * Changes the entity's position
 */
void entity_warp_to(entity_t* entity, location_t position)
{
	entity->position = position;
	entity->known_sector = position.sector;
}

/**
 * Returns the entity's position
 */
location_t entity_position(entity_t* entity)
{
	return entity->position;
}

object_proto_t entity_proto = {
	.init = (object_init_t)entity_init,
	.free = (object_free_t)entity_free
};
