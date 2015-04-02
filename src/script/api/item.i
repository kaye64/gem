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

%module(package="gem.api") Item

%{
#include <game/item_definition.h>
#include <game/item.h>
#include <game/item_collection.h>

typedef struct item_collection item_collection;
%}

%include "pyabc.i"

%include "game/item_definition.h"

%types(item = item_definition) "return &((item_t*)$from)->definition;";
%pythonabc(item, Definition);
%include "game/item.h"

%rename(Collection, %$isclass) item_collection;
typedef struct {
	int size;
	%extend {
		int insert(item_t item);
		int insert_at(int index, item_t item);
		int contains(item_definition_t definition);
		item_t* find_first(item_definition_t definition);
		int count(item_definition_t definition);
		int remove(item_definition_t definition, int count);
		item_t* get(int index);
	}
} item_collection;
