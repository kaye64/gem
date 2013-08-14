#ifndef _SCRIPT_HOOK_H_
#define _SCRIPT_HOOK_H_

#include <Python.h>

#define SCRIPT_HOOK_PLAYER_LOGIN 0

void hook_init();
void hook_free();

void hook_create_constants(PyObject* module);
void hook_register(int hook, PyObject* callback);
void hook_notify(int hook, void* args);

#endif /* _SCRIPT_HOOK_H_ */
