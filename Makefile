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

demo: install examples/main.c
	gcc -o build/demo.exe -Iinclude -Lbuild examples/main.c -ljot

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

demo: build/libjot.a examples/main.c
	gcc $(INCLUDES) -L./build -lglfw -ljot -o $@ examples/main.c

clean:
	rm -r -f build demo build/libjot.a

endif

build/%.o: src/%.c
	gcc -c $(INCLUDES) -Wall -o $@ $<
