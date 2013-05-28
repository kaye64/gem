#include <init/signal.h>
#include <util/args.h>
#include <util/log.h>
#include <jaggrab/jaggrab.h>
#include <runite/cache.h>

#include <stdio.h>
#include <stdlib.h>

#define LOG_TAG "init"

struct {
	cache_t* cache;
	archive_server_t* jag_server;
} instance;

void cleanup(int forceful);

int main(int argc, char **argv) {
	INFO("Starting up...");
	sig_install(cleanup);
	parse_args(argc, argv);
	instance.cache = cache_open_dir(instance.cache, inst_args.cache_dir);
	if (instance.cache == NULL) {
		ERROR("Unable to find cache");
		return 1;
	}
	instance.jag_server = jaggrab_create(instance.cache, inst_args.bind_addr);
	jaggrab_start(instance.jag_server);
	server_poll((server_t*)instance.jag_server, 1);
	return 0;
}

void cleanup(int forceful) {
	INFO("Cleaning up for shutdown");
	server_stop((server_t*)instance.jag_server);
	jaggrab_free(instance.jag_server);
	cache_free(instance.cache);
}
