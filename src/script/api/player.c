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

#include <game/packet/builders.h>
#include <game/player.h>

/**
 * Builds the argument tuple for the player authentication callback
 */
PyObject* build_player_auth_args(void* args)
{
	player_t* player = (player_t*)args;
	PyObject* player_object = api_player_in(player);
	return Py_BuildValue("(O)", player_object);
}

/**
 * Builds the argument tuple for the player load callback
 */
PyObject* build_player_load_args(void* args)
{
	player_t* player = (player_t*)args;
	PyObject* player_object = api_player_in(player);
	return Py_BuildValue("(O)", player_object);
}

/**
 * Builds the argument tuple for the player login callback
 */
PyObject* build_player_login_args(void* args)
{
	player_t* player = (player_t*)args;
	PyObject* player_object = (PyObject*)api_player_in(player);
	return Py_BuildValue("(O)", player_object);
}

/**
 * Builds the argument tuple for the player logout callback
 */
PyObject* build_player_logout_args(void* args)
{
	player_t* player = (player_t*)args;
	PyObject* player_object = api_player_in(player);
	return Py_BuildValue("(O)", player_object);
}

/**
 * Builds the argument tuple for the player button click callback
 */
PyObject* build_button_click_args(void* _args)
{
	button_click_args_t* args = (button_click_args_t*)_args;
	PyObject* player_object = api_player_in(args->player);
	return Py_BuildValue("(Oi)", player_object, args->button);
}

/**
 * Builds the argument tuple for the player position update callback
 */
PyObject* build_player_position_args(void* _args)
{
	player_position_args_t* args = (player_position_args_t*)_args;
	player_t* player = args->player;
	PyObject* player_object = api_player_in(player);
	location_t location = mob_position(mob_for_player(player));
	PyObject* location_object = api_location_in(location);
	return Py_BuildValue("(OOO)", player_object, location_object, (args->warped ? Py_True : Py_False));
}
