#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include "lua.h"
#include "core.h"
#include "graphics.h"
#include "console.h"






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
int luaGetKeyPressed(lua_State *L)
{
    lua_pushnumber(L, getKeyPressed());
    return 1; // number of results
}
int luaGetCharPressed(lua_State *L)
{
    lua_pushnumber(L, getCharPressed());
    return 1; // number of results
}
void printStack(lua_State *L) {
    int top = lua_gettop(L);
    printf("Total elements in the stack: %d\n", top);
    for (int i = 1; i <= top; i++) {
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:
                printf("%d String: %s\n",i ,lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                printf("%d Boolean: %s\n",i, lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                printf("%d Number: %g\n",i, lua_tonumber(L, i));
                break;
            default:
                printf("%d Other: %s\n",i, lua_typename(L, t));
                break;
        }
    }
}
int luaBgTileLoad(lua_State *L)
{
    u8 index = lua_tonumber(L, -2);
    tile out;
    lua_pushnil(L);  // first key
    int y = 0;
    while (lua_next(L, -2) != 0) {
        // key is at -2 and value is at -1
        lua_pushnil(L);  // first key
        while (lua_next(L, -2) != 0) {
            // key is at -2 and value is at -1
            u8 key = lua_tonumber(L, -2);
            u8 value = lua_tonumber(L, -1);
            out[y][key-1] = value;
            lua_pop(L, 1);
        }
        y++;
        lua_pop(L, 1);
    }

    bgTileLoad(index, out);

    return 0; // number of results

}
int luaSpriteTileLoad(lua_State *L)
{
    u8 index = lua_tonumber(L, -2);
    tile out;
    lua_pushnil(L);  // first key
    int y = 0;
    while (lua_next(L, -2) != 0) {
        // key is at -2 and value is at -1
        lua_pushnil(L);  // first key
        while (lua_next(L, -2) != 0) {
            // key is at -2 and value is at -1
            u8 key = lua_tonumber(L, -2);
            u8 value = lua_tonumber(L, -1);
            out[y][key-1] = value;
            lua_pop(L, 1);
        }
        y++;
        lua_pop(L, 1);
    }

    spriteTileLoad(index, out);

    return 0; // number of results
}
int luaDrawFilled(lua_State *L)
{
    int x = lua_tonumber(L, -3);
    int y = lua_tonumber(L, -2);
    u8 color = lua_tonumber(L, -1);
    drawFilled(x, y, color,5);
    return 0; // number of results
}
int LuaSetTransparent(lua_State *L)
{
    u8 color = lua_tonumber(L, -1);
    setTransparent(color);
    return 0;
}
int LuaSetSprite(lua_State *L)
{
    u8 index = lua_tonumber(L, -7);
    int x = lua_tonumber(L, -6);
    int y = lua_tonumber(L, -5);
    u8 tileIndex = lua_tonumber(L, -4);
    bool flipH = lua_toboolean(L, -3);
    bool flipV = lua_toboolean(L, -2);
    bool priority = lua_toboolean(L, -1);
    setSprite(index, x, y, tileIndex, flipH, flipV, priority);
    return 0;
}
int LuaSetSpriteX(lua_State *L)
{
    u8 index = lua_tonumber(L, -2);
    int x = lua_tonumber(L, -1);
    setSpriteX(index, x);
    return 0;
}

int LuaSetSpriteY(lua_State *L)
{
    u8 index = lua_tonumber(L, -2);
    int y = lua_tonumber(L, -1);
    setSpriteY(index, y);
    return 0;
}

int LuaSetSpriteTileIndex(lua_State *L)
{
    u8 index = lua_tonumber(L, -2);
    u8 tileIndex = lua_tonumber(L, -1);
    setSpriteTileIndex(index, tileIndex);
    return 0;
}
int luaSaveTiles(lua_State *L)
{
    saveTiles();
    return 0;
}
int luaSaveSprites(lua_State *L)
{
    saveSprites();
    return 0;
}
int luaLoadTilesData(lua_State *L)
{
    cstring filename = lua_tostring(L, -1);
    const char* data = (const char*)loadTilesData(filename);
    lua_newtable(L);
    int i = 0;
    for (int i = 0; i < 256 * 8 * 8; i++) {
        lua_pushinteger(L, i + 1);
        lua_pushinteger(L, data[i]);
        lua_settable(L, -3);
    }
    return 1;
}
int luaPrintS(lua_State *L)
{
    int x = lua_tonumber(L, -4);
    int y = lua_tonumber(L, -3);
    u8 color = lua_tonumber(L, -2);
    cstring str = lua_tostring(L, -1);
    printS(x, y, color, str);
    return 0; // number of results
}
int luaPrint(lua_State *L)
{
    cstring str = lua_tostring(L, -1);
    print(str);
    return 0; // number of results
}
void registerFunctions(lua_State *L)
{
    lua_register(L, "bgSet", luaBgSet);
    lua_register(L, "bgGet", luaBgGet);
    lua_register(L, "getMouseX", luaGetMouseX);
    lua_register(L, "getMouseY", luaGetMouseY);
    lua_register(L, "btn", luaBtn);
    lua_register(L, "setScrollX", luaSetScrollX);
    lua_register(L, "setScrollY", luaSetScrollY);
    lua_register(L, "getKeyPressed", luaGetKeyPressed);
    lua_register(L, "getCharPressed", luaGetCharPressed);
    lua_register(L, "bgTileLoad", luaBgTileLoad);
    lua_register(L, "spriteTileLoad", luaSpriteTileLoad);
    lua_register(L, "drawFilled", luaDrawFilled);
    lua_register(L, "setTransparent", LuaSetTransparent);
    lua_register(L, "setSprite", LuaSetSprite);
    lua_register(L, "setSpriteX", LuaSetSpriteX);
    lua_register(L, "setSpriteY", LuaSetSpriteY);
    lua_register(L, "setSpriteTileIndex", LuaSetSpriteTileIndex);
    lua_register(L, "saveTiles", luaSaveTiles);
    lua_register(L, "saveSprites", luaSaveSprites);
    lua_register(L, "loadTilesData", luaLoadTilesData);
    lua_register(L, "printS", luaPrintS);
    lua_register(L, "print", luaPrint);
}




void initLua(core *cCore)
{
    cCore->vm.L = luaL_newstate();
    luaL_openlibs(cCore->vm.L);
    registerFunctions(cCore->vm.L);
    if(cCore->ram.script!= NULL)
    {
        if (luaL_dostring(cCore->vm.L, cCore->ram.script) != LUA_OK) {
            fprintf(stderr, "Error: %s\n", lua_tostring(cCore->vm.L, -1));
            lua_pop(cCore->vm.L, 1);
        }
    }
    cCore->setup = execLuaSetup;
    cCore->loop = execLuaLoop;
    cCore->vblank = execLuaVBLANK;
    cCore->close = closeLua;
    cCore->resetVM = restartLua;
}
void closeLua(core *cCore)
{
    lua_close(cCore->vm.L);
}
void restartLua(core *cCore)
{
    printf("lua restarting\n");
    closeLua(cCore);
    initLua(cCore);
    printf("lua restarted\n");
}


void execLuaVBLANK(core *cCore)
{
    lua_getglobal(cCore->vm.L, "vblank");
    if(lua_isfunction(cCore->vm.L, -1))
        lua_call(cCore->vm.L, 0, 0);
    else
    {
        lua_pop(cCore->vm.L, 1);
        //printf("Vblank not found!!!\n");
    }
}
void execLuaLoop(core *cCore)
{
    lua_getglobal(cCore->vm.L, "loop");
    if(lua_isfunction(cCore->vm.L, -1))
        lua_call(cCore->vm.L, 0, 0);
    else
    {
        lua_pop(cCore->vm.L, 1);
        stopRunning();
    }
}
void execLuaSetup(core *cCore)
{

    lua_getglobal(cCore->vm.L, "setup");
    if(lua_isfunction(cCore->vm.L, -1))
        lua_call(cCore->vm.L, 0, 0);
    else
    {
        lua_pop(cCore->vm.L, 1);
        printf("Setup not found!!!\n");
    }

}

