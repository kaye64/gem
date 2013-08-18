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
 * log.c
 *
 * Defines the gem api log module
 */

#include <script/api/log.h>
#include <util/log.h>

#define LOG_TAG "api"
#define SCRIPT_LOG_TAG "script"

/**
 * Log an error message
 */
static PyObject* script_log_error(PyObject *self, PyObject *args)
{
	char* script_tag;
	char* message;
	if (!PyArg_ParseTuple(args, "ss", &script_tag, &message)) {
		return NULL;
	}
	char tag[32];
	sprintf(tag, "%s/%s", SCRIPT_LOG_TAG, script_tag);

	log_error(tag, message);

	Py_INCREF(Py_None);
	return Py_None;
}


/**
 * Log a warn message
 */
static PyObject* script_log_warn(PyObject *self, PyObject *args)
{
	char* script_tag;
	char* message;
	if (!PyArg_ParseTuple(args, "ss", &script_tag, &message)) {
		return NULL;
	}
	char tag[32];
	sprintf(tag, "%s/%s", SCRIPT_LOG_TAG, script_tag);

	log_warn(tag, message);

	Py_INCREF(Py_None);
	return Py_None;
}


/**
 * Log an info message
 */
static PyObject* script_log_info(PyObject *self, PyObject *args)
{
	char* script_tag;
	char* message;
	if (!PyArg_ParseTuple(args, "ss", &script_tag, &message)) {
		return NULL;
	}
	char tag[32];
	sprintf(tag, "%s/%s", SCRIPT_LOG_TAG, script_tag);
	
	log_info(tag, message);

	Py_INCREF(Py_None);
	return Py_None;
}


/**
 * Log a debug message
 */
static PyObject* script_log_debug(PyObject *self, PyObject *args)
{
	char* script_tag;
	char* message;
	if (!PyArg_ParseTuple(args, "ss", &script_tag, &message)) {
		return NULL;
	}
	char tag[32];
	sprintf(tag, "%s/%s", SCRIPT_LOG_TAG, script_tag);

	log_debug(tag, message);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef log_methods[] = {
    {"error", script_log_error, METH_VARARGS, "Log an error message"},
    {"warn", script_log_warn, METH_VARARGS, "Log a warn message"},
    {"info", script_log_info, METH_VARARGS, "Log an info message"},
    {"debug", script_log_debug, METH_VARARGS, "Log a debug message"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef log_moduledef = {
        PyModuleDef_HEAD_INIT,
        "log",
        NULL,
        -1,
        log_methods,
        NULL,
        NULL,
        NULL,
        NULL
};

/**
 * Initializes the gem.log module
 */
PyObject* log_init_module()
{
	PyObject* module_name = PyUnicode_FromString("log");
	PyObject* module = PyModule_Create(&log_moduledef);
	PyObject_SetAttrString(module, "__path__", module_name);
	return module;
}
