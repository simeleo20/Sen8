#ifndef EDITOR_H
#define EDITOR_H

#include "../types/types.h"
#include "../core.h"

enum editorPage
{
    TILE_EDITOR,
    SCRIPT_EDITOR
};

void button(int x, int y, u8 w, u8 h, void (*draw)(int, int), void (*callback)(void));
void drawRect(int x1, int y1, int x2, int y2, u8 color, u8 z);
void drawHorizontaFilledLine(int x1, int x2, int y, u8 color, u8 z);

int editorLoop();

#endif // EDITOR_H