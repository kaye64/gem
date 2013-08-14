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

/**
 * Defines the gem.register_hook api function
 */
static PyObject* api_register_hook(PyObject* self, PyObject* args)
{
	int hook;
	PyObject* callback;
	if (!PyArg_ParseTuple(args, "iO", &hook, &callback)) {
		return NULL;
	}

	hook_register(hook, callback);
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef gem_methods[] = {
	{"register_hook", api_register_hook, METH_VARARGS, "Register an api hook"},
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
	api_entity_init_type(module);
	api_mob_init_type(module);
	api_player_init_type(module);
	return module;
}
