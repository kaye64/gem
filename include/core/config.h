#ifndef _CONFIG_H_
#define _CONFIG_H_

struct config_t {
	int verbose_on;
} inst_config;

void parse_args(int argc, char **argv);

#endif /* _CONFIG_H_ */
