/**
 *  This file is part of Gem.
 *
 *  Gem is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gem is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gem.  If not, see <http://www.gnu.org/licenses/\>.
 */

#ifndef _SORTED_LIST_H_
#define _SORTED_LIST_H_

struct link {
	void* data;
	struct link* prev;
	struct link* next;
};
typedef struct link link_t;

typedef int(*compare_func_t)(void*,void*);

void slist_free(link_t* link);
link_t* slist_top(link_t* link);
link_t* slist_bottom(link_t* link);
link_t* slist_insert_before(link_t* link, void* data);
link_t* slist_insert_after(link_t* link, void* data);
link_t* slist_insert(link_t* link, void* data, compare_func_t compare);
void slist_remove(link_t* link);

#endif /* _SORTED_LIST_H_ */
