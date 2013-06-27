#ifndef _WORLD_H_
#define _WORLD_H_

#include <util/list.h>
#include <game/location.h>

/* The cache contains 60x162 map chunks, each containing 8 sectors */
#define NUM_SECTORS_X 60*8
#define NUM_SECTORS_Y 162*8

/* These are the indices of the array returned by world_get_local_sectors */
#define LOCAL_SECTOR_NW 0
#define LOCAL_SECTOR_N 1
#define LOCAL_SECTOR_NE 2
#define LOCAL_SECTOR_W 3
#define LOCAL_SECTOR_C 4
#define LOCAL_SECTOR_E 5
#define LOCAL_SECTOR_SW 6
#define LOCAL_SECTOR_S 7
#define LOCAL_SECTOR_SE 8

typedef struct player player_t;

struct world_sector {
	sector_t sector;
	list_t players;
};
typedef struct world_sector world_sector_t;

struct world {
	world_sector_t* sectors[NUM_SECTORS_X][NUM_SECTORS_Y][4];
};
typedef struct world world_t;

void world_create(world_t* world);
void world_free(world_t* world);

world_sector_t** world_get_local_sectors(world_t* world, sector_t center);
world_sector_t* world_get_sector(world_t* world, sector_t sector);
void world_gc(world_t* world);

void sector_register_player(world_sector_t* sector, player_t* player);
void sector_unregister_player(world_sector_t* sector, player_t* player);

#endif /* _WORLD_H_ */
