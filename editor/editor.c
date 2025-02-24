#include "editor.h"
#include <raylib.h>
#include "../graphics.h"
#include <stdio.h>

extern core cCore;

int selectedTile = 1;
int selectedColor = 6;
Vector2 mousePos;

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
void drawHorizontaFilledLine(int x1, int x2, int y, u8 color)
{
    for(int x = x1; x < x2; x++)
    {
        drawFilled(x, y, color);
    }
}
void drawSelectedTile(u8 x, u8 y)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            drawFilled(x+j, y+i, cCore.ram.bgTilesMem[selectedTile][i][j]);
        }
    }
    drawRect(x*8-1, y*8-1, (x+8)*8, (y+8)*8, _WHITE, 20);
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
            drawFilled(x+j, y+i, counter);
            counter++;
        }
    }
    drawRect(x*8-1, y*8-1, (x+8)*8, (y+2)*8, _WHITE, 20);

}
void drawTileSelector(u8 x, u8 y)
{
    int counter =0;
    for(int y1 = 0;y1<16;y1++)
    {
        for(int x1 = 0;x1<16;x1++)
        {
            drawTile(x+x1, y+y1, &cCore.ram.bgTilesMem[counter], false, 0);
            if(counter == selectedTile)
            {
                drawRect((x+x1)*8-1, (y+y1)*8-1, (x+x1)*8+8, (y+y1)*8+8, _WHITE, 20);
            }
            counter++;
        }
    }
    drawRect(x*8-1, y*8-1, (x+16)*8, (y+16)*8, _WHITE, 20);
    

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
        cCore.ram.bgTilesMem[selectedTile][mouseY/8-y ][mouseX/8-x] = selectedColor;
    }
}
void detectTileSelect(u8 x, u8 y)
{
    int mouseX = mousePos.x;
    int mouseY = mousePos.y;
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    if(mouseX >= x*8 && mouseX < (x+16)*8 && mouseY >= y*8 && mouseY < (y+16)*8)
    {
        if((mouseY/8-y)*16 + (mouseX/8-x)!=0)
        selectedTile = (mouseY/8-y)*16 + (mouseX/8-x);
    }
}



int editorLoop()
{
    mousePos = calcMousePosition();
    drawHorizontaFilledLine(0, 32, 0, 8);
    drawHorizontaFilledLine(0, 32, 1, 8);
    drawSelectedTile(1,3);
    detectDraw(1,3);
    drawColorPicker(1,12);
    detectColorSelect(1,12);
    drawTileSelector(13,3);
    detectTileSelect(13,3);
    
    //drawLine(0, 0, 256, 240, _RED, 0);
    return 1;
}


