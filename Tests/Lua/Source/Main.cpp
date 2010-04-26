#include <iostream>
#include <sstream>
extern "C" 
{	
	#include <lauxlib.h>
	#include <lualib.h>
}

std::string LUA_CODE = 
"require 'os';                                                              "
"                                                                           "
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
"                                                                           ";

int main(int argc, const char * argv[])
{	

	lua_State * L = luaL_newstate();
	luaL_openlibs(L);
	int error = luaL_loadbuffer(L, LUA_CODE.c_str(), LUA_CODE.size(), "Embedded Code")
		|| lua_pcall(L, 0, 0, 0);
	if (error) 
	{
		std::cout << "An error occured within Lua:" 
			<< lua_tostring(L, -1) << std::endl;
	}
	lua_close(L);
}