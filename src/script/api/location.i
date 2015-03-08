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

%module Location

%{
#include <game/location.h>
%}


%rename("%(camelcase)s") "";

%include "game/location.h"

%{
	PyObject* location_wrap_from_location(location_t loc) {
		PyObject* result = SWIG_NewPointerObj(SWIG_as_voidptr(&loc), SWIGTYPE_p_location, SWIG_POINTER_NEW | 0 );
		return result;
	}

	location_t location_from_location_wrap(PyObject* loc) {
		location_t native_obj;
		void* p;
		int res = SWIG_ConvertPtr(loc, &p, SWIGTYPE_p_location, 0 );
		if (!SWIG_IsOK(res)) {
			printf("Couldnt convert location\n");
		}
		native_obj = *((location_t*)p);
		return native_obj;
	}
%}
