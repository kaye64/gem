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
 * signal.c
 *
 * The signal handler
 */
#include <init/signal.h>

#include <signal.h>

exit_func_t exit_func;

/**
 * sig_install
 *
 * Installs our signal handler
 *  - exit: An exit_func_t to call upon shutdown
 */
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
		exit_func(false);
		break;
	case SIGQUIT:
		exit_func(true);
		break;
	}
}
