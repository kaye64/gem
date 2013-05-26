#ifndef _CONFIG_H_
#define _CONFIG_H_

struct args_t {
	int verbose_on;
} inst_args;

void parse_args(int argc, char **argv);

#endif /* _CONFIG_H_ */
