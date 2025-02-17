#ifndef CORE_H
#define CORE_H

#include "types.h"

// RGB color structure
typedef struct rgb
{
    u8 r;
    u8 g;
    u8 b;
} rgb;

// 8x8 tile 
typedef u8 tile[8][8];

typedef struct sprite
{
    int x;
    int y;
    u8 tileIndex;
    bool flipH;
    bool flipV;
    bool priority;
} sprite;

//buttons
#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_LEFT 3
#define BUTTON_A 4
#define BUTTON_B 5
#define BUTTON_MOUSEL 6
#define BUTTON_MOUSER 7



void initScreen();
void corePPUDraw();


void bgSet(u16 x, u16 y, u8 tileIndex);
u8 bgGet(u16 x, u16 y);
void setScrollX(int x);
void setScrollY(int y);

#endif // CORE_H