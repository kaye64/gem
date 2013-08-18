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
 * engine.c
 *
 * Sets up and controls the scripting interface
 */

#include <Python.h>

#include <script/engine.h>

#include <script/hook.h>
#include <script/api/gem.h>
#include <script/api/log.h>
#include <util/log.h>

#define LOG_TAG "script"

static PyObject* core_module;

/**
 * Initializes the scripting engine with the given script root directory
 */
bool script_init(const char* content_dir)
{
	/* setup the api modules */
	PyImport_AppendInittab("gem", &gem_init_module);

	/* init python */
	Py_Initialize();

	/* init our subsystems */
	hook_init();

	/* import the core module */
	wchar_t content_dir_wide[100];
	swprintf(content_dir_wide, 100, L"%hs", content_dir);
	PySys_SetPath(content_dir_wide);
	core_module = PyImport_ImportModule("core");
	if (core_module == NULL) {
		ERROR("Unable to import core module");
		goto error;
	}

	/* run register_hooks() */
	PyObject* reg_hook_func = PyObject_GetAttrString(core_module, "register_hooks");
	if (!reg_hook_func || !PyCallable_Check(reg_hook_func)) {
		ERROR("Unable to find/call startup function");
		goto error;
	}
	PyObject_CallObject(reg_hook_func, NULL);

	goto exit;
error:
	PyErr_Print();
	return false;

exit:
	Py_XDECREF(reg_hook_func);
	return true;
}

/**
 * Cleans up the scripting engine
 */
void script_free()
{
	hook_free();
	Py_Finalize();
}
