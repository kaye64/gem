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
	#define SCRIPT_LOG_TAG "script"

	void format_tag(char* out_tag, const char* script_tag)
	{
		sprintf(out_tag, "%s/%s", SCRIPT_LOG_TAG, script_tag);
	}

	void error(const char* script_tag, const char* message)
	{
		char tag[32];
		format_tag(tag, script_tag);
		log_error(tag, message);
	}

	void warn(const char* script_tag, const char* message)
	{
		char tag[32];
		format_tag(tag, script_tag);
		log_warn(tag, message);
	}


	void info(const char* script_tag, const char* message)
	{
		char tag[32];
		format_tag(tag, script_tag);
		log_info(tag, message);
	}


	void debug(const char* script_tag, const char* message)
	{
		char tag[32];
		format_tag(tag, script_tag);
		log_debug(tag, message);
	}

	void session(const char* script_tag, const char* message)
	{
		char tag[32];
		format_tag(tag, script_tag);
		log_session(tag, message);
	}
%}

void error(const char* script_tag, const char* message);
void warn(const char* script_tag, const char* message);
void info(const char* script_tag, const char* message);
void debug(const char* script_tag, const char* message);
void session(const char* script_tag, const char* message);
