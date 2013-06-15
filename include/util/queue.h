#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>

#include <util/container_of.h>

struct queue_item;

struct queue {
	struct queue_item* top;
	struct queue_item* bottom;
	bool must_free;
};
typedef struct queue queue_t;

struct queue_item {
	struct queue_item* prev;
	struct queue_item* next;
};
typedef struct queue_item queue_item_t;

queue_t* queue_create(queue_t* queue);
void queue_free(queue_t* queue);

void queue_push(queue_t* queue, queue_item_t* item);
queue_item_t* queue_pop(queue_t* queue);
bool queue_empty(queue_t* queue);

#endif /* _QUEUE_H_ */
