/**
 * queue.c
 *
 * A FIFO queue implementation
 */
#include <util/queue.h>

#include <stdlib.h>
#include <assert.h>

/**
 * Initializes a new queue
 */
static void queue_init(queue_t* queue)
{
	object_init(list, &queue->list);
}

/**
 * Properly frees a queue_t
 */
static void queue_free(queue_t* queue)
{
	object_free(&queue->list);
}

/**
 * Pushes a new item onto a queue
 */
void queue_push(queue_t* queue, list_node_t* item)
{
	assert(item != NULL);
	list_push_front(&queue->list, item);
}

/**
 * Pops an item from the queue
 */
list_node_t* queue_pop(queue_t* queue)
{
	list_node_t* node = list_back(&queue->list);
	assert(node != NULL);
	list_erase(&queue->list, node);
	return node;
}

/**
 * Returns the next item in the queue without removing it
 */
list_node_t* queue_peek(queue_t* queue)
{
	list_node_t* node = list_back(&queue->list);
	assert(node != NULL);
	return node;
}

/**
 * Checks if a queue is empty
 */
bool queue_empty(queue_t* queue)
{
	return list_empty(&queue->list);
}

/**
 * Empties a queue
 */
void queue_clear(queue_t* queue)
{
	while (!queue_empty(queue)) {
		queue_pop(queue);
	}
}

object_proto_t queue_proto = {
	.init = (object_init_t)queue_init,
	.free = (object_free_t)queue_free
};
