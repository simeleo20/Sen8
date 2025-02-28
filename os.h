#ifndef OS_H
#define OS_H

#include "types/types.h"

enum page
{
    CONSOLE,
    RUNTIME,
    EDITOR
};
//bool int key
typedef struct 
{
    bool (*checkKey1)(int);
    u32 key1;
    bool (*checkKey2)(int);
    u32 key2;
    bool (*checkKey3)(int);
    u32 key3;
    void (*callback)();
}shortCut;


void osLoop();

void detectShortCut(shortCut *shortCuts, u32 size);

#endif // OS_H