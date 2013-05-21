#include <core/log.h>

#include <string.h>

#define LOG_FORMAT "%s/%s: %s\n"

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
