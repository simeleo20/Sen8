#ifndef OS_H
#define OS_H

#include "types/types.h"

enum page
{
    CONSOLE,
    RUNTIME,
    EDITOR
};
// checkKey function to check the key || use NULL to dont use the key
// key the key to check
// callback to call if all key are verified;
//
// bool (*checkKey1)(int);
// u32 key1;
// bool (*checkKey2)(int);
// u32 key2;
// bool (*checkKey3)(int);
// u32 key3;
// void (*callback)();
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