#include "types.h"
#include "core.h"
#include "lua.h"
#include <stdlib.h>
#include "oslua.c"
#include "raylib.h"



//current core
core cCore;
//size 127*8
char font[] = {
#include "font.inl"
};
void printZbuffer()
{

}
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
    cls();
    cCore.vblank(&cCore);
}
void coreLoop()
{
    cCore.loop(&cCore);
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

    free(cCore.ram.script);
    cCore.ram.script = fileToString("editor.lua");
    loadTiles();
    //script = malloc(strlen(os) + 1);
    //strcpy(script, os);
    
    
    
    
    initLua(&cCore);
    
    cCore.setup(&cCore);
    
}
void closeScript()
{
    cCore.close(&cCore);
    
}



void pixelToScreen(int x, int y, u8 color, s8 z)
{
    x -= cCore.ram.scrollX;
    x %= SCREEN_WIDTH*2;
    y -= cCore.ram.scrollY;
    y %= SCREEN_HEIGHT*2;
    if(x < 0) x += SCREEN_WIDTH*2;
    if(y < 0) y += SCREEN_HEIGHT*2;
    if(x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;

    if(cCore.ram.zbuffer[y][x] > z) return;
    cCore.ram.zbuffer[y][x] = z;
    cCore.ram.screen[y][x] = color;
    
}

void drawTile(int x, int y, tile t, bool transparency, s8 z)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(t[j][i] == cCore.ram.transparent && transparency) continue;
            
            pixelToScreen(x*8+i,y*8+j, t[j][i], z);
        }
    }
}

//draw tile with screen position
void drawTileSP(int x, int y, tile t, bool transparency, s8 z)
{
    

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(t[j][i] == cCore.ram.transparent && transparency) continue;

            pixelToScreen(x+i,y+j, t[j][i], z);
        }
    }
}

void drawBackground()
{
    for(int y = 0; y < 60; y++)
    {
        for(int x = 0; x < 64; x++)
        {
            bool transp = true;
            if(cCore.ram.bgMap[y][x] == 0) transp = false;
            drawTile(x, y, cCore.ram.bgTilesMem[cCore.ram.bgMap[y][x]], transp, 0);
        }
    }
    printS(0, 0, 7, "Hello World!");


}

void drawSprites()
{
    for(int i = 0; i < 64; i++)
    {
        sprite s = cCore.ram.sprites[i];
        drawTileSP(s.x, s.y, cCore.ram.spritesTileMem[s.tileIndex],true,10);
    }
}

void drawFilled(int x, int y, u8 color)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            pixelToScreen(x*8+i, y*8+j, color,5);
        }
    }
}


void bgSet(u16 x, u16 y, u8 tileIndex)
{
    if(x >= 64 || y >= 60) return;
    cCore.ram.bgMap[y][x] = tileIndex;
}
u8 bgGet(u16 x, u16 y)
{
    if(x >= 64 || y >= 60) return -1;
    return cCore.ram.bgMap[y][x];
}
void bgTileLoad(u8 index, tile t)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            cCore.ram.bgTilesMem[index][j][i] = t[j][i];
        }
    }
}
void spriteTileLoad(u8 index, tile t)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            cCore.ram.spritesTileMem[index][j][i] = t[j][i];
        }
    }
}

void setScrollX(int x)
{
    cCore.ram.scrollX = x;
}
void setScrollY(int y)
{
    cCore.ram.scrollY = y;
}

void cls()
{
    for(int x = 0; x < 256; x++)
    {
        for(int y = 0; y < 240; y++)
        {
            cCore.ram.screen[y][x] = cCore.ram.transparent;
            cCore.ram.zbuffer[y][x] = -128;
        }
    }
}
void resetBgMap()
{
    for(int i = 0; i < 60; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            cCore.ram.bgMap[i][j] = 0;
        }
    }
}

void setSprite(u8 index, int x, int y, u8 tileIndex, bool flipH, bool flipV, bool priority)
{
    cCore.ram.sprites[index].x = x;
    cCore.ram.sprites[index].y = y;
    cCore.ram.sprites[index].tileIndex = tileIndex;
    cCore.ram.sprites[index].flipH = flipH;
    cCore.ram.sprites[index].flipV = flipV;
    cCore.ram.sprites[index].priority = priority;
}
void setSpriteX(u8 index, int x)
{
    cCore.ram.sprites[index].x = x;
}
void setSpriteY(u8 index, int y)
{
    cCore.ram.sprites[index].y = y;
}
void setSpriteTileIndex(u8 index, u8 tileIndex)
{
    cCore.ram.sprites[index].tileIndex = tileIndex;
}
void setSpriteFlipH(u8 index, bool flipH)
{
    cCore.ram.sprites[index].flipH = flipH;
}
void setSpriteFlipV(u8 index, bool flipV)
{
    cCore.ram.sprites[index].flipV = flipV;
}
void setSpritePriority(u8 index, bool priority)
{
    cCore.ram.sprites[index].priority = priority;
}

void setTransparent(u8 color)
{
    cCore.ram.transparent = color;
}

void corePPUDraw()
{

    drawBackground();
    drawSprites();
    printZbuffer();
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
    for(int color = 0; color < 16; color++)
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                cCore.ram.bgTilesMem[color][i][j] = color;
            }
        }
    }
}


void initScreen()
{

    cCore.ram.scrollX = 0;
    cCore.ram.scrollY = 0;
    cCore.ram.transparent = 0;
    cls();
    resetBgMap();
    for(int x = 0; x < 256; x++)
    {
        for(int y = 0; y < 240; y++)
        {
            cCore.ram.screen[y][x] = cCore.ram.transparent;
        }
    }
    
    //drawTile(64, 0, prova);
    //setup sprites
    for(int i = 0; i < 64; i++)
    {
        cCore.ram.sprites[i].x = -8;
        cCore.ram.sprites[i].y = -8;
        cCore.ram.sprites[i].tileIndex = 0;
        cCore.ram.sprites[i].flipH = false;
        cCore.ram.sprites[i].flipV = false;
        cCore.ram.sprites[i].priority = false;
    }
    //setup bgmem

    for(int i = 0; i < 256; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                cCore.ram.bgTilesMem[i][j][k] = cCore.ram.transparent;
            }
        }
    }
    /*
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            bgTilesMem[1][i][j] = prova[i][j];
        }
    }
    populateBG();*/

}

void saveTiles()
{
    SaveFileData("tiles.bin", cCore.ram.bgTilesMem, 256*8*8);
}
void saveSprites()
{
    SaveFileData("sprites.bin", cCore.ram.spritesTileMem, 256*8*8);
}

void saveTilesData(tile t[256], cstring filename)
{
    SaveFileData(filename, t, 256*8*8);
}

cstring loadTilesData(cstring filename)
{
    int fileSize;
    cstring buffer = LoadFileData(filename, &fileSize);
    string out = malloc(fileSize);
    for(int i = 0; i < fileSize; i++)
    {
        out[i] = buffer[i];
    }
    UnloadFileData((unsigned char *)buffer);
    return out;
}

void loadTiles()
{
    cstring buffer = loadTilesData("tiles.bin");
    for(int i = 0; i < 256; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                cCore.ram.bgTilesMem[i][j][k] = buffer[i*8*8+j*8+k];
            }
        }
    }
    free((void *)buffer);
}

void printC(int x, int y, char c, u8 color)
{
    for(int y1 = 0; y1 < 8; y1++)
    {
        for(int x1 = 0; x1 < 8; x1++)
        {
            if(font[(int)c*8+y1] & (1 << x1))
            {
                pixelToScreen(x+x1, y+y1, color, 20);
            }
        }
    }
}
void printS(int x, int y, u8 color,cstring s)
{
    int i = 0;
    while(s[i] != '\0')
    {
        printC(x+i*6, y, s[i], color);
        i++;
    }
}


