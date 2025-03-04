#include "editor.h"
#include "../graphics.h"
#include "stdio.h"
#include "stdlib.h"

int selectedTile = 1;
int selectedColor = 6;

extern core cCore;
extern Vector2 mousePos;

enum TILEPAGES{
    TILE_PAGE,
    SPRITE_PAGE
};

int tilePage = TILE_PAGE;
tile *currentTileMem;

void drawSelectedTile(u8 x, u8 y)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            drawFilled(x+j, y+i, currentTileMem[selectedTile][i][j], 5);
        }
    }
    drawRect(x*8-1, y*8-1, (x+8)*8, (y+8)*8, _BLACK, 10);
}
void drawColorPicker(u8 x, u8 y)
{
    u8 counter=0;
    for(int i = 0; i<2; i++)
    {
        for(int j = 0; j<8; j++)
        {
            if(counter == selectedColor)
            {
                drawRect(x*8+j*8, y*8+i*8, x*8+j*8+7, y*8+i*8+7, _BLACK, 20);
                drawRect(x*8+j*8-1, y*8+i*8-1, x*8+j*8+8, y*8+i*8+8, _WHITE, 20);

            }
            drawFilled(x+j, y+i, counter,5);
            counter++;
        }
    }
    drawRect(x*8-1, y*8-1, (x+8)*8, (y+2)*8, _BLACK, 10);

}
void drawTileSelector(u8 x, u8 y)
{
    int counter =0;
    for(int y1 = 0;y1<16;y1++)
    {
        for(int x1 = 0;x1<16;x1++)
        {
            drawTile(x+x1, y+y1, &(currentTileMem[counter]), false, 0);
            if(counter == selectedTile)
            {
                drawRect((x+x1)*8-1, (y+y1)*8-1, (x+x1)*8+8, (y+y1)*8+8, _WHITE, 20);
            }
            counter++;
        }
    }
    drawRect(x*8-1, y*8-1, (x+16)*8, (y+16)*8, _BLACK, 10);
    

}
void detectColorSelect(u8 x, u8 y)
{
    int mouseX = mousePos.x;
    int mouseY = mousePos.y;
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    if(mouseX >= x*8 && mouseX < (x+8)*8 && mouseY >= y*8 && mouseY < (y+2)*8)
    {
        selectedColor = (mouseY/8-y)*8 + (mouseX/8-x);
    }
}
void detectDraw(u8 x, u8 y)
{
    int mouseX = mousePos.x;
    int mouseY = mousePos.y;
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    if(mouseX >= x*8 && mouseX < (x+8)*8 && mouseY >= y*8 && mouseY < (y+8)*8)
    {
        currentTileMem[selectedTile][mouseY/8-y ][mouseX/8-x] = selectedColor;
    }
}
void detectTileSelect(u8 x, u8 y)
{
    int mouseX = mousePos.x;
    int mouseY = mousePos.y;
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    if(mouseX >= x*8 && mouseX < (x+16)*8 && mouseY >= y*8 && mouseY < (y+16)*8)
    {
        if((mouseY/8-y)*16 + (mouseX/8-x)!=0 || tilePage == SPRITE_PAGE)
        selectedTile = (mouseY/8-y)*16 + (mouseX/8-x);
    }
}
void printTileToC()///<<<<<<<<<<<<<-------------------------------da rimuovere
{
    printf("{\n");
    for(int i = 0; i < 8; i++)
    {
        printf("    {");
        for(int j = 0; j < 8; j++)
        {
            if(currentTileMem[selectedTile][i][j] != 0)
            {
                printf("c, " );
            } 
            else
            {
                printf("0, ");
            }
        }
        printf("},\n");
    }
    printf("},\n");

}
void drawSpriteButton(int x, int y)
{
    u8 c;
    if(tilePage == SPRITE_PAGE)
    {
        c = _WHITE;
    }
    else
    {
        c = _LAVENDER;
    }
    
    tile ul ={
        {c, c, c, c, c, c, c, c, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, 0, 0, 0, 0, c, c, c, },
        {c, 0, 0, 0, c, c, c, c, },
        {c, 0, 0, 0, c, c, c, c, },
    };
    tile ur = {
        {c, c, c, c, c, c, c, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {c, c, 0, 0, 0, 0, 0, c, },
        {c, c, c, c, c, 0, 0, c, },
        {c, c, c, 0, 0, 0, 0, c, },
    };
    tile dl = {
        {c, 0, 0, c, c, c, c, c, },
        {c, 0, 0, c, c, c, c, c, },
        {c, 0, 0, c, c, c, c, c, },
        {c, 0, 0, 0, 0, c, c, c, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, c, c, c, c, c, c, c, },
    };
    tile dr = {
        {c, c, c, c, c, 0, 0, c, },
        {c, c, c, c, c, c, 0, c, },
        {c, c, c, c, c, 0, 0, c, },
        {c, c, c, c, 0, 0, 0, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {c, c, c, c, c, c, c, c, },
    };
    drawTileSP(x, y, &ul,true,35);
    drawTileSP(x+8, y, &ur,true,35);
    drawTileSP(x, y+8, &dl,true,35);
    drawTileSP(x+8, y+8, &dr,true,35);
}
void drawTileButton(int x, int y)
{
    u8 c;
    if(tilePage == TILE_PAGE)
    {
        c = _WHITE;
    }
    else
    {
        c = _LAVENDER;
    }
    
    tile ul ={
        {c, c, c, c, c, c, c, c, },
        {c, 0, 0, 0, 0, 0, 0, c, },
        {c, 0, 0, 0, 0, 0, 0, c, },
        {c, 0, 0, 0, 0, 0, 0, c, },
        {c, 0, 0, 0, 0, 0, 0, c, },
        {c, c, c, c, c, c, c, c, },
        {c, 0, 0, 0, c, 0, 0, 0, },
        {c, 0, 0, 0, c, 0, 0, 0, },
    };
    tile ur = {
        {c, c, c, c, c, c, c, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {0, 0, 0, 0, 0, 0, 0, c, },
        {c, c, c, c, c, c, c, c, },
        {0, 0, 0, 0, c, 0, 0, c, },
        {0, 0, 0, 0, c, 0, 0, c, },
    };
    tile dl = {
        {c, 0, 0, 0, c, 0, 0, 0, },
        {c, 0, 0, 0, c, 0, 0, 0, },
        {c, c, c, c, c, c, c, c, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, 0, 0, 0, 0, 0, 0, 0, },
        {c, c, c, c, c, c, c, c, },
    };
    tile dr = {
        {0, 0, 0, 0, c, 0, 0, c, },
        {0, 0, 0, 0, c, 0, 0, c, },
        {c, c, c, c, c, c, c, c, },
        {c, 0, 0, 0, 0, 0, 0, c, },
        {c, 0, 0, 0, 0, 0, 0, c, },
        {c, 0, 0, 0, 0, 0, 0, c, },
        {c, 0, 0, 0, 0, 0, 0, c, },
        {c, c, c, c, c, c, c, c, },
    };
    drawTileSP(x, y, &ul,true,35);
    drawTileSP(x+8, y, &ur,true,35);
    drawTileSP(x, y+8, &dl,true,35);
    drawTileSP(x+8, y+8, &dr,true,35);
}
void changeToSpritePage()
{
    tilePage = SPRITE_PAGE;
    currentTileMem = cCore.ram.spritesTileMem;
}
void changeToTilePage()
{
    tilePage = TILE_PAGE;
    currentTileMem = cCore.ram.bgTilesMem;
    if(selectedTile == 0) selectedTile =1;
}
void checkCurrentTilePage()
{
    if(tilePage == TILE_PAGE)
    {
        currentTileMem = cCore.ram.bgTilesMem;
    }
    else
    {
        currentTileMem = cCore.ram.spritesTileMem;
    }
}
void drawTileSelectedNumber(int x,int y)
{
    string out = malloc(4);
    sprintf(out,"%03d",selectedTile);
    drawRectFilled(x,y,6*3+1,7,_LIGHT_GREY,0);
    printS(x+1,y+1,_LAVENDER,out);
    free(out);
}


void tileEditorLoop()
{
    for(int i = 2; i < 29; i++)
    {
        drawHorizontaFilledLine(0, 32, i, _DARK_GREY,0);
    }
    button(15*8-1,19*8+1,16,16,drawSpriteButton,changeToSpritePage);
    button(13*8-1,19*8+1,16,16,drawTileButton,changeToTilePage);
    button(27*8-2,19*8+2,8,8,drawTileSelectedNumber, NULL);
    checkCurrentTilePage();
    drawSelectedTile(1,3);
    detectDraw(1,3);
    drawColorPicker(1,12);
    detectColorSelect(1,12);
    drawTileSelector(13,3);
    detectTileSelect(13,3);
    if(IsKeyPressed(KEY_P))
    printTileToC();
}