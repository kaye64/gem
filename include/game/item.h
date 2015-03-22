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

#ifndef _ITEM_H_
#define _ITEM_H_

#include <game/item_definition.h>

#ifdef SWIG
#define SWIG_DEF(x) x;
#else
#define SWIG_DEF(x)
#endif /* SWIG */

SWIG_DEF(%rename(Item) item);
struct item {
	item_definition_t definition;
	int stack_count;
#ifdef SWIG
	%extend {
		item(int id) {
			item_t item = item_create_by_id(id);
			item_t* dyn_item = (item_t*)malloc(sizeof(item_t));
			memcpy(dyn_item, &item, sizeof(item_t));
			return dyn_item;
		}
		~item() {
			free($self);
		}
	};
#endif /* SWIG */
};
typedef struct item item_t;

#ifndef SWIG
item_t item_create(item_definition_t definition);
item_t item_create_by_id(int id);
#endif /* SWIG */

#endif /* _ITEM_H_ */
