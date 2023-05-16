DIRS := *.c
CFILES := $(foreach DIR, $(DIRS), $(wildcard src/$(DIR)))
OFILES := $(patsubst src/%.c, build/%.o, $(CFILES))

INCLUDES = -I./include -I./src -I/usr/local/include -I./libs/glfw/include -I./libs/cglm/include -I./libs/stb/include -I./libs/glad/include

ifeq ($(OS), Windows_NT)

all: build/libjot.dll

init:
	mkdir build

build/libjot.a: $(OFILES)
	ar -rc $@ $(OFILES)
	
install: uninstall build/libjot.a
	xcopy build\libjot.a C:\MinGW\lib
	xcopy include C:\MinGW\include\jot /E /I

uninstall:
	del C:\MinGW\lib\libjot.a
	if exist C:\MinGW\include\jot del /f /s /q C:\MinGW\include\jot
	if exist C:\MinGW\include\jot rmdir /s /q C:\MinGW\include\jot

hello: install examples/hello.c
	gcc -o build/hello.exe -Iinclude -Lbuild examples/hello.c -ljot -lglfw3 -lgdi32 -lopengl32

balls: install examples/balls.c
	gcc -o build/balls.exe -O3 -Iinclude -Lbuild examples/balls.c -ljot -lglfw3 -lgdi32 -lopengl32

centipede: install examples/centipede.c
	gcc -o build/centipede.exe -O3 -Iinclude -Lbuild examples/centipede.c -ljot -lglfw3 -lgdi32 -lopengl32

DEMO_SOURCE = examples/demo/main.c examples/demo/systems.c examples/demo/types.c
demo: install ${DEMO_SOURCE}
	gcc -o build/demo.exe -O3 -Iinclude -Lbuild ${DEMO_SOURCE} -ljot -lglfw3 -lgdi32 -lopengl32

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
	gcc $(INCLUDES) -Iinclude -Lbuild -o build/$@ examples/balls.c -ljot -lglfw3

centipede: build/libjot.a examples/centipede.c
	gcc $(INCLUDES) -Iinclude -Lbuild -o build/$@ examples/centipede.c -ljot -lglfw3

demo: build/libjot.a examples/demo/main.c examples/demo/types.c examples/demo/systems.c
	gcc $(INCLUDES) -Iinclude -Lbuild -o build/$@ examples/demo/main.c examples/demo/types.c examples/demo/systems.c -ljot -lglfw3

clean:
	rm -r -f build demo build/libjot.a

endif

build/%.o: src/%.c
	gcc -c $(INCLUDES) -Wall -o $@ $<
