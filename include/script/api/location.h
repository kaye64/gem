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

#ifndef _API_LOCATION_H_
#define _API_LOCATION_H_

#include <Python.h>

#include <game/location.h>

typedef struct api_location api_location_t;
struct api_location {
	PyObject_HEAD
	location_t location;
	int absolute_x;
	int absolute_y;
	int absolute_z;
	int sector_x;
	int sector_y;
	int sector_z;
};

extern PyTypeObject location_type;

void api_location_init_type(PyObject* module);
PyObject* api_location_create(location_t location);
void api_location_init(api_location_t* api_location, location_t location);

#endif /* _API_LOCATION_H_ */
