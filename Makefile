CFLAGS = -g -std=gnu99 -Wall -Lrunite/
INCLUDE_DIRS = -Iinclude/ -Irunite/include/
OUT = server
LIBS = -lrunite -lev -lz -lpthread
SUBDIRS = init util net crypto jaggrab world

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
