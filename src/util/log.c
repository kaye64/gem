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

/**
 * log.c
 *
 * Provides rudimentary, tiered logging functionality
 */
#include <util/log.h>

#include <string.h>

#define LOG_FORMAT "%s/%s: %s\n"

/**
 * Produces a log message prefixed with the 'E' tag
 */
void log_error(const char *tag, const char *fmt, ...)
{
	char buffer[512];
	strcpy(buffer, "");

	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	fprintf(stderr, LOG_FORMAT, "E", tag, buffer);
}

/**
 * Produces a log message prefixed with the 'W' tag
 */
void log_warn(const char *tag, const char *fmt, ...)
{
	char buffer[512];
	strcpy(buffer, "");

	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	fprintf(stderr, LOG_FORMAT, "W", tag, buffer);
}

/**
 * Produces a log message prefixed with the 'I' tag
 */
void log_info(const char *tag, const char *fmt, ...)
{
	char buffer[512];
	strcpy(buffer, "");

	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	fprintf(stderr, LOG_FORMAT, "I", tag, buffer);
}

/**
 * Produces a log message prefixed with the 'D' tag
 */
void log_debug(const char *tag, const char *fmt, ...)
{
	char buffer[512];
	strcpy(buffer, "");

	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	fprintf(stderr, LOG_FORMAT, "D", tag, buffer);
}
