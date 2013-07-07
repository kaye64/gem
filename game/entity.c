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
