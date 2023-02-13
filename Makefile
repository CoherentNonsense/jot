DIRS := *.c
CFILES := $(foreach DIR, $(DIRS), $(wildcard src/$(DIR)))
OFILES := $(patsubst src/%.c, build/%.o, $(CFILES))

INCLUDES = -I./include -I./src -I/usr/local/include -I./libs/glfw/include -I./libs/cglm/include -I./libs/stb/include -I./libs/glad/include

ifeq ($(OS), Windows_NT)

all: build/libjot.dll

init:
	mkdir build

build/libjot.dll: $(OFILES)
	gcc -shared -o $@ $(OFILES) -L./libs/glfw/build/src -lglfw3 -lgdi32 -lopengl32

install: uninstall build/libjot.dll
	xcopy build\libjot.dll C:\MinGW\lib
	xcopy include C:\MinGW\include\jot /E /I

uninstall:
	del C:\MinGW\lib\libjot.dll
	if exist C:\MinGW\include\jot del /f /s /q C:\MinGW\include\jot
	if exist C:\MinGW\include\jot rmdir /s /q C:\MinGW\include\jot

hello: install examples/hello.c
	gcc -o build/demo.exe -Iinclude -Lbuild examples/hello.c -ljot

balls: install examples/balls.c
	gcc -o build/balls.exe -Iinclude -Lbuild examples/balls.c -ljot

# -lglfw3 -lgdi32 -lopengl32
else

all: build/libjot.a

init:
	mkdir -p build

build/libjot.a: init $(OFILES)
	ar -rc $@ $(OFILES)

install: uninstall build/libjot.a
	cp build/libjot.a /usr/local/lib
	cp -r include /usr/local/include/jot

uninstall:
	rm -f /usr/local/lib/libjot.a
	rm -f -r /usr/local/include/jot

hello: build/libjot.a examples/hello.c
	gcc $(INCLUDES) -L./build -lglfw -ljot -o build/$@ examples/hello.c

balls: build/libjot.a examples/balls.c
	gcc $(INCLUDES) -Iinclude -Lbuild -o build/$@ examples/balls.c -ljot -lglfw

centipede: build/libjot.a examples/centipede.c
	gcc $(INCLUDES) -Iinclude -Lbuild -o build/$@ examples/centipede.c -ljot -lglfw

demo: build/libjot.a examples/demo/main.c examples/demo/types.c examples/demo/systems.c
	gcc $(INCLUDES) -Iinclude -Lbuild -o build/$@ examples/demo/main.c examples/demo/types.c examples/demo/systems.c -ljot -lglfw

clean:
	rm -r -f build demo build/libjot.a

endif

build/%.o: src/%.c
	gcc -c $(INCLUDES) -Wall -o $@ $<
