#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "raylib.h"
#include "types/types.h"

Vector2 calcMousePosition();
bool btn(u8 btncode);
int getKeyPressed();
int getCharPressed();

#endif // GRAPHICS_H