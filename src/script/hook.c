#include <script/hook.h>

#include <runite/util/list.h>
#include <runite/util/container_of.h>

#include <script/api/player.h>
#include <util/log.h>

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

/* The lookup table of all event hooks */
static hook_t dispatch_lookup[] = {
	{ .hook = SCRIPT_HOOK_STARTUP, .const_name = "HOOK_STARTUP", .build_func = NULL },
	{ .hook = SCRIPT_HOOK_SHUTDOWN, .const_name = "HOOK_SHUTDOWN", .build_func = NULL },
	{ .hook = SCRIPT_HOOK_PLAYER_AUTHENTICATE, .const_name = "HOOK_PLAYER_AUTHENTICATE", .build_func = build_player_auth_args },
	{ .hook = SCRIPT_HOOK_PLAYER_LOGIN, .const_name = "HOOK_PLAYER_LOGIN", .build_func = build_player_login_args },
	{ .hook = SCRIPT_HOOK_PLAYER_LOGOUT, .const_name = "HOOK_PLAYER_LOGOUT", .build_func = build_player_logout_args },
	{ .hook = SCRIPT_HOOK_BUTTON_CLICK, .const_name = "HOOK_BUTTON_CLICK", .build_func = build_button_click_args },
	{ .hook = SCRIPT_HOOK_PLAYER_POSITION, .const_name = "HOOK_PLAYER_POSITION", .build_func = build_player_position_args },
	{ .hook = SCRIPT_HOOK_PLAYER_LOAD, .const_name = "HOOK_PLAYER_LOAD", .build_func = build_player_load_args },
	{ .hook = -1, .const_name = "", .build_func = NULL }
};

static PyObject* hook_module;
static PyObject* hook_dispatch_func;
static PyObject* hook_dispatch_exc_func;

#define py_lookup_func(dest, object, func) {				\
		dest = PyObject_GetAttrString(object, func);		\
		if (!dest || !PyCallable_Check(dest)) {				\
			ERROR("Unable to find " #func " function"); \
		}													\
	}

/**
 * Initialize the hook system
 */
void hook_init()
{
	hook_module = PyImport_ImportModule("hook");
	if (hook_module == NULL) {
		ERROR("Unable to import hook module");
		return;
	}

	py_lookup_func(hook_dispatch_func, hook_module, "dispatch");
	py_lookup_func(hook_dispatch_exc_func, hook_module, "dispatch_exclusive");
}

/**
 * Clean up after the hook system
 */
void hook_free()
{

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
 * Looks up the hook build func and builds the py arguments
 */
static PyObject* hook_build_args(int hook, void* args) {
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
	return py_args;
}

/**
 * Dispatches a hook with multiple recievers
 */
void hook_notify(int hook, void* args)
{
	/* find the build func and get the arguments */
	PyObject* py_args = hook_build_args(hook, args);

	/* dispatch hook to python's hook.dispatch */
	static PyObject* empty_tuple;
	if (!empty_tuple) {
		empty_tuple = Py_BuildValue("()");
	}

	PyObject* hook_params = Py_BuildValue("(iO)", hook, (py_args ? py_args : empty_tuple));
	if (!PyObject_CallObject(hook_dispatch_func, hook_params)) {
		ERROR("hook notify failed: %i", hook);
		PyErr_Print();
	}
}

/**
 * Dispatches a hook with a single reciever and returns its value
 */
void* hook_call(int hook, void* args)
{
	/* find the build func and get the arguments */
	PyObject* py_args = hook_build_args(hook, args);

	/* dispatch hook to python's hook.dispatch_exclusive */
	static PyObject* empty_tuple;
	if (!empty_tuple) {
		empty_tuple = Py_BuildValue("()");
	}

	PyObject* retval;
	PyObject* hook_params = Py_BuildValue("(iO)", hook, (py_args ? py_args : empty_tuple));
	if (!(retval = PyObject_CallObject(hook_dispatch_exc_func, hook_params))) {
		ERROR("hook call failed: %i", hook);
		PyErr_Print();
	}
	return (void*)retval;
}
