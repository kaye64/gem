/**
 * gem.c
 *
 * Defines the gem api root module
 */

#include <script/api/gem.h>
#include <script/api/log.h>
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
	PyObject* log_module = log_init_module();
	PyModule_AddObject(module, "log", log_module);
	return module;
}
