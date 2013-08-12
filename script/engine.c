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

	/* import the core module */
	wchar_t content_dir_wide[100];
	swprintf(content_dir_wide, 100, L"%hs", content_dir);
	PySys_SetPath(content_dir_wide);
	core_module = PyImport_ImportModule("core");
	if (core_module == NULL) {
		ERROR("Unable to import core module");
		goto error;
	}

	/* run startup() */
	PyObject* startup_func = PyObject_GetAttrString(core_module, "startup");
	if (!startup_func || !PyCallable_Check(startup_func)) {
		ERROR("Unable to find/call startup function");
		goto error;
	}
	PyObject_CallObject(startup_func, NULL);

	goto exit;
error:
	PyErr_Print();
	return false;

exit:
	Py_XDECREF(startup_func);
	return true;
}

/**
 * Cleans up the scripting engine
 */
void script_free()
{
	/* run shutdown() */
	PyObject* shutdown_func = PyObject_GetAttrString(core_module, "shutdown");
	if (!shutdown_func || !PyCallable_Check(shutdown_func)) {
		ERROR("Unable to find/call shutdown function");
		PyErr_Print();
		goto exit;
	}
	PyObject_CallObject(shutdown_func, NULL);
	Py_DECREF(shutdown_func);

exit:
	Py_Finalize();
}
