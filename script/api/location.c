/**
 * location.c
 *
 * Defines the gem.Location API type
 */
#include <script/api/location.h>

#include <structmember.h>

#include <game/location.h>

static PyObject* api_location_py_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
	api_location_t* self;
	self = (api_location_t*)type->tp_alloc(type, 0);
	if (self == NULL) {
		return NULL;
	}
	self->absolute_x = 0;
	self->absolute_y = 0;
	self->absolute_z = 0;
	self->sector_x = 0;
	self->sector_y = 0;
	self->sector_z = 0;
	return (PyObject*)self;
}

static int api_location_py_init(api_location_t* self, PyObject* args, PyObject* kwds)
{
	static char* kwlist[] = {"x", "y", "z", NULL};
	int x, y, z;
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "iii", kwlist, &x, &y, &z)) {
		return -1;
	}
	location_t location = absolute_coord(x, y, z);
	api_location_init(self, location);
	return 0;
}

static PyMethodDef location_methods[] = {
    {NULL, NULL, 0, NULL}
};

static PyMemberDef location_members[] = {
    {"abs_x", T_INT, offsetof(api_location_t, absolute_x), 0, "absolute x position"},
    {"abs_y", T_INT, offsetof(api_location_t, absolute_y), 0, "absolute y position"},
    {"abs_z", T_INT, offsetof(api_location_t, absolute_z), 0, "absolute z position"},
    {"sector_x", T_INT, offsetof(api_location_t, sector_x), 0, "sector x position"},
    {"sector_y", T_INT, offsetof(api_location_t, sector_y), 0, "sector y position"},
    {"sector_z", T_INT, offsetof(api_location_t, sector_z), 0, "sector z position"},
    {NULL, 0, 0, 0, NULL}
};

PyTypeObject location_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "gem.Location",            /* tp_name */
    sizeof(api_location_t),    /* tp_basicsize */
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
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "Location object",         /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    location_methods,          /* tp_methods */
    location_members,          /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)api_location_py_init,      /* tp_init */
    0,                         /* tp_alloc */
    api_location_py_new,       /* tp_new */
};

/**
 * Initializes the gem.Location type
 */
void api_location_init_type(PyObject* module)
{
	if (PyType_Ready(&location_type) < 0) {
        return;
	}
	Py_INCREF(&location_type);
	PyModule_AddObject(module, "Location", (PyObject*)&location_type);
}

/**
 * Allocates and initializes a new gem.Location
 */
PyObject* api_location_create(location_t location)
{
	api_location_t* api_location;
	api_location = (api_location_t*)location_type.tp_alloc(&location_type, 0);
	api_location_init(api_location, location);
	return (PyObject*)api_location;
}

/**
 * Initializes a new gem.Location
 */
void api_location_init(api_location_t* api_location, location_t location)
{
	api_location->location = location;
	api_location->absolute_x = location.x;
	api_location->absolute_y = location.y;
	api_location->absolute_z = location.z;
	api_location->sector_x = location.sector.x;
	api_location->sector_y = location.sector.y;
	api_location->sector_z = location.sector.z;
}
