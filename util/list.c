#include <util/list.h>

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

void list_free(list_t* list)
{
	if (list->must_free) {
		free(list);
	}
}

bool list_empty(list_t* list)
{
	return list->top == (list_node_t*)NULL && list->bottom == (list_node_t*)NULL;
}

void list_push_back(list_t* list, list_node_t* node)
{
	if (list_empty(list)) {
		list->top = list->bottom = node;
		node->next = node->prev = (list_node_t*)NULL;
	} else {
		list_insert_after(list, list->bottom, node);
	}
}

void list_push_front(list_t* list, list_node_t* node)
{
	if (list_empty(list)) {
		list->top = list->bottom = node;
		node->next = node->prev = (list_node_t*)NULL;
	} else {
		list_insert_before(list, list->top, node);
	}
}

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

list_node_t* list_back(list_t* list)
{
	return list->bottom;
}

list_node_t* list_front(list_t* list)
{
	return list->top;
}

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
