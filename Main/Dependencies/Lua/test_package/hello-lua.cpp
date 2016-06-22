#include <iostream>
#include <string>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


const std::string LUA_CODE = "require 'os'; print('Hello Moon')";


int main() {
    lua_State * L = luaL_newstate();
    luaL_openlibs(L);
    int error = luaL_loadbuffer(L, LUA_CODE.c_str(), LUA_CODE.size(),
                                "hello-lua.cpp")
                || lua_pcall(L, 0, 0, 0);
    if (error)
    {
        std::cerr << "An error occured within Lua:"
            << lua_tostring(L, -1) << std::endl;
    }
    lua_close(L);
    return error;
}
