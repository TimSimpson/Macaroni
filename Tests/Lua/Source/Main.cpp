#include <iostream>
#include <sstream>
extern "C" 
{	
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <Macaroni/Tests/Lua/PoloLuaMetaData.h>

std::string LUA_CODE = 
"require 'os';                                                              "
"requre 'Macaroni.Tests.Lua.Polo';											"
"                                                                           "
"local Polo = Macaroni.Tests.Lua.Polo;										"
"																			"
"function formatTimeToString(date)                                          "
"    return (date.year .. '-' .. date.month .. '-' .. date.day .. '-' ..    "
"            date.hour .. ':' .. date.min .. ':' .. date.sec);              "
"end                                                                        "
"                                                                           "
"function localTime()                                                       "
"    return formatTimeToString( os.date('*t') );                            "
"end                                                                        "
"                                                                           "
"print('This code is being executed from Lua.');                            "
"print('The time is now ' .. localTime() .. '.');                           "
"                                                                           "
"                                                                           ";

static const struct luaL_Reg libs[] = {
	{"Macaroni.Tests.Lua.Polo", Macaroni::Tests::Lua::PoloLuaMetaData::OpenInLua},
	{0, 0} /* sentinel */
};

void openOurLibs(lua_State * L)
{
	lua_getglobal(L, "package");
	lua_pushstring(L, "preload");
	lua_gettable(L, -2);
	luaL_register(L, 0, libs);
}

int main(int argc, const char * argv[])
{	
	lua_State * L = luaL_newstate();
	luaL_openlibs(L);	
	openOurLibs(L);
	int error = luaL_loadbuffer(L, LUA_CODE.c_str(), LUA_CODE.size(), "Embedded Code")
		|| lua_pcall(L, 0, 0, 0);
	if (error) 
	{
		std::cout << "An error occured within Lua:" 
			<< lua_tostring(L, -1) << std::endl;
	}
	lua_close(L);
}