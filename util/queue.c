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
	list_create(&queue->list);
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
	list_free(&queue->list);
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
void queue_push(queue_t* queue, list_node_t* item)
{
	list_push_front(&queue->list, item);
}

/**
 * queue_pop
 *
 * Pops an item from the queue
 *  - queue: The queue
 * returns: The item
 */
list_node_t* queue_pop(queue_t* queue)
{
	list_node_t* node = list_back(&queue->list);
	list_erase(&queue->list, node);
	return node;
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
	return list_empty(&queue->list);
}
