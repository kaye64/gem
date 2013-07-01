#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct object_proto object_proto_t;
typedef struct object object_t;

typedef void (*object_init_t)(void*);
typedef void (*object_free_t)(void*);

struct object_proto {
	object_init_t init;
	object_free_t free;
};

struct object {
	object_proto_t prototype;
	bool must_free;
};

object_t* _object_new(object_proto_t proto, size_t size);
void _object_init(object_proto_t proto, object_t* object);
void _object_free(object_t* object);

#define object_new(type) (type##_t*)_object_new(type##_proto, sizeof(type##_t))
#define object_init(type, object) _object_init(type##_proto, (object_t*)object)
#define object_free(object) _object_free((object_t*)object);

#endif /* _OBJECT_H_ */
