/**
 * queue.c
 *
 * A FIFO queue implementation
 */
#include <util/queue.h>

#include <stdlib.h>

/**
 * queue_create
 *
 * Initializes a new queue
 *  - queue: Some preallocated memory, or null to put on heap
 * returns: The initialized queue
 */
queue_t* queue_create(queue_t* queue)
{
	if (queue == NULL) {
		queue = (queue_t*)malloc(sizeof(queue_t));
		queue->must_free = true;
	} else {
		queue->must_free = false;
	}
	queue->top = queue->bottom = NULL;
	return queue;
}

/**
 * queue_free
 *
 * Properly frees a queue_t
 *  - queue: The queue
 */
void queue_free(queue_t* queue)
{
	if (queue->must_free) {
		free(queue);
	}
}

/**
 * queue_push
 *
 * Pushes a new item onto a queue
 *  - queue: The queue
 *  - item: The item
 */
void queue_push(queue_t* queue, queue_item_t* item)
{
	if (queue_empty(queue)) {
		queue->top = queue->bottom = item;
		item->prev = item->next = NULL;
		return;
	}
	item->prev = NULL;
	item->next = queue->top;
	queue->top = item;
}

/**
 * queue_pop
 *
 * Pops an item from the queue
 *  - queue: The queue
 * returns: The item
 */
queue_item_t* queue_pop(queue_t* queue)
{
	if (queue_empty(queue)) {
		return NULL;
	}

	queue_item_t* item = queue->bottom;
	if (item->prev != NULL) {
		item->prev->next = NULL;
		queue->bottom = item->prev;
	} else {
		queue->top = queue->bottom = NULL;
	}
	return item;
}

/**
 * queue_empty
 *
 * Checks if a queue is empty
 *  - queue: The queue
 * returns: Whether the queue is empty
 */
bool queue_empty(queue_t* queue)
{
	return queue->top == (queue_item_t*)NULL || queue->bottom == (queue_item_t*)NULL;
}
