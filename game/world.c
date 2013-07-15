/**
 * world.c
 *
 * This represents the state of the entire game world.
 * We do this by keeping track of all the sectors which have entities
 * occupying them, saving memory by only keeping sectors which we have
 * entities in memory.
 */
#include <game/world.h>

#include <game/player.h>

world_sector_t* sector_create(sector_t sector);
void sector_free(world_sector_t* sector);

/**
 * Initializes a world_t
 */
static void world_init(world_t* world)
{
	for (int x = 0; x < NUM_SECTORS_X; x++) {
		for (int y = 0; y < NUM_SECTORS_Y; y++) {
			for (int z = 0; z < 4; z++) {
				world->sectors[x][y][z] = (world_sector_t*)NULL;
			}
		}
	}
}

/**
 * Cleans up a world_t for exit
 */
static void world_free(world_t* world)
{
	for (int x = 0; x < NUM_SECTORS_X; x++) {
		for (int y = 0; y < NUM_SECTORS_Y; y++) {
			for (int z = 0; z < 4; z++) {
				if (world->sectors[x][y][z] != (world_sector_t*)NULL) {
					sector_free(world->sectors[x][y][z]);
					world->sectors[x][y][z] = (world_sector_t*)NULL;
				}
			}
		}
	}
}

/**
 * Returns the 8 sectors surrounding a center sector.
 *  - center: The center sector
 * returns: An array of sectors. Indices are LOCAL_SECTOR_*
 */
world_sector_t** world_get_local_sectors(world_t* world, sector_t center)
{
	world_sector_t** local_sectors = malloc(sizeof(world_sector_t*)*9);
	local_sectors[LOCAL_SECTOR_NW] = world_get_sector(world, sector(center.x-1, center.y+1, center.z));
	local_sectors[LOCAL_SECTOR_N] = world_get_sector(world, sector(center.x, center.y+1, center.z));
	local_sectors[LOCAL_SECTOR_NE] = world_get_sector(world, sector(center.x+1, center.y+1, center.z));
	local_sectors[LOCAL_SECTOR_W] = world_get_sector(world, sector(center.x-1, center.y, center.z));
	local_sectors[LOCAL_SECTOR_C] = world_get_sector(world, center);
	local_sectors[LOCAL_SECTOR_E] = world_get_sector(world, sector(center.x+1, center.y, center.z));
	local_sectors[LOCAL_SECTOR_SW] = world_get_sector(world, sector(center.x-1, center.y-1, center.z));
	local_sectors[LOCAL_SECTOR_S] = world_get_sector(world, sector(center.x, center.y-1, center.z));
	local_sectors[LOCAL_SECTOR_SE] = world_get_sector(world, sector(center.x+1, center.y-1, center.z));
	return local_sectors;
}

/**
 * Accesses a sector from a set of sector coords (sector_t)
 */
world_sector_t* world_get_sector(world_t* world, sector_t sector)
{
	if (world->sectors[sector.x][sector.y][sector.z] == (world_sector_t*)NULL) {
		world->sectors[sector.x][sector.y][sector.z] = sector_create(sector);
	}
	return world->sectors[sector.x][sector.y][sector.z];
}

/**
 * Performs garbage collection on the world, cleaning and deallocating any
 * un-used sectors.
 */
void world_gc(world_t* world)
{
	for (int x = 0; x < NUM_SECTORS_X; x++) {
		for (int y = 0; y < NUM_SECTORS_Y; y++) {
			for (int z = 0; z < 4; z++) {
				world_sector_t* sector = world->sectors[x][y][z];
				if (sector != (world_sector_t*)NULL) {
					if (list_empty(&sector->players)) {
						sector_free(sector);
						world->sectors[x][y][z] = (world_sector_t*)NULL;
					}
				}
			}
		}
	}
}

/**
 * Initializes a sector from a sector_t
 */
world_sector_t* sector_create(sector_t sector)
{
	world_sector_t* world_sector = (world_sector_t*)malloc(sizeof(world_sector_t));
	world_sector->sector = sector;
	object_init(list, &world_sector->players);
	return world_sector;
}

/**
 * Deallocates a sector
 */
void sector_free(world_sector_t* sector)
{
	object_free(&sector->players);
	free(sector);
}

/**
 * Registers a player to a sector
 */
void sector_register_player(world_sector_t* sector, player_t* player)
{
	list_push_back(&sector->players, &player->world_node);
}

/**
 * Unregisters a player from a sector
 */
void sector_unregister_player(world_sector_t* sector, player_t* player)
{
	list_erase(&sector->players, &player->world_node);
}

object_proto_t world_proto = {
	.init = (object_init_t)world_init,
	.free = (object_free_t)world_free
};