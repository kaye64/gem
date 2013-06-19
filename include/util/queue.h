#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>

#include <util/container_of.h>
#include <util/list.h>

struct queue {
	list_t list;
	bool must_free;
};
typedef struct queue queue_t;

queue_t* queue_create(queue_t* queue);
void queue_free(queue_t* queue);

void queue_push(queue_t* queue, list_node_t* item);
list_node_t* queue_pop(queue_t* queue);
bool queue_empty(queue_t* queue);

#endif /* _QUEUE_H_ */
