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

#ifndef _API_MOB_H_
#define _API_MOB_H_

#include <Python.h>

#include <script/api/entity.h>

typedef struct mob mob_t;
typedef struct api_mob api_mob_t;
struct api_mob {
	api_entity_t entity;
	mob_t* mob;
};

extern PyTypeObject mob_type;

void api_mob_init_type(PyObject* module);
PyObject* api_mob_create(mob_t* mob);
void api_mob_init(api_mob_t* api_mob, mob_t* mob);

#endif /* _API_MOB_H_ */
