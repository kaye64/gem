/**
 * list.c
 *
 * A simple list structure
 */

#include <util/list.h>

/**
 * list_create
 *
 * Creates a new list_t
 *  - list: Some preallocated memory, or NULL to put on heap
 * returns: The list
 */
list_t* list_create(list_t* list)
{
	if (list == NULL) {
		list = (list_t*)malloc(sizeof(list_t));
		list->must_free = true;
	} else {
		list->must_free = false;
	}
	list->top = list->bottom = (list_node_t*)NULL;
	return list;
}

/**
 * list_free
 *
 * Properly frees a list_t
 *  - list: The list
 */
void list_free(list_t* list)
{
	if (list->must_free) {
		free(list);
	}
}

/**
 * list_empty
 *
 * Checks whether a list is empty or not
 *  - list: The list
 * returns: Whether the list is empty
 */
bool list_empty(list_t* list)
{
	return list->top == (list_node_t*)NULL && list->bottom == (list_node_t*)NULL;
}

/**
 * list_push_back
 *
 * Puts a node to the back of the list
 *  - list: The list
 *  - node: The node
 */
void list_push_back(list_t* list, list_node_t* node)
{
	if (list_empty(list)) {
		list->top = list->bottom = node;
		node->next = node->prev = (list_node_t*)NULL;
	} else {
		list_insert_after(list, list->bottom, node);
	}
}

/**
 * list_push_front
 *
 * Puts a node to the front of the list
 *  - list: The list
 *  - node: The node
 */
void list_push_front(list_t* list, list_node_t* node)
{
	if (list_empty(list)) {
		list->top = list->bottom = node;
		node->next = node->prev = (list_node_t*)NULL;
	} else {
		list_insert_before(list, list->top, node);
	}
}

/**
 * list_inert_before
 *
 * Inserts one node before another
 *  - list: The list
 *  - other: The other node
 *  - node: The node
 */
void list_insert_before(list_t* list, list_node_t* other, list_node_t* node)
{
	list_node_t* prev = other->prev;
	other->prev = node;
	if (prev != (list_node_t*)NULL) {
		prev->next = node;
	}
	node->prev = prev;
	node->next = other;
	if (other == list->top) {
		list->top = node;
	}
}

/**
 * list_inert_after
 *
 * Inserts one node after another
 *  - list: The list
 *  - other: The other node
 *  - node: The node
 */
void list_insert_after(list_t* list, list_node_t* other, list_node_t* node)
{
	list_node_t* next = other->next;
	other->next = node;
	if (next != (list_node_t*)NULL) {
		next->prev = node;
	}
	node->prev = other;
	node->next = next;
	if (other == list->bottom) {
		list->bottom = node;
	}
}

/**
 * list_back
 *
 * Returns the node at the back of the list
 *  - list: The list
 * returns: The node
 */
list_node_t* list_back(list_t* list)
{
	return list->bottom;
}

/**
 * list_front
 *
 * Returns the node at the front of the list
 *  - list: The list
 * returns: The node
 */
list_node_t* list_front(list_t* list)
{
	return list->top;
}

/**
 * list_erase
 *
 * Erases a node from the list
 *  - list: The list
 *  - node: The node
 */
void list_erase(list_t* list, list_node_t* node)
{
	list_node_t* prev = node->prev;
	list_node_t* next = node->next;
	if (node == list->top) {
		list->top = node->next;
	}
	if (node == list->bottom) {
		list->bottom = node->prev;
	}
	if (prev != NULL) {
		prev->next = next;
	}
	if (next != NULL) {
		next->prev = prev;
	}
	node->prev = node->next = (list_node_t*)NULL;
}
