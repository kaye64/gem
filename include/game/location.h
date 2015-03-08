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

#ifndef _LOCATION_H_
#define _LOCATION_H_

#include <Python.h>

#define AREA_GRANULARITY 8
#define REGION_GRANULARITY 13

/**
 * A sector is an 8x8 tile plot
 */
struct sector {
	int x;
	int y;
	int z;
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
#ifdef SWIG
%rename(Absolute) location;
#endif /* SWIG */
struct location {
	int x;
	int y;
	int z;
	sector_t sector;
#ifdef SWIG
	%extend {
		location(int x, int y, int z) {
			location_t loc = absolute_coord(x, y, z);
			location_t* v;
			v = (location_t *) malloc(sizeof(location_t));
			memcpy(v, &loc, sizeof(location_t));
			return v;
		}
		~location() {
			free($self);
		}
	};
#endif /* SWIG */
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
sector_t sector(int x, int y, int z);
region_t center_region_on(location_t loc);
region_local_t local_coord(location_t loc, region_t region);

PyObject* location_wrap_from_location(location_t loc);
location_t location_from_location_wrap(PyObject* loc);

#endif /* _LOCATION_H_ */
