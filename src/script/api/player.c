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
 * player.c
 *
 * Defines the gem.Player API type
 */
#include <script/api/player.h>

#include <structmember.h>

#include <script/api/mob.h>
#include <game/packet/builders.h>
#include <game/player.h>

static PyObject* api_player_getattro(api_player_t* self, PyObject* name);
static int api_player_setattro(api_player_t* self, PyObject* attr_name, PyObject* value);
static PyObject* api_player_get_location(player_t* player, void* offset);
static int api_player_set_location(player_t* player, PyObject* args, void* offset);

ref_table_entry_t api_player_ref_table[] = {
	reflect_pyint_field("uid", player_t, client_uid),
	reflect_pyint_field("rights", player_t, rights),
	reflect_pyint_field("index", player_t, index),
	reflect_pystring_field("username", player_t, username),
	reflect_pystring_field("password", player_t, password),
	reflect_field("location", &api_player_get_location, &api_player_set_location, NULL),
	reflect_terminator,
};


/**
 * Retrieves the profile object stored in player->attachment
 */
static PyObject* api_player_get_profile(PyObject* self, PyObject* args)
{
	player_t* player = ((api_player_t*)self)->player;
	PyObject* profile = (PyObject*)player->attachment;
	Py_INCREF(profile);
	return profile;
}

/**
 * Flushes the stored player identity to the client
 * Sets the identity update flag on the mob
 */
static PyObject* api_player_flush_identity(PyObject* self, PyObject* args)
{
	player_t* player = ((api_player_t*)self)->player;
	player->mob.update_flags |= MOB_FLAG_IDENTITY_UPDATE;

	Py_INCREF(Py_None);
	return Py_None;
}

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

	player->state.tab_interfaces[tab_id] = interface_id;
	player->state.update_flags |= STATE_TAB_UPDATE;

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
 * Returns the player's location
 */
static PyObject* api_player_get_location(player_t* player, void* offset)
{
	location_t location = mob_position(mob_for_player(player));
	PyObject* py_location = location_wrap_from_location(location);
	Py_INCREF(py_location);
	return py_location;
}

/**
 * Warps a player to a specified location
 * player.location = gem.Location(3200, 3200, 0)
 */
static int api_player_set_location(player_t* player, PyObject* args, void* offset)
{
	location_t location = location_from_location_wrap(args);
	player_warp_to(player, location);
	return 0;
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
 * Builds the argument tuple for the player authentication callback
 */
PyObject* build_player_auth_args(void* args)
{
	player_t* player = (player_t*)args;
	PyObject* player_object = api_player_create(player);
	return Py_BuildValue("(O)", player_object);
}

/**
 * Builds the argument tuple for the player load callback
 */
PyObject* build_player_load_args(void* args)
{
	player_t* player = (player_t*)args;
	PyObject* player_object = api_player_create(player);
	return Py_BuildValue("(O)", player_object);
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
	PyObject* player_object = api_player_create(args->player);
	return Py_BuildValue("(Oi)", player_object, args->button);
}

/**
 * Builds the argument tuple for the player position update callback
 */
PyObject* build_player_position_args(void* _args)
{
	player_position_args_t* args = (player_position_args_t*)_args;
	player_t* player = args->player;
	PyObject* player_object = api_player_create(player);
	location_t location = mob_position(mob_for_player(player));
	PyObject* location_object = location_wrap_from_location(location);
	return Py_BuildValue("(OOO)", player_object, location_object, (args->warped ? Py_True : Py_False));
}

static PyMethodDef player_methods[] = {
	{"get_profile", api_player_get_profile, METH_VARARGS, "Retrieves the associated profile object"},
	{"set_tab_interface", api_player_set_tab_interface, METH_VARARGS, "Update a player's tab interface"},
	{"send_message", api_player_send_message, METH_VARARGS, "Send a game message to a player"},
	{"logout", api_player_logout, METH_VARARGS, "Logs the player out"},
	{"set_running", api_player_set_running, METH_VARARGS, "Sets the player's run status"},
	{"flush_identity", api_player_flush_identity, METH_VARARGS, "Flushes the player's identity to the client"},
    {NULL, NULL, 0, NULL}
};

static PyMemberDef player_members[] = {
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
    api_player_getattro,       /* tp_getattro */
    api_player_setattro,       /* tp_setattro */
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
}

/**
 * tp_getattro function for player object
 */
PyObject* api_player_getattro(api_player_t* self, PyObject* attr_name)
{
	char* identifier = PyUnicode_AsUTF8(attr_name);
	PyObject* result = reflect_get(self->player, &api_player_ref_table[0], identifier);
	if (result != NULL) {
		return result;
	}

	result = PyObject_GenericGetAttr((PyObject*) self, attr_name);
	if (result != NULL) {
		return result;
	}

	return NULL;
}

/**
 * tp_setattro function for player object
 */
int api_player_setattro(api_player_t* self, PyObject* attr_name, PyObject* value)
{
	char* identifier = PyUnicode_AsUTF8(attr_name);
	int result = reflect_set(self->player, &api_player_ref_table[0], identifier, value);
	if (result >= 0) {
		return result;
	}

	return PyObject_GenericSetAttr((PyObject*) self, attr_name, value);
}
