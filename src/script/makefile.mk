OBJECTS += $(addprefix src/script/,engine.o hook.o)

SUBDIRS = src/script/api

include $(addsuffix /makefile.mk, $(SUBDIRS))
