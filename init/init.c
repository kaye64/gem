/**
 * init.c
 *
 * Performs Gem startup routine
 */
#include <init/signal.h>
#include <util/args.h>
#include <util/config.h>
#include <util/log.h>
#include <jaggrab/jaggrab.h>
#include <runite/cache.h>
#include <world/dispatcher.h>
#include <world/game_service.h>
#include <world/update_service.h>
#include <crypto/rsa.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LOG_TAG "init"
#define ENGINE_TICK_RATE 0.6
#define IO_TICK_RATE 0.2

struct {
	cache_t* cache;
	archive_server_t* jag_server;
	dispatcher_t* world_dispatcher;
	game_service_t* game_service;
	update_service_t* update_service;
	pthread_t engine_thread;
	pthread_t io_thread;
	struct ev_loop* engine_loop;
	struct ev_loop* io_loop;
	ev_timer engine_tick;
	ev_timer io_tick;
	rsa_t rsa;
} instance;

void engine_tick();
void io_tick();
void cleanup(bool forceful);
void io_thread();
void engine_thread();

int main(int argc, char **argv)
{
	INFO("Starting up...");
	sig_install(cleanup);
	parse_args(argc, argv);

	/* load our rsa private key, generating one if necessary */
	rsa_create(&instance.rsa, RSA_MODULUS, RSA_PUBLIC_EXPONENT, RSA_PRIVATE_EXPONENT);

	/* open the cache */
	instance.cache = cache_open_dir(instance.cache, inst_args.cache_dir);
	if (instance.cache == NULL) {
		ERROR("Unable to find cache");
		return 1;
	}

	/* create the archive server */
	instance.jag_server = jaggrab_create(instance.cache, inst_args.bind_addr);
	assert(instance.jag_server != 0);

	/* create the world dispatcher */
	instance.game_service = game_create(NULL, &instance.rsa, instance.cache);
	instance.update_service = update_create(NULL, instance.cache);
	instance.world_dispatcher = dispatcher_create(inst_args.bind_addr, (service_t*)instance.game_service, (service_t*)instance.update_service);
	assert(instance.game_service != 0);
	assert(instance.update_service != 0);
	assert(instance.world_dispatcher != 0);

	/* create the main game threads */
	pthread_create(&instance.io_thread, NULL, (void*)io_thread, (void*)NULL);
	pthread_create(&instance.engine_thread, NULL, (void*)engine_thread, (void*)NULL);

	pthread_join(instance.io_thread, NULL);
	pthread_join(instance.engine_thread, NULL);
	return 0;
}

/**
 * io_thread
 *
 * The main networking/io thread
 */
void io_thread()
{
	instance.io_loop = ev_loop_new(EVBACKEND_EPOLL | EVFLAG_NOENV);

	/* start the servers listening */
	jaggrab_start(instance.jag_server, instance.io_loop);
	dispatcher_start(instance.world_dispatcher, instance.io_loop);

	ev_loop(instance.io_loop, 0);
}

/**
 * engine_thread
 *
 * The game logic thread
 */
void engine_thread()
{
	instance.engine_loop = ev_default_loop(0);

	/* create the engine tick timer */
	ev_timer_init(&instance.engine_tick, engine_tick, 0.0, ENGINE_TICK_RATE);
	ev_timer_start(instance.engine_loop, &instance.engine_tick);

	/* create the io tick timer */
	ev_timer_init(&instance.io_tick, io_tick, 0.0, IO_TICK_RATE);
	ev_timer_start(instance.engine_loop, &instance.io_tick);

	ev_loop(instance.engine_loop, 0);
}

/**
 * engine_tick
 *
 * The engine tick function.
 * Sends the client periodic update
 */
void engine_tick()
{


	ev_timer_again(instance.engine_loop, &instance.engine_tick);
}

/**
 * io_tick
 *
 * The io tick function.
 * Processes incoming and outgoing packet queues
 */
void io_tick()
{
	game_process_io(instance.game_service);

	ev_timer_again(instance.engine_loop, &instance.io_tick);
}

/**
 * cleanup
 *
 * Cleanly closes all sockets, quits all threads, generally cleans up for shutdown
 *  - forceful: How quickly we need to shutdown. SIGINT/SIGTERM = false, SIGQUIT = true
 */
void cleanup(bool forceful) {
	if (!forceful) {
		INFO("Cleaning up for shutdown");
		server_stop(&instance.jag_server);
		server_stop(&instance.world_dispatcher);
		ev_timer_stop(instance.engine_loop, &instance.engine_tick);
		ev_timer_stop(instance.engine_loop, &instance.io_tick);

		ev_break(instance.io_loop, EVBREAK_ALL);
		ev_break(instance.engine_loop, EVBREAK_ALL);
	}

	dispatcher_free(instance.world_dispatcher);
	game_free(instance.game_service);
	update_free(instance.update_service);

	jaggrab_free(instance.jag_server);

	cache_free(instance.cache);

	exit(EXIT_SUCCESS);
}
