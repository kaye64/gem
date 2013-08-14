#ifndef _API_ENTITY_H_
#define _API_ENTITY_H_

#include <Python.h>

typedef struct entity entity_t;
typedef struct api_entity api_entity_t;
struct api_entity {
	PyObject_HEAD
	entity_t* entity;
	int index;
};

extern PyTypeObject entity_type;

void api_entity_init_type(PyObject* module);
PyObject* api_entity_create(entity_t* entity);
void api_entity_init(api_entity_t* api_entity, entity_t* entity);

#endif /* _API_ENTITY_H_ */
