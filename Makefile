CFLAGS = -g -std=c99 -Wall -Lrunite/
INCLUDE_DIRS = -Iinclude/ -Irunite/include/
OUT = server
LIBS = -lrunite
SUBDIRS = init core

OBJECTS :=

include $(addsuffix /makefile.mk, $(SUBDIRS))

all: $(OUT)

$(OUT): runite/librunite.a $(OBJECTS)
	gcc $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

runite/librunite.a:
	make -C runite/

%.o: %.c
	gcc -c $(CFLAGS) $(LIBS) $(INCLUDE_DIRS) -o $@ $^

clean:
	make -C runite/ clean
	rm $(OUT) $(OBJECTS)
