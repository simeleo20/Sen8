#include "types/types.h"
#include "core.h"
#include "lua.h"
#include <stdlib.h>
//#include "oslua.c"
#include "raylib.h"
#include "os.h"
#include <stdio.h>
#include "./editor/scriptEditor.h"

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

//current core
core cCore = {0};
//size 127*8
char font[] = {
#include "font.inl"
};
void printTile(tile t)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d ", t[i][j]);
        }
        printf("\n");
    }
}


void coreVBLANK()
{
    cls();
    if(cCore.running == false) return;
    if(cCore.vblank != NULL)
        cCore.vblank(&cCore);
}
void coreLoop()
{
    if(cCore.running == false) return;
    if(cCore.loop != NULL)
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

    /*
    free(cCore.ram.script);
    cCore.ram.script = fileToString("editor.lua");
    loadTiles();
    */
    initLua(&cCore);
    
    

    
    if(cCore.setup != NULL)
        cCore.setup(&cCore);
    
}
void coreClose()
{

    if(cCore.close != NULL)
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

void drawTile(int x, int y, tile *t, bool transparency, s8 z)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if((*t)[j][i] == cCore.ram.transparent && transparency) continue;
            
            pixelToScreen(x*8+i,y*8+j, (*t)[j][i], z);
        }
    }
}

//draw tile with screen position
void drawTileSP(int x, int y, tile *t, bool transparency, s8 z)
{
    

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if((*t)[j][i] == cCore.ram.transparent && transparency) continue;

            pixelToScreen(x+i,y+j, (*t)[j][i], z);
        }
    }
}
//draw tile with screen position with flips
void drawTileSPFlips(int x, int y, tile *t, bool transparency, s8 z, bool flipH, bool flipV)
{
    //printf("%d %d\n",flipH,flipV);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            int srcX = (flipH==true) ? 7 - i : i;
            int srcY = (flipV==true) ? 7 - j : j;
            if((*t)[srcY][srcX] == cCore.ram.transparent && transparency) continue;

            pixelToScreen(x + i, y + j, (*t)[srcY][srcX], z);
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
            if(cCore.ram.bgMap[y][x] == 0) continue;;
            drawTile(x, y, &cCore.ram.bgTilesMem[cCore.ram.bgMap[y][x]], transp, 5);
        }
    }
}

void drawSprites()
{
    for(int i = 0; i < 64; i++)
    {
        sprite s = cCore.ram.sprites[i];
        s8 z = 0;
        if(s.priority)
        {
            z=10;
        }
        else
        {
            z=1;
        }
        drawTileSPFlips(s.x, s.y, &cCore.ram.spritesTileMem[s.tileIndex],true,z,s.flipH,s.flipV);
    }
}

void drawFilled(int x, int y, u8 color, u8 z)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            pixelToScreen(x*8+i, y*8+j, color,z);
        }
    }
}
void drawRectFilled(int x, int y, int w, int h, u8 color, u8 z)
{
    for(int y1 = 0; y1 < h; y1++)
    {
        for(int x1 = 0; x1 < w; x1++)
        {
            pixelToScreen(x+x1, y+y1, color,z);
        }
    }
}
void drawLine(int x1, int y1, int x2, int y2, u8 color, u8 z)
{
    int dx = abs(x2-x1);
    int dy = abs(y2-y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx-dy;
    int e2;
    while(true)
    {
        pixelToScreen(x1, y1, color, z);
        if(x1 == x2 && y1 == y2) break;
        e2 = 2*err;
        if(e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if(e2 < dx)
        {
            err += dx;
            y1 += sy;
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
    if(cCore.running == false) return;
    drawBackground();
    drawSprites();
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

void createVM()
{
    if(cCore.ram.language == LUA)
    {
        initLua(&cCore);
    }
}

void startRunning()
{
    loadScriptToRam();
    extern int currentPage;
    if(cCore.resetVM == NULL)
    {
        createVM(&cCore);
    }
    else {
        cCore.resetVM(&cCore);
    }
    currentPage = RUNTIME;
    cCore.running = true;
    resetScreen();
    coreSetup();
}

void stopRunning()
{
    resetScreen();
    extern int currentPage;
    currentPage = CONSOLE;
    cCore.running = false;
}

void resetScreen()
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

void printC(int x, int y, char c, u8 color, u8 z)
{
    for(int y1 = 0; y1 < 8; y1++)
    {
        for(int x1 = 0; x1 < 8; x1++)
        {
            if(font[(int)c*8+y1] & (1 << x1))
            {
                pixelToScreen(x+x1, y+y1, color, z);
            }
        }
    }
}
void printS(int x, int y, u8 color,cstring s)
{
    int i = 0;
    while(s[i] != '\0')
    {
        printC(x+i*6, y, s[i], color, 20);
        i++;
    }
}

int loadTilesFromString(cstring str, tile *tiles, cstring startStr, cstring endStr)
{
    cstring cursor = strstr(str, startStr);
    if(cursor == NULL) return 0;
    cursor = strchr(cursor, '\n')+1;
    cstring end = strstr(cursor, endStr);
    if(end == NULL) return 0;
    cstring line = cursor;
    while(cursor < end)
    {
        cstring lineEnd = strchr(cursor, '\n');
        if(lineEnd >= end) break;
        int lineSize = lineEnd - cursor;
        string line = malloc(lineSize+1);
        memcpy(line, cursor, lineSize);
        line[lineSize] = '\0';
        int i=0;
        printf("line: %s\n",line);
        if(strncmp(line, "--", 2) == 0)
        {
            line[0] = ' ';
            line[1] = ' ';
            sscanf(line, "%03d:",&i);
            sscanf(line, "%03d: %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x",
                &i,
                (unsigned int *)&tiles[i][0][0], (unsigned int *)&tiles[i][0][1], (unsigned int *)&tiles[i][0][2], (unsigned int *)&tiles[i][0][3], (unsigned int *)&tiles[i][0][4], (unsigned int *)&tiles[i][0][5], (unsigned int *)&tiles[i][0][6], (unsigned int *)&tiles[i][0][7],
                (unsigned int *)&tiles[i][1][0], (unsigned int *)&tiles[i][1][1], (unsigned int *)&tiles[i][1][2], (unsigned int *)&tiles[i][1][3], (unsigned int *)&tiles[i][1][4], (unsigned int *)&tiles[i][1][5], (unsigned int *)&tiles[i][1][6], (unsigned int *)&tiles[i][1][7],
                (unsigned int *)&tiles[i][2][0], (unsigned int *)&tiles[i][2][1], (unsigned int *)&tiles[i][2][2], (unsigned int *)&tiles[i][2][3], (unsigned int *)&tiles[i][2][4], (unsigned int *)&tiles[i][2][5], (unsigned int *)&tiles[i][2][6], (unsigned int *)&tiles[i][2][7],
                (unsigned int *)&tiles[i][3][0], (unsigned int *)&tiles[i][3][1], (unsigned int *)&tiles[i][3][2], (unsigned int *)&tiles[i][3][3], (unsigned int *)&tiles[i][3][4], (unsigned int *)&tiles[i][3][5], (unsigned int *)&tiles[i][3][6], (unsigned int *)&tiles[i][3][7],
                (unsigned int *)&tiles[i][4][0], (unsigned int *)&tiles[i][4][1], (unsigned int *)&tiles[i][4][2], (unsigned int *)&tiles[i][4][3], (unsigned int *)&tiles[i][4][4], (unsigned int *)&tiles[i][4][5], (unsigned int *)&tiles[i][4][6], (unsigned int *)&tiles[i][4][7],
                (unsigned int *)&tiles[i][5][0], (unsigned int *)&tiles[i][5][1], (unsigned int *)&tiles[i][5][2], (unsigned int *)&tiles[i][5][3], (unsigned int *)&tiles[i][5][4], (unsigned int *)&tiles[i][5][5], (unsigned int *)&tiles[i][5][6], (unsigned int *)&tiles[i][5][7],
                (unsigned int *)&tiles[i][6][0], (unsigned int *)&tiles[i][6][1], (unsigned int *)&tiles[i][6][2], (unsigned int *)&tiles[i][6][3], (unsigned int *)&tiles[i][6][4], (unsigned int *)&tiles[i][6][5], (unsigned int *)&tiles[i][6][6], (unsigned int *)&tiles[i][6][7],
                (unsigned int *)&tiles[i][7][0], (unsigned int *)&tiles[i][7][1], (unsigned int *)&tiles[i][7][2], (unsigned int *)&tiles[i][7][3], (unsigned int *)&tiles[i][7][4], (unsigned int *)&tiles[i][7][5], (unsigned int *)&tiles[i][7][6], (unsigned int *)&tiles[i][7][7]
            );
            printTile(tiles[i]);
        }   
        free(line);
        
        cursor = lineEnd+1;
    }
    return 1;
}

int loadSenString(cstring fileChars)
{
    core newCore;
    cstring langDecl = strstr(fileChars, "--lang=");
    if(langDecl == NULL) return 0;
    langDecl += 7;
    if(strncmp(langDecl, "lua", 3) == 0)
    {
        newCore.ram.language = LUA;
    }
    else
    {
        return 0;
    }

    cstring codeStart;
    if(newCore.ram.language == LUA) codeStart= strstr(fileChars, "--<CODE>");
    codeStart = strchr(codeStart, '\n')+1;
    if(codeStart == NULL) return 0;

    cstring codeEnd;
    if(newCore.ram.language == LUA) codeEnd = strstr(codeStart, "--</CODE>");

    if(codeEnd == NULL) return 0;
    int codeSize = codeEnd - codeStart;
    coreClose();

    string code = malloc(codeSize+1);
    memcpy(code, codeStart, codeSize);
    code[codeSize] = '\0';
    newCore.ram.script = code;
    printf("%s\n", code);

    if(newCore.ram.language == LUA) loadTilesFromString(fileChars, newCore.ram.bgTilesMem, "--<TILES>", "--</TILES>");
    
    if(newCore.ram.language == LUA) loadTilesFromString(fileChars, newCore.ram.spritesTileMem, "--<SPRITES>", "--</SPRITES>");

    
    initLua(&newCore);
    newCore.running = false;
    cCore = newCore;

    return 1;

}

void setZeroRam(mem *ram)
{
    ram->transparent=0;
    ram->scrollX = 0;
    ram->scrollY = 0;
    ram->script = NULL;
    memset(ram->bgMap,0,sizeof(ram->bgMap));

}

int loadCart(cart *cartridge)
{
    
    core newCore;
    setZeroRam(&(newCore.ram));
    newCore.ram.language = cartridge->language;
    newCore.ram.script = cartridge->script;
    printf("tra %d\n",newCore.ram.transparent);
    




    
    memcpy(newCore.ram.bgTilesMem, cartridge->bgTilesMem, sizeof(newCore.ram.bgTilesMem));
    memcpy(newCore.ram.spritesTileMem, cartridge->spritesTileMem, sizeof(newCore.ram.spritesTileMem));

    initLua(&newCore);
    newCore.running = false;
    cCore = newCore;
    return 1;
}

cstring generateSenStr()
{
    char commentSimbol;
    if(cCore.ram.language == LUA) commentSimbol = '-';
    string tilesStr = malloc(256*8*8);
    tile *tiles = cCore.ram.bgTilesMem;

    for(int i = 0; i < 256; i++)
    {
        bool empty = true;
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                if(tiles[i][j][k] != 0)
                {
                    empty = false;
                    break;
                }
            }
            if(!empty) break;
        }
        for(int j = 0; j < 8; j++)
        {
            for(int k = 0; k < 8; k++)
            {
                
            }
        }
    }
    string sprites = malloc(256*8*8);
}