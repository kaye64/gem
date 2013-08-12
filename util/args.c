/**
 * args.c
 *
 * Parses command line arguments
 */
#include <util/args.h>

#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include <version.h>

static struct option program_options[] = {
	{"verbose", no_argument, &inst_args.verbose_on, 1},
	{"version", no_argument, 0, 'v'},
	{"help", no_argument, 0, 'h'},
	{"bind", required_argument, 0, 'b'},
	{"cache", required_argument, 0, 'c'},
	{"content", required_argument, 0, 'C'}
};

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
			break;
		case 'h':
			break;
		case 'b':
			strcpy(inst_args.bind_addr, optarg);
			break;
		case 'c':
			strcpy(inst_args.cache_dir, optarg);
			break;
		case 'C':
			strcpy(inst_args.content_dir, optarg);
			break;
		}
	}
}
