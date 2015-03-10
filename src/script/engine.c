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
#include <util/log.h>

#define LOG_TAG "script"

static PyObject* core_module;

PyObject* PyInit__Location(void);
PyObject* PyInit__Log(void);
PyObject* PyInit__Entity(void);
PyObject* PyInit__Hook(void);

/**
 * Initializes the scripting engine with the given script root directory
 */
bool script_init(const char* content_dir)
{
	/* setup the api modules */
	PyImport_AppendInittab("_Location", &PyInit__Location);
	PyImport_AppendInittab("_Log", &PyInit__Log);
	PyImport_AppendInittab("_Entity", &PyInit__Entity);
	PyImport_AppendInittab("_Hook", &PyInit__Hook);

	/* init python */
	Py_Initialize();

	/* import the core module */
	/* add content dir to path */
	PyRun_SimpleString("import sys\n");
	char insert[64];
	sprintf(insert, "sys.path.insert(0, \"%s\")\n", content_dir);
	PyRun_SimpleString(&insert);

	core_module = PyImport_ImportModule("core");
	if (core_module == NULL) {
		ERROR("Unable to import core module");
		goto error;
	}

	/* init our subsystems */
	hook_init();

	/* run content_init() */
	PyObject* init_func = PyObject_GetAttrString(core_module, "content_init");
	if (!init_func || !PyCallable_Check(init_func)) {
		ERROR("Unable to find core.content_init function");
		goto error;
	}
	PyObject_CallObject(init_func, NULL);

	goto exit;
error:
	PyErr_Print();
	return false;

exit:
	Py_XDECREF(init_func);
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
