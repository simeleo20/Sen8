#include "os.h"
#include <raylib.h>
#include "console.h"
#include "editor/editor.h"
#include "editor/scriptEditor.h"

extern core cCore;

shortCut osShortCuts[] ={
    {IsKeyDown,KEY_LEFT_CONTROL,IsKeyPressed, KEY_R, NULL,KEY_NULL,startRunning},
    {IsKeyPressed,KEY_F11,NULL,KEY_NULL,NULL,KEY_NULL,ToggleBorderlessWindowed}
};
#define osShortCutsSize 2

void detectShortCut(shortCut *shortCuts, u32 size)
{
    for(u32 i = 0; i<size;i++)
    {
        shortCut *current = &shortCuts[i];
        if(
            (current->checkKey1 ==NULL ||current->checkKey1(current->key1)) &&
            (current->checkKey2 ==NULL ||current->checkKey2(current->key2)) &&
            (current->checkKey3 ==NULL ||current->checkKey3(current->key3)) 
        )
        {
            current->callback();
        }
    }
}


int currentPage = EDITOR;

void osLoop()
{
    detectShortCut(osShortCuts,osShortCutsSize);
    if(currentPage == CONSOLE)
    {
        consoleLoop();
    }
    else if(currentPage == EDITOR)
    {
        editorLoop();
    }

    if(IsKeyPressed(KEY_ESCAPE))
    {
        if(cCore.running == true)
        {
            stopRunning();
            currentPage = CONSOLE;
        }
        else if(currentPage == CONSOLE)
        {
            currentPage = EDITOR;
        }
        else if(currentPage == EDITOR)
        {
            currentPage = CONSOLE;
            loadScriptToRam();
        }
    }

}