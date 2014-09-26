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
 * gem.c
 *
 * Defines the gem api root module
 */

#include <script/hook.h>
#include <script/api/gem.h>
#include <script/api/log.h>
#include <script/api/entity.h>
#include <script/api/mob.h>
#include <script/api/player.h>
#include <util/log.h>

#define LOG_TAG "api"

static PyMethodDef gem_methods[] = {
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef gem_moduledef = {
        PyModuleDef_HEAD_INIT,
        "gem",
        NULL,
        -1,
        gem_methods,
        NULL,
        NULL,
        NULL,
        NULL
};

/**
 * Initializes the gem module
 */
PyObject* gem_init_module()
{
	PyObject* module_name = PyUnicode_FromString("gem");
	PyObject* module = PyModule_Create(&gem_moduledef);
	PyObject_SetAttrString(module, "__path__", module_name);
	hook_create_constants(module);

	/* create the log module */
	PyObject* log_module = log_init_module();
	PyModule_AddObject(module, "log", log_module);

	/* create the entity, mob, and player types */
	api_location_init_type(module);
	api_entity_init_type(module);
	api_mob_init_type(module);
	api_player_init_type(module);
	return module;
}
