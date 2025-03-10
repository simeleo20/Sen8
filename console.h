#ifndef CONSOLE_H
#define CONSOLE_H

#include "types/types.h"
#include "core.h"
#include <raylib.h>

typedef struct
{
#define maxConsoleLines 60
#define maxConsoleChars 42
    char lines[maxConsoleLines][maxConsoleChars + 1];
    u16 bottomPtr;
    u8 bottomXPtr;
} printMem;

typedef struct charNode
{
    char c;
    struct charNode *next;
} charNode;

typedef struct
{
    charNode *head;
    int cursor;
} writing;

typedef struct 
{
    cstring keyWord;
    int (*exec)(cstring);
    cstring shortDescr;
}command;


void print(cstring str, ...)  __attribute__ ((format (printf, 1, 2)));
void vprint(cstring str,va_list args);
void consoleLoop();

int execHelp(cstring str);
int execLs(cstring str);
int execCd(cstring str);
int execMk(cstring str);
int execLoad(cstring str);
int execSave(cstring str);
int execRun(cstring str);
int execFolder(cstring str);
#if defined(PLATFORM_WEB)
int execDownload(cstring str);
#endif
#endif // CONSOLE_H