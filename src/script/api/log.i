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

%module(package="gem.api") Log

%{
#include <util/log.h>
%}

%rename("%(camelcase)s") "";

%{
	void error(const char* tag, const char* message)
	{
		log_error(tag, message);
	}

	void warn(const char* tag, const char* message)
	{
		log_warn(tag, message);
	}


	void info(const char* tag, const char* message)
	{
		log_info(tag, message);
	}


	void debug(const char* tag, const char* message)
	{
		log_debug(tag, message);
	}
%}

void error(const char* tag, const char* message);
void warn(const char* tag, const char* message);
void info(const char* tag, const char* message);
void debug(const char* tag, const char* message);
