#include "editor.h"
#include <raylib.h>
#include "../graphics.h"
#include <stdio.h>
#include "tileEditor.h"
#include "scriptEditor.h"
#include "../os.h"
extern core cCore;

Vector2 mousePos;

shortCut editorShortCuts[] = {
};
#define editorShortCutsSize 0



int editorPage = SCRIPT_EDITOR;


void button(int x, int y, u8 w, u8 h, void (*draw)(int,int), void (*callback)(void))
{
    draw(x,y);
    if(mousePos.x >= x && mousePos.x < x+w && mousePos.y >= y && mousePos.y < y+h)
    {
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            callback();
        }
    }
}

void drawRect(int x1, int y1, int x2, int y2, u8 color, u8 z)
{
    drawLine(x1, y1, x2, y1, color, z);
    drawLine(x1, y1, x1, y2, color, z);
    drawLine(x2, y1, x2, y2, color, z);
    drawLine(x1, y2, x2, y2, color, z);
}
void drawHorizontaFilledLine(int x1, int x2, int y, u8 color, u8 z)
{
    for(int x = x1; x < x2; x++)
    {
        drawFilled(x, y, color,z);
    }
}

void drawScriptIcon(int x, int y)
{

    u8 c;
    if(editorPage == SCRIPT_EDITOR)
    {
        c = _LIGHT_PEACH;
    }
    else
    {
        c = _DARK_PURPLE;
    }
    tile ul = {
        {0, 0, 0, 0, 0, 0, 0, 0, },
        {0, 0, 0, 0, 0, 0, 0, 0, },
        {0, 0, 0, c, c, c, c, 0, },
        {0, 0, 0, c, c, c, c, 0, },
        {0, c, c, c, c, 0, 0, 0, },
        {0, c, c, c, c, 0, 0, 0, },
        {0, c, c, c, c, 0, 0, 0, },
        {0, c, c, c, c, 0, 0, 0, }
    };
    tile ur = {
        {0, 0, 0, 0, 0, 0, 0, 0, },
        {0, 0, 0, 0, 0, 0, 0, 0, },
        {0, c, c, c, c, 0, 0, 0, },
        {0, c, c, c, c, 0, 0, 0, },
        {0, 0, 0, c, c, c, c, 0, },
        {0, 0, 0, c, c, c, c, 0, },
        {0, 0, 0, c, c, c, c, 0, },
        {0, 0, 0, c, c, c, c, 0, }
    };
    tile dl = {
        {0, c, c, c, c, 0, 0, 0, },
        {0, c, c, c, c, 0, 0, 0, },
        {0, c, c, c, c, 0, 0, 0, },
        {0, c, c, c, c, 0, 0, 0, },
        {0, 0, 0, c, c, c, c, 0, },
        {0, 0, 0, c, c, c, c, 0, },
        {0, 0, 0, 0, 0, 0, 0, 0, },
        {0, 0, 0, 0, 0, 0, 0, 0, },
    };
    tile dr = {
        {0, 0, 0, c, c, c, c, 0, },
        {0, 0, 0, c, c, c, c, 0, },
        {0, 0, 0, c, c, c, c, 0, },
        {0, 0, 0, c, c, c, c, 0, },
        {0, c, c, c, c, 0, 0, 0, },
        {0, c, c, c, c, 0, 0, 0, },
        {0, 0, 0, 0, 0, 0, 0, 0, },
        {0, 0, 0, 0, 0, 0, 0, 0, },
    };
    drawTileSP(x, y, &ul,true,35);
    drawTileSP(x+8, y, &ur,true,35);
    drawTileSP(x, y+8, &dl,true,35);
    drawTileSP(x+8, y+8, &dr,true,35);
}
void changeToScriptEditor()
{
    editorPage = SCRIPT_EDITOR;
}
void drawTileIcon(int x, int y)
{
    u8 c;
    if(editorPage == TILE_EDITOR)
    {
        c = _LIGHT_PEACH;
    }
    else
    {
        c = _DARK_PURPLE;
    }
    
    tile ul ={
        {0, 0, 0, 0, 0, 0, 0, 0, },
        {0, c, c, c, c, c, c, c, },
        {0, c, 0, 0, 0, 0, 0, 0, },
        {0, c, 0, 0, 0, 0, 0, 0, },
        {0, c, c, c, c, c, c, c, },
        {0, c, 0, 0, c, 0, 0, 0, },
        {0, c, 0, 0, c, 0, 0, 0, },
        {0, c, c, c, c, c, c, c, },
    };
    tile ur = {
        {0, 0, 0, 0, 0, 0, 0, 0, },
        {c, c, c, c, c, c, c, 0, },
        {c, 0, 0, 0, 0, 0, c, 0, },
        {c, 0, 0, 0, 0, 0, c, 0, },
        {c, c, c, c, c, c, c, 0, },
        {0, 0, c, 0, 0, 0, c, 0, },
        {0, 0, c, 0, 0, 0, c, 0, },
        {c, c, c, c, c, c, c, 0, },
    };
    tile dl = {
        {0, c, 0, 0, 0, 0, 0, c, },
        {0, c, 0, 0, 0, 0, 0, c, },
        {0, c, c, c, c, c, c, c, },
        {0, c, 0, 0, 0, c, 0, 0, },
        {0, c, 0, 0, 0, c, 0, 0, },
        {0, c, c, c, c, c, c, c, },
        {0, 0, 0, 0, 0, 0, 0, 0, },
        {0, 0, 0, 0, 0, 0, 0, 0, },
    };
    tile dr = {
        {0, 0, 0, 0, 0, 0, c, 0, },
        {0, 0, 0, 0, 0, 0, c, 0, },
        {c, c, c, c, c, c, c, 0, },
        {0, 0, 0, c, 0, 0, c, 0, },
        {0, 0, 0, c, 0, 0, c, 0, },
        {c, c, c, c, c, c, c, 0, },
        {0, 0, 0, 0, 0, 0, 0, 0, },
        {0, 0, 0, 0, 0, 0, 0, 0, },
    };
    drawTileSP(x, y, &ul,true,35);
    drawTileSP(x+8, y, &ur,true,35);
    drawTileSP(x, y+8, &dl,true,35);
    drawTileSP(x+8, y+8, &dr,true,35);
}

void changeToTileEditor()
{
    editorPage = TILE_EDITOR;
}
void setTilePage()
{
    editorPage = TILE_EDITOR;
}
void setScriptPage()
{
    editorPage = SCRIPT_EDITOR;
}


int editorLoop()
{
    mousePos = calcMousePosition();
    drawHorizontaFilledLine(0, 32, 0, 8,30);
    drawHorizontaFilledLine(0, 32, 1, 8,30);

    drawHorizontaFilledLine(0, 32, 29, 8,30);


    button(0, 0, 16,16, drawScriptIcon, changeToScriptEditor);
    button(16, 0, 16,16, drawTileIcon, changeToTileEditor);
    detectShortCut(editorShortCuts,editorShortCutsSize);
    if(editorPage == TILE_EDITOR)
    {
        tileEditorLoop();
    }
    else if(editorPage == SCRIPT_EDITOR)
    {
        scriptEditorLoop();
    }
    
    //drawLine(0, 0, 256, 240, _RED, 0);
    return 1;
}


