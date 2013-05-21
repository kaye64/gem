CFLAGS = -std=c99 -Wall
INCLUDE_DIRS = -Iinclude/

SUBDIRS = init/

OBJECTS =

include $(addsuffix makefile.mk, $(SUBDIRS))

all: server

server: $(OBJECTS)
	gcc -o $@ $<
