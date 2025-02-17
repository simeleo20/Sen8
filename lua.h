#ifndef LUA_H
#define LUA_H

void initLua(const char* filename);
void closeLua();
void execLuaVBLANK();
void execLuaLoop();
void execLuaSetup();

#endif // LUA_H