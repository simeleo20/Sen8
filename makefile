CC = gcc
CFLAGS = -Wall -Wextra -I"C:/raylib/raylib/src" -I"C:/Program Files/Lua/include"
LDFLAGS = -L"C:/raylib/raylib/src" -L"C:/Program Files/Lua" -llua54
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
SRC = $(wildcard *.c) $(wildcard editor/*.c) $(wildcard types/*.c)
OBJ = $(SRC:.c=.o)
TARGET = graphics

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -g $(LDFLAGS) $(LIBS)
	./graphics.exe

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /f $(TARGET).exe $(subst /,\,$(OBJ))
