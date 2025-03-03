CC = gcc
CFLAGS = -Wall -Wextra -I"C:/raylib/raylib/src" -I"C:/Program Files/Lua/include"
LDFLAGS = -L"C:/raylib/raylib/src" -L"C:/Program Files/Lua/lua-5.4.7/src"  #-llua
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
SRC = $(wildcard *.c) $(wildcard editor/*.c) $(wildcard types/*.c) $(wildcard C:/Program Files/Lua/lua-5.4.7/src/*.c) 
OBJ = $(SRC:.c=.o)
TARGET = graphics

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -g $(LDFLAGS) $(LIBS)
	./graphics.exe

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

web:
	emcc -o game.html $(filter %.c,$(SRC)) -Os $(CFLAGS) "C:\raylib\raylib\src\web\libraylib.a" "./lib/lua/liblua.a" -L"C:/raylib/raylib/src" -I"C:/raylib/raylib/src" -I"C:/emsdk/emsdk-4.0.3/upstream/emscripten/cache/sysroot/include" -s USE_GLFW=3 -s --shell-file "./minshell.html" -DPLATFORM_WEB

clean:
	del /f $(TARGET).exe $(subst /,\,$(OBJ))
