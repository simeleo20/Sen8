#ifndef CORE_H
#define CORE_H

#include "types/types.h"
#include <lua.h>

//buttons
#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_LEFT 3
#define BUTTON_A 4
#define BUTTON_B 5
#define BUTTON_MOUSEL 6
#define BUTTON_MOUSER 7

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240

// RGB color structure
typedef struct rgb
{
    u8 r;
    u8 g;
    u8 b;
} rgb;

#define TILE_SIZE 64
// 8x8 tile 
typedef u8 tile[8][8];

#define SPRITE_SIZE 4+4+1+1+1+1
typedef struct sprite
{
    int x;
    int y;
    u8 tileIndex;
    bool flipH;
    bool flipV;
    bool priority;
} sprite;
typedef enum color {
    _BLACK = 0,
    _DARK_BLUE,
    _DARK_PURPLE,
    _DARK_GREEN,
    _BROWN,
    _DARK_GREY,
    _LIGHT_GREY,
    _WHITE,
    _RED,
    _ORANGE,
    _YELLOW,
    _GREEN,
    _BLUE,
    _LAVENDER,
    _PINK,
    _LIGHT_PEACH
} color;




#define PTR_SIZE 8
#define MEM_SIZE (256+256)*TILE_SIZE+(60*64)+(2*4)+(SPRITE_SIZE*64)+(SCREEN_HEIGHT*SCREEN_WIDTH*2)+1+1+PTR_SIZE+1+1

typedef struct {
    union 
    {
        struct 
        {
            tile spritesTileMem[256];
            tile bgTilesMem[256];
            u8 bgMap[60][64];
            int scrollX;
            int scrollY;
            sprite sprites[64];
            u8 screen[SCREEN_HEIGHT][SCREEN_WIDTH];
            s8 zbuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
            string script;
            u8 language;
            u8 transparent;
        };
        u8 data[MEM_SIZE];
        
    };
} mem;
typedef struct {
    tile spritesTileMem[256];
    tile bgTilesMem[256];
    u8 language;
    cstring script;
} cart;

//vm
typedef struct vm {
    union {
        lua_State *L;
        // Add other VM types here in the future
    };
} vm;

// ram
//
typedef struct {
    mem ram;
    vm vm;
    bool running;
    void (*vblank)();
    void (*loop)();
    void (*setup)();
    void (*close)();

} core;

typedef enum languageE { LUA, PYTHON, JS } languageE;

void initScreen();
void corePPUDraw();


void bgSet(u16 x, u16 y, u8 tileIndex);
u8 bgGet(u16 x, u16 y);
void bgTileLoad(u8 index, tile t);
void spriteTileLoad(u8 index, tile t);
void setScrollX(int x);
void setScrollY(int y);
void drawTile(int x, int y, tile *t, bool transparency, s8 z);
void drawTileSP(int x, int y, tile *t, bool transparency, s8 z);
void drawFilled(int x, int y, u8 color, u8 z);
void drawRectFilled(int x, int y, int w, int h, u8 color, u8 z);
void drawLine(int x1, int y1, int x2, int y2, u8 color, u8 z);
void setTransparent(u8 color);
void setSprite(u8 index, int x, int y, u8 tileIndex, bool flipH, bool flipV, bool priority);
void setSpriteX(u8 index, int x);
void setSpriteY(u8 index, int y);
void setSpriteTileIndex(u8 index, u8 tileIndex);
void saveTiles();
void saveSprites();

void coreVBLANK();
void coreLoop();
void coreSetup();
void coreClose();
void cls();
cstring loadTilesData(cstring filename);
void loadTiles();
int loadSenString(cstring fileChars);
int loadCart(cart *cartridge);
void printS(int x, int y, u8 color,cstring s);
void printC(int x, int y, char c, u8 color, u8 z);

#endif // CORE_H