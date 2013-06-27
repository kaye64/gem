#include <game/location.h>

/**
 * absolute_coord
 *
 * Constructs a location_t from absolute x, y, and z
 *  - x, y, z: The absolute coordinates
 * returns: The location
 */
location_t absolute_coord(int x, int y, int z)
{
	location_t location = { .x = x, .y = y, .z = z };
	location.sector = sector(location.x / AREA_GRANULARITY, location.y / AREA_GRANULARITY, z);
	return location;
}

/**
 * to_absolute_coord
 *
 * Converts region local coordinates to absolute coordinates
 *  - local: The region local coordinates
 * returns: The absolute coordinates
 */
location_t to_absolute_coord(region_local_t local)
{
	int x = local.x + (AREA_GRANULARITY * local.region.base.x);
	int y = local.y + (AREA_GRANULARITY * local.region.base.y);
	return absolute_coord(x, y, local.z);
}

sector_t sector(int x, int y, int z)
{
	sector_t sector = { .x = x, .y = y, .z = z };
	return sector;
}

/**
 * center_region_on
 *
 * Calculates the region with it's center at a given location
 *  - location: The absolute coords of the center
 * returns: The region
 */
region_t center_region_on(location_t location)
{
	region_t region;
	region.base.x = location.sector.x - ((REGION_GRANULARITY - 1) / 2);
	region.base.y = location.sector.y - ((REGION_GRANULARITY - 1) / 2);
	region.base.z = location.sector.z;
	return region;
}

/**
 * local_coord
 *
 * Calculates local coords of a location, relative to a region
 *  - location: The absolute coords
 *  - region: The region
 * returns: The region local coords
 */
region_local_t local_coord(location_t location, region_t region)
{
	region_local_t local;
	local.x = location.x - (AREA_GRANULARITY * region.base.x);
	local.y = location.y - (AREA_GRANULARITY * region.base.y);
	local.z = location.z;
	local.region = region;
	return local;
}
