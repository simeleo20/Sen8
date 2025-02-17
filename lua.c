#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include "lua.h"
#include "core.h"
#include "graphics.h"

lua_State *L;


int luaBgSet(lua_State *L)
{
    u16 x = lua_tonumber(L, -3);
    u16 y = lua_tonumber(L, -2);
    u8 tileIndex = lua_tonumber(L, -1);
    bgSet(x, y, tileIndex);
    return 0; // number of results
}
int luaBgGet(lua_State *L)
{
    u16 x = lua_tonumber(L, -2);
    u16 y = lua_tonumber(L, -1);
    lua_pushnumber(L, bgGet(x, y));
    return 1; // number of results
}

int luaGetMouseX(lua_State *L)
{
    lua_pushnumber(L, (calcMousePosition().x));
    return 1; // number of results
}
int luaGetMouseY(lua_State *L)
{
    lua_pushnumber(L, (calcMousePosition().y));
    return 1; // number of results
}

int luaBtn(lua_State *L)
{
    u8 btncode = lua_tonumber(L, -1);
    lua_pushnumber(L, btn(btncode));
    return 1; // number of results
}
int luaSetScrollX(lua_State *L)
{
    int x = lua_tonumber(L, -1);
    setScrollX(x);
    return 0; // number of results
}
int luaSetScrollY(lua_State *L)
{
    int y = lua_tonumber(L, -1);
    setScrollY(y);
    return 0; // number of results
}

void registerFunctions()
{
    lua_register(L, "bgSet", luaBgSet);
    lua_register(L, "bgGet", luaBgGet);
    lua_register(L, "getMouseX", luaGetMouseX);
    lua_register(L, "getMouseY", luaGetMouseY);
    lua_register(L, "btn", luaBtn);
    lua_register(L, "setScrollX", luaSetScrollX);
    lua_register(L, "setScrollY", luaSetScrollY);
}


void initLua(const char* filename)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    registerFunctions();
    if (luaL_dofile(L, filename) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

}
void closeLua()
{
    lua_close(L);
}


void execLuaVBLANK()
{
    lua_getglobal(L, "Vblank");
    if(lua_isfunction(L, -1))
        lua_call(L, 0, 0);
}
void execLuaLoop()
{
    lua_getglobal(L, "loop");
    if(lua_isfunction(L, -1))
        lua_call(L, 0, 0);
}
void execLuaSetup()
{
    lua_getglobal(L, "setup");
    if(lua_isfunction(L, -1))
        lua_call(L, 0, 0);
}

