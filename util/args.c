#include <util/args.h>

#include <getopt.h>
#include <stdio.h>

#include <version.h>

static struct option program_options[] = {
	{"verbose", no_argument, &inst_args.verbose_on, 1},
	{"version", no_argument, 0, 'v'},
	{"help", no_argument, 0, 'h'},
};

void parse_args(int argc, char **argv)
{
	int c = 0;
	while (c >= 0) {
		int option_index = 0;
		c = getopt_long(argc, argv, "vh", program_options, &option_index);
		if (c < 0) {
			break;
		}
		switch (c) {
		case 'v':
			break;
		case 'h':
			break;
		}
	}
}
