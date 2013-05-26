#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>
#include <stdio.h>

void log_error(const char *tag, const char *fmt, ...);
void log_warn(const char *tag, const char *fmt, ...);
void log_info(const char *tag, const char *fmt, ...);
void log_debug(const char *tag, const char *fmt, ...);

#define ERROR(...) log_error(LOG_TAG, __VA_ARGS__)
#define WARN(...) log_warn(LOG_TAG, __VA_ARGS__)
#define INFO(...) log_info(LOG_TAG, __VA_ARGS__)
#define DEBUG(...) log_debug(LOG_TAG, __VA_ARGS__)

#endif /* _LOG_H_ */
