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

#ifndef _SCRIPT_REFLECT_H_
#define _SCRIPT_REFLECT_H_

#include <Python.h>

typedef void* ref_table_get_t(void* self, void* extra);
typedef int ref_table_set_t(void* self, void* value, void* extra);

typedef struct ref_table_entry ref_table_entry_t;
typedef ref_table_entry_t* ref_table_t;

struct ref_table_entry {
	const char* identifier;
	ref_table_get_t* get_func;
	ref_table_set_t* set_func;
	void* extra;
};

void* reflect_get(void* self, ref_table_entry_t* table, const char* identifier);
int reflect_set(void* self, ref_table_entry_t* table, const char* identifier, void* value);

PyObject* reflect_get_pyint(void* self, void* offset);
int reflect_set_pyint(void* self, PyObject* value, void* offset);

PyObject* reflect_get_pystring(void* self, void* offset);
int reflect_set_pystring(void* self, PyObject* value, void* offset);

#define reflect_pyint_field(ident, type, field)		\
	{										   	\
		.identifier = ident,				\
		.get_func = &reflect_get_pyint,	\
		.set_func = &reflect_set_pyint,	\
		.extra = offsetof(type, field),		\
	}											\

#define reflect_pystring_field(ident, type, field)		\
	{										   	\
		.identifier = ident,				\
		.get_func = &reflect_get_pystring,	\
		.set_func = &reflect_set_pystring,	\
		.extra = offsetof(type, field),		\
	}											\

#define reflect_terminator	\
	{					   	\
		.identifier = NULL,	\
		.get_func = NULL,	\
		.set_func = NULL,	\
		.extra = 0,			\
	}						\

#endif /* _SCRIPT_REFLECT_H_ */
