CFLAGS = -g -std=gnu99 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Lrunite/
INCLUDE_DIRS = -Iinclude/ -Irunite/include/ -I/usr/include/python3.3m/
OUT = gem
LIB_DIRS = -L
LIBS = -lrunite -lev -lz -lpthread -lgmp -lpython3.3m
SUBDIRS = src/init src/util src/net src/crypto src/jaggrab src/script src/game

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
