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
 * args.c
 *
 * Parses command line arguments
 */
#include <util/args.h>

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <version.h>

static struct option program_options[] = {
	{"verbose", no_argument, &inst_args.verbose_on, 1},
	{"version", no_argument, 0, 'v'},
	{"help", no_argument, 0, 'h'},
	{"bind", required_argument, 0, 'b'},
	{"data", required_argument, 0, 'd'},
	{"content", required_argument, 0, 'c'}
};

void print_usage();
void print_version();

/**
 * Parses command line arguments into inst_args
 */
void parse_args(int argc, char **argv)
{
	int c = 0;

	// init args to their default
	inst_args.verbose_on = 1;
	strcpy(inst_args.bind_addr, "0.0.0.0");
	strcpy(inst_args.cache_dir, "../cache");
	strcpy(inst_args.content_dir, "./content/");

	while (c >= 0) {
		int option_index = 0;
		c = getopt_long(argc, argv, "vhb:c:C:", program_options, &option_index);
		if (c < 0) {
			break;
		}
		switch (c) {
		case 'v':
			print_version();
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			print_usage();
			exit(EXIT_FAILURE);
			break;
		case 'b':
			strcpy(inst_args.bind_addr, optarg);
			break;
		case 'd':
			strcpy(inst_args.cache_dir, optarg);
			break;
		case 'c':
			strcpy(inst_args.content_dir, optarg);
			break;
		}
	}
}

/**
 * Print version information
 */
void print_version()
{
	printf("Geilinor Emulator v%d.%d.%d (gem)\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

/**
 * Print help text
 */
void print_usage()
{
	print_version();
	printf("Usage: gem [OPTION]\n\n"
		   "Optional arguments:\n"
		   "  -b, --bind <address>\t\tspecify the bind address\n"
		   "  -d, --data <directory>\tspecify the game data directory\n"
		   "  -c, --content <directory>\tspecify the content (python script) directory\n"
		   "      --verbose\t\t\tturn on verbose logging\n\n"
		   "  -v, --version\t\toutput version information and exit\n"
		   "  -h, --help\t\tdisplay this help and exit\n");
}
