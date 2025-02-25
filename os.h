#ifndef OS_H
#define OS_H

#include "types/types.h"

enum page
{
    CONSOLE,
    RUNTIME,
    EDITOR
};

void osLoop();

#endif // OS_H