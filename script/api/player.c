/**
 * player.c
 *
 * Defines the gem.Player API type
 */
#include <script/api/player.h>

#include <structmember.h>

#include <script/api/mob.h>
#include <game/packet/builders.h>
#include <game/player.h>

/**
 * Sets an interface in one of the player's tabs
 * player.set_tab_interface(tab, interface)
 */
static PyObject* api_player_set_tab_interface(PyObject* self, PyObject* args)
{
	player_t* player = ((api_player_t*)self)->player;
	int tab_id;
	int interface_id;
	if (!PyArg_ParseTuple(args, "ii", &tab_id, &interface_id)) {
		return NULL;
	}

	if (tab_id > 13) {
		return NULL;
	}

	player->tab_interfaces[tab_id] = interface_id;
	player->update_flags |= PLAYER_FLAG_TAB_UPDATE;

	Py_INCREF(Py_None);
	return Py_None;
}

/**
 * Sets a player's run status
 * player.set_running(running)
 */
static PyObject* api_player_set_running(PyObject* self, PyObject* args)
{
	player_t* player = ((api_player_t*)self)->player;	
	mob_t* mob = mob_for_player(player);
	bool running;
	if (!PyArg_ParseTuple(args, "b", &running)) {
		return NULL;
	}

	mob->running = running;

	Py_INCREF(Py_None);
	return Py_None;
}

/**
 * Sends a game message to the player
 * player.send_message(message)
 */
static PyObject* api_player_send_message(PyObject* self, PyObject* args)
{
	player_t* player = ((api_player_t*)self)->player;	
	char* message;
	if (!PyArg_ParseTuple(args, "s", &message)) {
		return NULL;
	}

	player_enqueue_packet(player, packet_build_player_message(message));

	Py_INCREF(Py_None);
	return Py_None;
}

/**
 * Logs the player out
 * player.logout()
 */
static PyObject* api_player_logout(PyObject* self, PyObject* args)
{
	player_t* player = ((api_player_t*)self)->player;

	player_force_logout(player);

	Py_INCREF(Py_None);
	return Py_None;
}

/**
 * Builds the argument tuple for the player login callback
 */
PyObject* build_player_login_args(void* args)
{
	player_t* player = (player_t*)args;
	PyObject* player_object = api_player_create(player);
	return Py_BuildValue("(O)", player_object);
}


/**
 * Builds the argument tuple for the player logout callback
 */
PyObject* build_player_logout_args(void* args)
{
	player_t* player = (player_t*)args;
	PyObject* player_object = api_player_create(player);
	return Py_BuildValue("(O)", player_object);
}

/**
 * Builds the argument tuple for the player button click callback
 */
PyObject* build_button_click_args(void* _args)
{
	button_click_args_t* args = (button_click_args_t*)_args;
	player_t* player = args->player;
	PyObject* player_object = api_player_create(args->player);
	return Py_BuildValue("(Oi)", player_object, args->button);
}

static PyMethodDef player_methods[] = {
	{"set_tab_interface", api_player_set_tab_interface, METH_VARARGS, "Update a player's tab interface"},
	{"send_message", api_player_send_message, METH_VARARGS, "Send a game message to a player"},
	{"logout", api_player_logout, METH_VARARGS, "Logs the player out"},
	{"set_running", api_player_set_running, METH_VARARGS, "Sets the player's run status"},
    {NULL, NULL, 0, NULL}
};

static PyMemberDef player_members[] = {
    {"username", T_OBJECT_EX, offsetof(api_player_t, username), READONLY, "player username"},
    {"rights", T_INT, offsetof(api_player_t, rights), READONLY, "player rights"},
    {NULL, 0, 0, 0, NULL}
};

static PyTypeObject player_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"gem.Player",              /* tp_name */
    sizeof(api_player_t),      /* tp_basicsize */
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
    "Player object",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
	0,                         /* tp_iternext */
	player_methods,            /* tp_methods */
    player_members,            /* tp_members */
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
	api_player->username = PyUnicode_FromString(player->username);
	api_player->rights = player->rights;
}
