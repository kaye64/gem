#ifndef _SORTED_LIST_H_
#define _SORTED_LIST_H_

struct link {
	void* data;
	struct link* prev;
	struct link* next;
};
typedef struct link link_t;

typedef int(*compare_func_t)(void*,void*);

void list_free(link_t* link);
link_t* list_top(link_t* link);
link_t* list_bottom(link_t* link);
link_t* list_insert_before(link_t* link, void* data);
link_t* list_insert_after(link_t* link, void* data);
link_t* list_insert(link_t* link, void* data, compare_func_t compare);
void list_remove(link_t* link);

#endif /* _SORTED_LIST_H_ */
