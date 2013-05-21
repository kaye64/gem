#include <core/config.h>
#include <core/log.h>

#include <stdio.h>

#define LOG_TAG "init"

int main(int argc, char **argv) {
	log_info(LOG_TAG, "Starting up...");
	parse_args(argc, argv);
	return 0;
};
