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

#include <script/reflect.h>

#include <string.h>

#include <util/log.h>

#define LOG_TAG "reflect"

/**
 * Performs a member get using a given reflection table
 */
void* reflect_get(void* self, ref_table_entry_t* table, const char* identifier)
{
	ref_table_entry_t* entry = (ref_table_entry_t*)table;
	ref_table_get_t* get_func = NULL;
	void* extra;
	while (entry->identifier != NULL) {
		if (strcmp(entry->identifier, identifier) == 0) {
			get_func = entry->get_func;
			extra = entry->extra;
			break;
		}
		++entry;
	}

	if (!get_func) {
		return NULL;
	}

	return get_func(self, extra);
}

/**
 * Performs a member set using a given reflection table
 */
int reflect_set(void* self, ref_table_entry_t* table, const char* identifier, void* value)
{
	ref_table_entry_t* entry = (ref_table_entry_t*)table;
	ref_table_set_t* set_func = NULL;
	void* extra;
	while (entry->identifier != NULL) {
		if (strcmp(entry->identifier, identifier) == 0) {
			set_func = entry->set_func;
			extra = entry->extra;
			break;
		}
		++entry;
	}

	if (!set_func) {
		return -1;
	}

	return set_func(self, value, extra);
}

#define reflect_get_offset(self, type, offset) ((type*)((void*)self + (uint64_t)offset))
#define reflect_set_offset(self, type, value, offset)	\
	*(reflect_get_offset(self, type, offset)) = value;

/**
 * Reflection member get which returns a PyInt
 */
PyObject* reflect_get_pyint(void* self, void* offset)
{
	PyObject* result = PyLong_FromLong(*reflect_get_offset(self, int, offset));
	Py_INCREF(result);
	return result;
}

/**
 * Reflection member set which accepts a PyInt
 */
int reflect_set_pyint(void* self, PyObject* value, void* offset)
{
	int native_value = PyLong_AsLong(value);
	reflect_set_offset(self, int, native_value, offset);
	return 0;
}

/**
 * Reflection member get which returns a PyUnicode
 */
PyObject* reflect_get_pystring(void* self, void* offset)
{
	const char* native_value = (const char*)reflect_get_offset(self, char, offset);
	PyObject* result = PyUnicode_FromString(native_value);
	Py_INCREF(result);
	return result;
}

/**
 * Reflection member set which returns a PyUnicode
 */
int reflect_set_pystring(void* self, PyObject* value, void* offset)
{
	char* native_value = PyUnicode_AsUTF8(value);
	strcpy(reflect_get_offset(self, char, offset), native_value);
	return 0;
}
