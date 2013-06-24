#ifndef _LOCATION_H_
#define _LOCATION_H_

#define AREA_GRANULARITY 8
#define REGION_GRANULARITY 13

/**
 * A sector is an 8x8 tile plot
 */
struct sector {
	int x;
	int y;
};
typedef struct sector sector_t;

/**
 * A region is a 13x13 sector (104x104 tile) plot. This is the
 * loaded area of the map around the player. As the player moves
 * between sectors, the region is rebased to make the player's
 * current sector the center of their region.
 * The base of the region is the index of the lowest corner sector.
 */
struct region {
	sector_t base;
};
typedef struct region region_t;

/**
 * A location contains the world absolute coordinates, and the sector
 * those coordinates belong to.
 */
struct location {
	int x;
	int y;
	int z;
	sector_t sector;
};
typedef struct location location_t;

/**
 * Region local coordinates are the coordinates relative to the base
 * of a region.
 */
struct region_local {
	int x;
	int y;
	int z;
	region_t region;
};
typedef struct region_local region_local_t;

location_t absolute_coord(int x, int y, int z);
location_t to_absolute_coord(region_local_t local);
region_t center_region_on(location_t loc);
region_local_t local_coord(location_t loc, region_t region);

#endif /* _LOCATION_H_ */
