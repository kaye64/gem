/**
 * object.c
 *
 * Defines our object system
 */
#include <util/object.h>

/**
 * Allocates and initializes an object
 */
object_t* _object_new(object_proto_t proto, size_t size)
{
	object_t* object = (object_t*)malloc(size);
	_object_init(proto, object);
	object->must_free = true;
	return object;
}

/**
 * Initializes an object
 */
void _object_init(object_proto_t proto, object_t* object)
{
	proto.init(object);
	object->prototype.init = proto.init;
	object->prototype.free = proto.free;
	object->must_free = false;
}

/**
 * Frees an object
 */
void _object_free(object_t* object)
{
	object->prototype.free(object);
	if (object->must_free) {
		free(object);
	}
}
