#include <runite/util/sorted_list.h>

#include <stdlib.h>

void slist_free(link_t* link)
{
	link = slist_bottom(link);

	while (link != NULL) {
		link_t* end = link;
		link = link->prev;
		free(end);
	}
}

link_t* slist_top(link_t* link)
{
	while (link->prev != NULL) {
		link = link->prev;
	}
	return link;
}

link_t* slist_bottom(link_t* link)
{
	while (link->next != NULL) {
		link = link->next;
	}
	return link;
}

link_t* slist_insert_before(link_t* link, void* data)
{
	link_t* new_link = (link_t*)malloc(sizeof(link_t));
	new_link->data = data;
	new_link->next = link;

	link_t* prev = link->prev;
	new_link->prev = prev;
	link->prev = new_link;
	if (prev != NULL) {
		prev->next = new_link;
	}
	return new_link;
}

link_t* slist_insert_after(link_t* link, void* data)
{
	link_t* new_link = (link_t*)malloc(sizeof(link_t));
	new_link->data = data;
	new_link->prev = link;

	link_t* next = link->next;
	new_link->next = next;
	link->next = new_link;
	if (next != NULL) {
		next->prev = new_link;
	}
	return new_link;
}

link_t* slist_insert(link_t* link, void* data, compare_func_t compare)
{
	if (link == NULL) {
		link_t* root = (link_t*)malloc(sizeof(link_t));
		root->prev = root->next = NULL;
		root->data = data;
		return root;
	}

	int cmp_result = compare(data, link->data);

	if (cmp_result == 0) {
		return slist_insert_after(link, data);
	}

	if (cmp_result < 0) {
		if (link->prev == NULL) {
			return slist_insert_before(link, data);
		}
		cmp_result = compare(data, link->prev->data);
		if (cmp_result > 0) {
			return slist_insert_before(link, data);
		}
		return slist_insert(link->prev, data, compare);
	}

	if (cmp_result > 0) {
		if (link->next == NULL) {
			return slist_insert_after(link, data);
		}
		cmp_result = compare(data, link->next->data);
		if (cmp_result < 0) {
			return slist_insert_after(link, data);
		}
		return slist_insert(link->next, data, compare);
	}
	// Shouldn't get here
	return slist_insert(link, data, compare);
}

void slist_remove(link_t* link)
{
	if (link->prev == NULL && link->next == NULL) {
		free(link);
		return;
	}
	if (link->prev != NULL && link->next != NULL) {
		link->prev->next = link->next;
		link->next->prev = link->prev;
		free(link);
		return;
	}
	if (link->prev != NULL && link->next == NULL) {
		link->prev->next = NULL;
		free(link);
		return;
	}
	if (link->prev == NULL && link->next != NULL) {
		link->next->prev = NULL;
		free(link);
		return;
	}
}
