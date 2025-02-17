CC = gcc
CFLAGS = -Wall -Wextra -I"C:/raylib/raylib/src" -I"C:\Program Files\Lua\include"
LDFLAGS = -L"C:/raylib/raylib/src" -L"C:\Program Files\Lua" -llua54
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

graphics: graphics.c lua.c
	$(CC) $(CFLAGS) -o graphics graphics.c lua.c $(LDFLAGS) $(LIBS)
	graphics

lua: lua.c
	$(CC) $(CFLAGS) -o luap lua.c $(LDFLAGS) $(LIBS)
	luap.exe

clean:
	del graphics.exe *.o