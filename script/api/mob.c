/**
 * mob.c
 *
 * Defines the gem.Mob API type
 */
#include <script/api/mob.h>

#include <script/api/entity.h>
#include <game/mob.h>

static PyMethodDef mob_methods[] = {
    {NULL, NULL, 0, NULL}
};

PyTypeObject mob_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "gem.Mob",                 /* tp_name */
    sizeof(api_mob_t),         /* tp_basicsize */
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
    "Mob object",              /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    mob_methods,               /* tp_methods */
    0,                         /* tp_members */
};

/**
 * Initializes the gem.mob type
 */
void api_mob_init_type(PyObject* module)
{
	mob_type.tp_new = PyType_GenericNew;
	mob_type.tp_base = &entity_type;
	if (PyType_Ready(&mob_type) < 0) {
        return;
	}
	Py_INCREF(&mob_type);
	PyModule_AddObject(module, "Mob", (PyObject*)&mob_type);
}

/**
 * Allocates and initializes a new gem.Mob
 */
PyObject* api_mob_create(mob_t* mob)
{
	api_mob_t* api_mob;
	api_mob = (api_mob_t*)mob_type.tp_alloc(&mob_type, 0);
	api_mob_init(api_mob, mob);
	return (PyObject*)api_mob;
}

/**
 * Initializes a new gem.Mob
 */
void api_mob_init(api_mob_t* api_mob, mob_t* mob)
{
	api_mob->mob = mob;	
	api_entity_init(&api_mob->entity, entity_for_mob(mob));
}
