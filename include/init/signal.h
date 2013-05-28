#ifndef _SIGNAL_H_
#define _SIGNAL_H_

// void exit_func(int forceful);
typedef void(*exit_func_t)(int);

void sig_install(exit_func_t exit);
void sig_handler(int signal);

#endif /* _SIGNAL_H_ */
