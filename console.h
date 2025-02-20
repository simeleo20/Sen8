#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"
#include "core.h"
#include <raylib.h>

typedef struct
{
#define maxConsoleLines 30
#define maxConsoleChars 32
    char lines[maxConsoleLines][maxConsoleChars + 1];
    u16 bottomPtr;
    u8 bottomXPtr;
} printMem;

typedef struct charNode
{
    char c;
    struct charNode *next;
} charNode;


void print(cstring str);

#endif // CONSOLE_H