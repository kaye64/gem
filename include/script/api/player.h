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

#ifndef _API_PLAYER_H_
#define _API_PLAYER_H_

#include <Python.h>
#include <stddef.h>

#include <script/reflect.h>
#include <script/api/mob.h>

typedef struct player player_t;
typedef struct api_player api_player_t;
typedef struct button_click_args button_click_args_t;
typedef struct player_position_args player_position_args_t;

struct api_player {
	api_mob_t mob;
	player_t* player;
};

struct button_click_args {
	player_t* player;
	int button;
};

struct player_position_args {
	player_t* player;
	int warped;
};

PyObject* build_player_auth_args(void* args);
PyObject* build_player_load_args(void* args);
PyObject* build_player_login_args(void* args);
PyObject* build_player_logout_args(void* args);
PyObject* build_button_click_args(void* args);
PyObject* build_player_position_args(void* _args);

void api_player_init_type(PyObject* module);
PyObject* api_player_create(player_t* player);
void api_player_init(api_player_t* api_player, player_t* player);

#endif /* _API_PLAYER_H_ */
