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

#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>
#include <stdio.h>

void log_error(const char *tag, const char *fmt, ...);
void log_warn(const char *tag, const char *fmt, ...);
void log_info(const char *tag, const char *fmt, ...);
void log_debug(const char *tag, const char *fmt, ...);
void log_session(const char *tag, const char *fmt, ...);

#define ERROR(...) log_error(LOG_TAG, __VA_ARGS__)
#define WARN(...) log_warn(LOG_TAG, __VA_ARGS__)
#define INFO(...) log_info(LOG_TAG, __VA_ARGS__)
#define DEBUG(...) log_debug(LOG_TAG, __VA_ARGS__)
#define SESSION(...) log_session(LOG_TAG, __VA_ARGS__)

#endif /* _LOG_H_ */
