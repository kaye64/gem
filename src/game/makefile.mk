OBJECTS += $(addprefix src/game/,dispatcher.o game_service.o update_service.o game_login.o entity.o entity_list.o entity_tracker.o mob.o player.o world.o location.o waypoint_queue.o chat_message.o item_definition.o item.o item_collection.o)

SUBDIRS = src/game/packet src/game/packet/builder src/game/packet/routine

include $(addsuffix /makefile.mk, $(SUBDIRS))
