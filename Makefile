DIRS := *.c
CFILES := $(foreach DIR, $(DIRS), $(wildcard src/$(DIR)))
OFILES := $(patsubst src/%.c, build/%.o, $(CFILES))

INCLUDES = -I./include -I./src -I/usr/local/include -I./libs/cglm/include -I./libs/stb/include -I./libs/glad/include

all: build/libjot.a

init:
	mkdir -p build

build/%.o: src/%.c
	gcc -c $(INCLUDES) -Wall -o $@ $<

build/libjot.a: init $(OFILES)
	ar -rc $@ $(OFILES)

install: uninstall build/libjot.a
	cp build/libjot.a /usr/local/lib
	cp -r include /usr/local/include/jot

uninstall:
	rm -f /usr/local/lib/libjot.a
	rm -f -r /usr/local/include/jot

demo: build/libjot.a examples/main.c
	gcc $(INCLUDES) -L./build -lglfw -ljot -o $@ examples/main.c

clean:
	rm -r -f build demo build/libjot.a