#include <init/signal.h>

#include <signal.h>

exit_func_t exit_func;

void sig_install(exit_func_t exit)
{
	exit_func = exit;
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	signal(SIGQUIT, sig_handler);
}

void sig_handler(int signal)
{
	switch (signal) {
	case SIGINT:
	case SIGTERM:
		exit_func(0);
		break;
	case SIGQUIT:
		exit_func(1);
		break;
	}
}
