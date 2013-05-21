CFLAGS = -std=c99 -Wall -Lrunite/
INCLUDE_DIRS = -Iinclude/
OUT = server
LIBS = -lrunite
SUBDIRS = init core

OBJECTS :=

include $(addsuffix /makefile.mk, $(SUBDIRS))

all: $(OUT)

$(OUT): $(OBJECTS) runite/librunite.a
	gcc $(CFLAGS) $(LIBS) -o $@ $^

runite/librunite.a:
	make -C runite/

%.o: %.c
	gcc -c $(CFLAGS) $(LIBS) $(INCLUDE_DIRS) -o $@ $^

clean:
	make -C runite/ clean
	rm $(OUT) $(OBJECTS)
