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

#include <script/hook.h>

#include <script/api/player.h>
#include <util/list.h>
#include <util/log.h>
#include <util/container_of.h>

#define LOG_TAG "hook"

typedef struct hook_entry hook_entry_t;
struct hook_entry {
	list_node_t node;
	int hook;
	PyObject* callback;
};

typedef struct hook hook_t;
struct hook {
	int hook;
	char const_name[32];
	PyObject*(*build_func)(void*);
};

/* The list of hook callbacks. Nodes are hook_entry_t */
static list_t hook_lookup;
/* The lookup table of all event hooks */
static hook_t dispatch_lookup[] = {
	{ .hook = SCRIPT_HOOK_STARTUP, .const_name = "HOOK_STARTUP", .build_func = NULL },
	{ .hook = SCRIPT_HOOK_SHUTDOWN, .const_name = "HOOK_SHUTDOWN", .build_func = NULL },
	{ .hook = SCRIPT_HOOK_PLAYER_LOGIN, .const_name = "HOOK_PLAYER_LOGIN", .build_func = build_player_login_args },
	{ .hook = SCRIPT_HOOK_PLAYER_LOGOUT, .const_name = "HOOK_PLAYER_LOGOUT", .build_func = build_player_logout_args },
	{ .hook = SCRIPT_HOOK_BUTTON_CLICK, .const_name = "HOOK_BUTTON_CLICK", .build_func = build_button_click_args },
	{ .hook = -1, .const_name = "", .build_func = NULL }
};

/**
 * Initialize the hook system
 */
void hook_init()
{
	object_init(list, &hook_lookup);
}

/**
 * Clean up after the hook system
 */
void hook_free()
{
	/* free up our hook table */
	hook_entry_t* entry = NULL;
	list_for_each(&hook_lookup) {
		list_for_get(entry);
		Py_DECREF(entry->callback);
		free(entry);
	}
	object_free(&hook_lookup);
}

/**
 * Create all of the gem.HOOK_* constants
 */
void hook_create_constants(PyObject* module)
{
	int i = 0;
	hook_t* dispatch = &dispatch_lookup[i];
	while (dispatch->hook != -1) {
		PyModule_AddIntConstant(module, dispatch->const_name, dispatch->hook);
		dispatch = &dispatch_lookup[++i];
	}
}

/**
 * Register a new API hook callback
 */
void hook_register(int hook, PyObject* callback)
{
	if (!callback || !PyCallable_Check(callback)) {
		ERROR("Unable to register hook");
		PyErr_Print();
		return;
	}
	hook_entry_t* hook_entry = (hook_entry_t*)malloc(sizeof(hook_entry_t));
	hook_entry->hook = hook;
	hook_entry->callback = callback;
	Py_INCREF(hook_entry->callback);
	list_push_back(&hook_lookup, &hook_entry->node);
}

/**
 * Dispatches a hook to each of the registered callers
 */
void hook_notify(int hook, void* args)
{
	/* find the build func and get the arguments */
	int i = 0;
	PyObject* py_args = NULL;
	hook_t* dispatch = &dispatch_lookup[i];
	while (dispatch->hook != -1) {
		if (dispatch->hook == hook) {
			if (dispatch->build_func == NULL) {
				/* no arguments */
				break;
			}
			py_args = dispatch->build_func(args);
			break;
		}
		dispatch = &dispatch_lookup[++i];
	}
	
	/* call each registered hook */
	hook_entry_t* entry = NULL;
	list_for_each(&hook_lookup) {
		list_for_get(entry);
		if (entry->hook == hook) {
			if (!PyObject_CallObject(entry->callback, py_args)) {
				ERROR("hook call failed");
				PyErr_Print();
			}
		}
	}
}
