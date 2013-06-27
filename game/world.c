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

void world_create(world_t* world)
{
	for (int x = 0; x < NUM_SECTORS_X; x++) {
		for (int y = 0; y < NUM_SECTORS_Y; y++) {
			for (int z = 0; z < 4; z++) {
				world->sectors[x][y][z] = (world_sector_t*)NULL;
			}
		}
	}
}

void world_free(world_t* world)
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

world_sector_t* world_get_sector(world_t* world, sector_t sector)
{
	if (world->sectors[sector.x][sector.y][sector.z] == (world_sector_t*)NULL) {
		world->sectors[sector.x][sector.y][sector.z] = sector_create(sector);
	}
	return world->sectors[sector.x][sector.y][sector.z];
}

void world_gc(world_t* world)
{

}


world_sector_t* sector_create(sector_t sector)
{
	world_sector_t* world_sector = (world_sector_t*)malloc(sizeof(world_sector_t));
	world_sector->sector = sector;
	list_create(&world_sector->players);
	return world_sector;
}

void sector_free(world_sector_t* sector)
{
	list_free(&sector->players);
	free(sector);
}

void sector_register_player(world_sector_t* sector, player_t* player)
{
	list_push_back(&sector->players, &player->world_node);
}

void sector_unregister_player(world_sector_t* sector, player_t* player)
{
	list_erase(&sector->players, &player->world_node);
}
