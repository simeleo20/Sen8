#ifndef LUA_H
#define LUA_H

#include "types/types.h"
#include "core.h"

void initLua(core *cCore);
void closeLua(core *cCore);
void restartLua(core *cCore);
void execLuaVBLANK(core *cCore);
void execLuaLoop(core *cCore);
void execLuaSetup(core *cCore);


#endif // LUA_H