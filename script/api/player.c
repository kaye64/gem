/**
 * player.c
 *
 * Defines the gem.Player API type
 */
#include <script/api/player.h>

#include <script/api/mob.h>
#include <game/player.h>

/**
 * Builds the argument tuple for the player login callback
 */
PyObject* build_player_login_args(void* args)
{
	player_t* player = (player_t*)args;
	PyObject* player_object = api_player_create(player);
	return Py_BuildValue("(O)", player_object);
}

static PyMethodDef player_methods[] = {
    {NULL, NULL, 0, NULL}
};

static PyTypeObject player_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"gem.Player",                 /* tp_name */
    sizeof(api_player_t),         /* tp_basicsize */
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
    "Player object",              /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
	0,                         /* tp_iternext */
	player_methods,            /* tp_methods */
    0,                         /* tp_members */
};

/**
 * Initializes the gem.Player type
 */
void api_player_init_type(PyObject* module)
{
	player_type.tp_new = PyType_GenericNew;
	player_type.tp_base = &mob_type;
	if (PyType_Ready(&player_type) < 0) {
		return;
	}
	Py_INCREF(&player_type);
	PyModule_AddObject(module, "Player", (PyObject*)&player_type);
}

/**
 * Allocates and initializes a new gem.Player
 */
PyObject* api_player_create(player_t* player)
{
	api_player_t* api_player;
	api_player = (api_player_t*)player_type.tp_alloc(&player_type, 0);
	api_player_init(api_player, player);
	return (PyObject*)api_player;
}

/**
 * Initializes a new gem.Player
 */
void api_player_init(api_player_t* api_player, player_t* player)
{
	api_player->player = player;
	api_mob_init(&api_player->mob, mob_for_player(player));
}
