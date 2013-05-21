CFLAGS = -std=c99 -Wall
INCLUDE_DIRS = -Iinclude/
OUT = server

SUBDIRS = init core

OBJECTS :=

include $(addsuffix /makefile.mk, $(SUBDIRS))

all: $(OUT)

$(OUT): $(OBJECTS)
	gcc -o $@ $^

%.o: %.c
	gcc -c $(CFLAGS) $(INCLUDE_DIRS) -o $@ $^

clean:
	rm $(OBJECTS)
