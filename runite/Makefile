CFLAGS = -std=c99 -Wall
INCLUDE_DIRS = -Iinclude/
OUT = librunite.a

SUBDIRS =

OBJECTS :=

include $(addsuffix /makefile.mk, $(SUBDIRS))

all: $(OUT)

$(OUT): $(OBJECTS)
	ar rcs $@ $^

%.o: %.c
	gcc -c $(CFLAGS) $(INCLUDE_DIRS) -o $@ $^

clean:
	rm $(OUT) $(OBJECTS)
