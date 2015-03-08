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
 * entity.c
 *
 * Defines the gem.Entity API type
 */
#include <script/api/entity.h>

#include <structmember.h>

#include <game/entity.h>

static PyMethodDef entity_methods[] = {
    {NULL, NULL, 0, NULL}
};

static PyMemberDef entity_members[] = {
    {"index", T_INT, offsetof(api_entity_t, index), READONLY, "entity index"},
	{"location", T_OBJECT_EX, offsetof(api_entity_t, location), 0, "player location"},
    {NULL, 0, 0, 0, NULL}
};

PyTypeObject entity_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "gem.Entity",              /* tp_name */
    sizeof(api_entity_t),      /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
	Py_TPFLAGS_BASETYPE,       /* tp_flags */
    "Entity object",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    entity_methods,            /* tp_methods */
    entity_members,            /* tp_members */
};

/**
 * Initializes the gem.Entity type
 */
void api_entity_init_type(PyObject* module)
{
	entity_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&entity_type) < 0) {
        return;
	}
	Py_INCREF(&entity_type);
	PyModule_AddObject(module, "Entity", (PyObject*)&entity_type);
}

/**
 * Allocates and initializes a new gem.Entity
 */
PyObject* api_entity_create(entity_t* entity)
{
	api_entity_t* api_entity;
	api_entity = (api_entity_t*)entity_type.tp_alloc(&entity_type, 0);
	api_entity_init(api_entity, entity);
	return (PyObject*)api_entity;
}

/**
 * Initializes a new gem.Entity
 */
void api_entity_init(api_entity_t* api_entity, entity_t* entity)
{
	api_entity->entity = entity;
	api_entity->index = entity->index;
	api_entity->location = location_wrap_from_location(entity->position);
}
