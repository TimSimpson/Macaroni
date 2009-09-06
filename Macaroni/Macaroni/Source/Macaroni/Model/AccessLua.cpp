#ifndef MACARONI_MODEL_ACCESSLUAREGISTER_CPP
#define MACARONI_MODEL_ACCESSLUAREGISTER_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "Access.h"
#include "AccessLua.h"
#include "../Exception.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Access"
#define GLOBALTABLENAME "Macaroni.Model.Access"

static const struct luaL_Reg tableMethods[]=
{
	{nullptr, nullptr}
};



int AccessLuaMetaData::OpenInLua(lua_State * L)
{	
	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	lua_pushlstring(L, "Private", 7);
	lua_pushinteger(L, Access_Private);
	lua_settable(L, -3);

	lua_pushlstring(L, "Protected", 9);
	lua_pushinteger(L, Access_Protected);
	lua_settable(L, -3);

	lua_pushlstring(L, "Public", 6);
	lua_pushinteger(L, Access_Public);
	lua_settable(L, -3);

	return 1;
}

END_NAMESPACE2

#endif
