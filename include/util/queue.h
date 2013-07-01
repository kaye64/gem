#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>

#include <util/object.h>
#include <util/container_of.h>
#include <util/list.h>

typedef struct queue queue_t;

struct queue {
	object_t object;
	list_t list;
};

extern object_proto_t queue_proto;

void queue_push(queue_t* queue, list_node_t* item);
list_node_t* queue_pop(queue_t* queue);
list_node_t* queue_peek(queue_t* queue);
bool queue_empty(queue_t* queue);
void queue_clear(queue_t* queue);

#endif /* _QUEUE_H_ */
