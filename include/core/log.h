#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>
#include <stdio.h>

void log_error(const char *tag, const char *fmt, ...);
void log_warn(const char *tag, const char *fmt, ...);
void log_info(const char *tag, const char *fmt, ...);
void log_debug(const char *tag, const char *fmt, ...);

#endif /* _LOG_H_ */
