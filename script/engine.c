/**
 * engine.c
 *
 * Sets up and controls the scripting interface
 */

#include <Python.h>

#include <script/engine.h>

#include <script/api/gem.h>
#include <script/api/log.h>
#include <util/log.h>

#define LOG_TAG "script"

/**
 * Initializes the scripting engine with the given script root directory
 */
bool script_init()
{
	/* setup the api modules */
	PyImport_AppendInittab("gem", &gem_init_module);

	/* init python */
	Py_Initialize();

	/* import the startup module */
	PySys_SetPath(L"./content/");
	PyObject* startup_module = PyImport_ImportModule("startup");
	if (startup_module == NULL) {
		ERROR("Unable to import startup module");
		PyErr_Print();
		return false;
	}

	/* run startup() */
	PyObject* startup_func = PyObject_GetAttrString(startup_module, "startup");
	if (!startup_func || !PyCallable_Check(startup_func)) {
		ERROR("Unable to find/call startup function");
		PyErr_Print();
		return false;
	}
	PyObject_CallObject(startup_func, NULL);
	Py_DECREF(startup_func);

	return true;
}

/**
 * Cleans up the scripting engine
 */
void script_free()
{
	Py_Finalize();
}
