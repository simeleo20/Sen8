CC = gcc
CFLAGS = -Wall -Wextra -I"C:/raylib/raylib/src" -I"C:\Program Files\Lua\include"
LDFLAGS = -L"C:/raylib/raylib/src" -L"C:\Program Files\Lua" -llua54
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

graphics: graphics.c lua.c os.lua oslua.c console.c os.c
	$(CC) $(CFLAGS) -o graphics graphics.c lua.c console.c os.c  $(LDFLAGS) $(LIBS)
	graphics

lua: lua.c
	$(CC) $(CFLAGS) -o luap lua.c $(LDFLAGS) $(LIBS)
	luap.exe

clean:
	del graphics.exe *.o

osCgenerator: os.lua
	$(CC) $(CFLAGS) -o osCgenerator osCgenerator.c $(LDFLAGS) $(LIBS)

oslua.c: os.lua osCgenerator
	osCgenerator

fontGenerator: fontGenerator.c
	$(CC) $(CFLAGS) -o fontGenerator fontGenerator.c $(LDFLAGS) $(LIBS)

console: console.c
	$(CC) $(CFLAGS) -o console console.c $(LDFLAGS) $(LIBS)

os: os.c
	$(CC) $(CFLAGS) -o os os.c $(LDFLAGS) $(LIBS)