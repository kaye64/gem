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
 * location.c
 *
 * Defines the various coordinate systems
 */
#include <game/location.h>

/**
 * Constructs a location_t from absolute x, y, and z
 */
location_t absolute_coord(int x, int y, int z)
{
	location_t location = { .x = x, .y = y, .z = z };
	location.sector = sector(location.x / AREA_GRANULARITY, location.y / AREA_GRANULARITY, z);
	return location;
}

/**
 * Converts region local coordinates to absolute coordinates
 */
location_t to_absolute_coord(region_local_t local)
{
	int x = local.x + (AREA_GRANULARITY * local.region.base.x);
	int y = local.y + (AREA_GRANULARITY * local.region.base.y);
	return absolute_coord(x, y, local.z);
}

/**
 * Constructs a sector_t from sector x, y and z
 */
sector_t sector(int x, int y, int z)
{
	sector_t sector = { .x = x, .y = y, .z = z };
	return sector;
}

/**
 * Calculates the region with it's center at a given location
 *  - location: The absolute coords of the center
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
 * Calculates local coords of a location, relative to a region
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
