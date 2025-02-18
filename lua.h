#ifndef LUA_H
#define LUA_H

#include "types.h"

void initLua(string script);
void closeLua();
void execLuaVBLANK();
void execLuaLoop();
void execLuaSetup();

#endif // LUA_H