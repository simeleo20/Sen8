#include "editor.h"
#include "../graphics.h"
#include "stdio.h"

int selectedTile = 1;
int selectedColor = 6;

extern core cCore;
extern Vector2 mousePos;

void drawSelectedTile(u8 x, u8 y)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            drawFilled(x+j, y+i, cCore.ram.bgTilesMem[selectedTile][i][j],5);
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
            drawFilled(x+j, y+i, counter,5);
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
void printTileToC()///<<<<<<<<<<<<<-------------------------------da rimuovere
{
    printf("{\n");
    for(int i = 0; i < 8; i++)
    {
        printf("    {");
        for(int j = 0; j < 8; j++)
        {
            if(cCore.ram.bgTilesMem[selectedTile][i][j] != 0)
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

void tileEditorLoop()
{
    drawSelectedTile(1,3);
    detectDraw(1,3);
    drawColorPicker(1,12);
    detectColorSelect(1,12);
    drawTileSelector(13,3);
    detectTileSelect(13,3);
    if(IsKeyPressed(KEY_P))
    printTileToC();
}