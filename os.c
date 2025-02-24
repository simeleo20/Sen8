#include "os.h"
#include <raylib.h>
#include "console.h"
#include "editor/editor.h"

extern core cCore;

enum page
{
    CONSOLE,
    RUNTIME,
    EDITOR
};

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
            cCore.running = false;
            currentPage = CONSOLE;
        }
        else if(currentPage == CONSOLE)
        {
            currentPage = EDITOR;
        }
        else if(currentPage == EDITOR)
        {
            currentPage = CONSOLE;
        }
    }

}