#include <init/signal.h>
#include <util/args.h>
#include <util/log.h>
#include <jaggrab/jaggrab.h>
#include <runite/cache.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LOG_TAG "init"
#define ENGINE_TICK_RATE 0.6

struct {
	cache_t* cache;
	archive_server_t* jag_server;
	pthread_t engine_thread;
	pthread_t io_thread;
	struct ev_loop* engine_loop;
	struct ev_loop* io_loop;
	ev_timer engine_tick;
} instance;

void tick();
void cleanup(int forceful);
void io_thread();
void engine_thread();

int main(int argc, char **argv)
{
	INFO("Starting up...");
	sig_install(cleanup);
	parse_args(argc, argv);

	/* open the cache */
	instance.cache = cache_open_dir(instance.cache, inst_args.cache_dir);
	if (instance.cache == NULL) {
		ERROR("Unable to find cache");
		return 1;
	}

	/* create the main game threads */
	pthread_create(&instance.io_thread, NULL, (void*)io_thread, (void*)NULL);
	pthread_create(&instance.engine_thread, NULL, (void*)engine_thread, (void*)NULL);

	pthread_join(instance.engine_thread, NULL);
	return 0;
}

void io_thread()
{
	instance.io_loop = ev_loop_new(EVBACKEND_EPOLL | EVFLAG_NOENV);

	/* create the archive server */
	instance.jag_server = jaggrab_create(instance.cache, inst_args.bind_addr);
	assert(instance.jag_server != 0);
	jaggrab_start(instance.jag_server, instance.io_loop);

	ev_loop(instance.io_loop, 0);
}

void engine_thread()
{
	instance.engine_loop = ev_default_loop(0);

	/* create the engine tick timer */
	ev_timer_init(&instance.engine_tick, tick, 0.0, ENGINE_TICK_RATE);
	ev_timer_start(instance.engine_loop, &instance.engine_tick);

	ev_loop(instance.engine_loop, 0);
}

void tick()
{
	ev_timer_again(instance.engine_loop, &instance.engine_tick);
}

void cleanup(int forceful) {
	INFO("Cleaning up for shutdown");
	ev_timer_stop(instance.engine_loop, &instance.engine_tick);
	server_stop((server_t*)instance.jag_server);
	jaggrab_free(instance.jag_server);
	cache_free(instance.cache);
}
