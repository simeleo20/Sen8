#include "os.h"
#include <raylib.h>
#include "console.h"
#include "editor/editor.h"
#include "editor/scriptEditor.h"

extern core cCore;



int currentPage = EDITOR;

void osLoop()
{
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