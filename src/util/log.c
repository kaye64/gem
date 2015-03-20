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

#define LOG_FORMAT "%s%s/%s%s: %s\n"

#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_RESET   "\033[0m"

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

	fprintf(stderr, LOG_FORMAT, COLOR_RED, "E", tag, COLOR_RESET, buffer);
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

	fprintf(stderr, LOG_FORMAT, COLOR_YELLOW, "W", tag, COLOR_RESET, buffer);
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

	fprintf(stderr, LOG_FORMAT, COLOR_GREEN, "I", tag, COLOR_RESET, buffer);
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

	fprintf(stderr, LOG_FORMAT, COLOR_CYAN, "D", tag, COLOR_RESET, buffer);
}

/**
 * Produces a log message prefixed with the 'S' tag
 */
void log_session(const char *tag, const char *fmt, ...)
{
	char buffer[512];
	strcpy(buffer, "");

	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	fprintf(stderr, LOG_FORMAT, COLOR_BLUE, "S", tag, COLOR_RESET, buffer);
}
