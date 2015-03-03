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

#ifndef _SCRIPT_HOOK_H_
#define _SCRIPT_HOOK_H_

#include <Python.h>

#define SCRIPT_HOOK_STARTUP 0
#define SCRIPT_HOOK_SHUTDOWN 1
#define SCRIPT_HOOK_PLAYER_LOGIN 2
#define SCRIPT_HOOK_PLAYER_LOGOUT 3
#define SCRIPT_HOOK_BUTTON_CLICK 4
#define SCRIPT_HOOK_PLAYER_POSITION 5
#define SCRIPT_HOOK_PLAYER_AUTHENTICATE 6
#define SCRIPT_HOOK_PLAYER_LOAD 7

void hook_init();
void hook_free();

void hook_create_constants(PyObject* module);
void hook_notify(int hook, void* args);
void* hook_call(int hook, void* args);

#endif /* _SCRIPT_HOOK_H_ */
