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

#ifndef _API_ENTITY_H_
#define _API_ENTITY_H_

#include <Python.h>

#include <script/api/location.h>

typedef struct entity entity_t;
typedef struct api_entity api_entity_t;
struct api_entity {
	PyObject_HEAD
	entity_t* entity;
	int index;
	PyObject* location;
};

extern PyTypeObject entity_type;

void api_entity_init_type(PyObject* module);
PyObject* api_entity_create(entity_t* entity);
void api_entity_init(api_entity_t* api_entity, entity_t* entity);

#endif /* _API_ENTITY_H_ */
