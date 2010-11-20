#include <iostream>
#include <sstream>
#include <string>
#include <boost/shared_ptr.hpp>

//extern "C" 
//{	
	#include <lauxlib.h>
	#include <lualib.h>
//}
#include <Macaroni/Tests/Lua/_.h>
#include <Macaroni/Tests/Lua/Polo.h>
#include <Macaroni/Tests/Lua/PoloLuaMetaData.h>

using Macaroni::Tests::Lua::Polo;
using Macaroni::Tests::Lua::PoloPtr;
using Macaroni::Tests::Lua::PoloLuaMetaData;

std::string LUA_CODE = 
"require 'os';                                                              "
"require 'Macaroni.Tests.Lua.Polo';											"
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
"function callMe(polo)                                                      "
"    print([[Polo's name is ]] .. polo:GetName());                          "
"end	                                                                    "
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
	PoloPtr blah(new Polo());
	blah->SetName("Arthur Mc. Barthur");
	
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
	
	// Now, call the function "callMe" which was not run originally, but 
	// exists in the LuaState.
	lua_getglobal(L, "callMe");
	PoloLuaMetaData::PutInstanceOnStack(L, blah);
	lua_call(L, 1, 0);
	
	lua_close(L);
}