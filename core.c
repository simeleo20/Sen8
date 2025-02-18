#include "types.h"
#include "core.h"
#include "lua.h"
#include "raylib.h"
#include <stdlib.h>
#include "oslua.c"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240

// Memory banks for sprites and background
tile spritesTileMem[256];
tile bgTilesMem[256];

u8 bgMap[60][64]; 
int scrollX = 0;
int scrollY = 0;

sprite sprites[64];

u8 screen[SCREEN_HEIGHT][SCREEN_WIDTH]; //32*30

string script;
languageE language = LUA;

// colors palette
const rgb palette[16] = {
    {0, 0, 0},       // black 0
    {29, 43, 83},    // dark-blue 1
    {126, 37, 83},   // dark-purple 2
    {0, 135, 81},    // dark-green 3
    {171, 82, 54},   // brown 4
    {95, 87, 79},    // dark-grey 5
    {194, 195, 199}, // light-grey 6
    {255, 241, 232}, // white 7
    {255, 0, 77},    // red
    {255, 163, 0},   // orange
    {255, 236, 39},  // yellow
    {0, 228, 54},    // green
    {41, 173, 255},  // blue
    {131, 118, 156}, // lavender
    {255, 119, 168}, // pink
    {255, 204, 170}  // light-peach
};

void coreVBLANK()
{
    if(language == LUA)
    {
        execLuaVBLANK();
    }
}
void coreLoop()
{
    if(language == LUA)
    {
        execLuaLoop();
    }
}
string fileToString(cstring filename)
{
    int fileSize;
    unsigned char *buffer = LoadFileData(filename, &fileSize);
    string out = malloc(fileSize+1);
    int i;
    for(i = 0; i < fileSize; i++)
    {
        out[i] = buffer[i];
    }
    out[i] = '\0';
    return out;
}
void coreSetup()
{
    free(script);
    //script = fileToString("os.lua");

    script = malloc(strlen(os) + 1);
    strcpy(script, os);
    
    
    if(language == LUA)
    {
        initLua(script);
        execLuaSetup();
    }
}
void closeScript()
{
    if(language == LUA)
    {
        closeLua();
    }
}



void pixelToScreen(int x, int y, u8 color)
{
    x -= scrollX;
    x %= SCREEN_WIDTH*2;
    y -= scrollY;
    y %= SCREEN_HEIGHT*2;
    if(x < 0) x += SCREEN_WIDTH*2;
    if(y < 0) y += SCREEN_HEIGHT*2;
    if(x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;

    screen[y][x] = color;
}

void drawTile(int x, int y, tile t)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            pixelToScreen(x*8+i,y*8+j, t[j][i]);
        }
    }
}

void drawBackground()
{
    for(int y = 0; y < 60; y++)
    {
        for(int x = 0; x < 64; x++)
        {
            drawTile(x, y, bgTilesMem[bgMap[y][x]]);
        }
    }
}

void drawSprites()
{
    for(int i = 0; i < 64; i++)
    {
        sprite s = sprites[i];
        drawTile(s.x, s.y, spritesTileMem[s.tileIndex]);
    }
}

void corePPUDraw()
{
    drawBackground();
    drawSprites();
}

void bgSet(u16 x, u16 y, u8 tileIndex)
{
    if(x >= 64 || y >= 60) return;
    bgMap[y][x] = tileIndex;
}
u8 bgGet(u16 x, u16 y)
{
    if(x >= 64 || y >= 60) return -1;
    return bgMap[y][x];
}

void setScrollX(int x)
{
    scrollX = x;
}
void setScrollY(int y)
{
    scrollY = y;
}

void setSprite(u8 index, int x, int y, u8 tileIndex, bool flipH, bool flipV, bool priority)
{
    sprites[index].x = x;
    sprites[index].y = y;
    sprites[index].tileIndex = tileIndex;
    sprites[index].flipH = flipH;
    sprites[index].flipV = flipV;
    sprites[index].priority = priority;
}
void setSpriteX(u8 index, int x)
{
    sprites[index].x = x;
}
void setSpriteY(u8 index, int y)
{
    sprites[index].y = y;
}
void setSpriteTileIndex(u8 index, u8 tileIndex)
{
    sprites[index].tileIndex = tileIndex;
}
void setSpriteFlipH(u8 index, bool flipH)
{
    sprites[index].flipH = flipH;
}
void setSpriteFlipV(u8 index, bool flipV)
{
    sprites[index].flipV = flipV;
}
void setSpritePriority(u8 index, bool priority)
{
    sprites[index].priority = priority;
}


tile prova = {
    {8, 8, 8, 8, 8, 8, 8, 8},
    {1, 1, 1, 8, 1, 1, 1, 1},
    {1, 1, 1, 8, 1, 1, 1, 1},
    {1, 1, 1, 8, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {8, 8, 8, 8, 8, 8, 8, 8},
};
void populateBG()
{
    tile a ={{1, 1, 1, 1, 1, 1, 1, 1},
             {1, 1, 1, 1, 1, 1, 1, 1},
             {1, 1, 1, 1, 7, 7, 1, 1},
             {1, 1, 1, 1, 1, 1, 7, 1},
             {1, 1, 1, 1, 7, 7, 7, 1},
             {1, 1, 1, 7, 1, 1, 7, 1},
             {1, 1, 1, 1, 7, 7, 7, 1},
             {1, 1, 1, 1, 1, 1, 1, 1}};
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            bgTilesMem[1][i][j] = a[i][j];
        }
    }
}
void initScreen()
{
    for(int x = 0; x < 256; x++)
    {
        for(int y = 0; y < 240; y++)
        {
            screen[y][x] = 1;
        }
    }
    scrollX = 0;
    //drawTile(64, 0, prova);
    //setup sprites
    for(int i = 0; i < 64; i++)
    {
        sprites[i].x = -1;
        sprites[i].y = -1;
        sprites[i].tileIndex = 0;
        sprites[i].flipH = false;
        sprites[i].flipV = false;
        sprites[i].priority = false;
    }
    //setup bgmem

    for(int i = 0; i < 256; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                bgTilesMem[i][j][k] = 1;//<-----------------da mettere zero
            }
        }
    }
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            bgTilesMem[1][i][j] = prova[i][j];
        }
    }
    populateBG();
}


