#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include <stdbool.h>

// void exit_func(bool forceful);
typedef void(*exit_func_t)(bool);

void sig_install(exit_func_t exit);
void sig_handler(int signal);

#endif /* _SIGNAL_H_ */
