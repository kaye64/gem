#include <core/args.h>
#include <core/log.h>
#include <runite/cache.h>

#include <stdio.h>

#define LOG_TAG "init"

int main(int argc, char **argv) {
	INFO("Starting up...");
	parse_args(argc, argv);
	return 0;
}
