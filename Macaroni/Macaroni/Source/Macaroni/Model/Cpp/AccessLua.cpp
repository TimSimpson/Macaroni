#ifndef MACARONI_MODEL_CPP_ACCESSLUAREGISTER_CPP
#define MACARONI_MODEL_CPP_ACCESSLUAREGISTER_CPP

extern "C" {
	#include "../../../Lua/lua.h"
	#include "../../../Lua/lauxlib.h"
	#include "../../../Lua/lualib.h"
}
#include "Access.h"
#include "AccessLua.h"
#include "../../Exception.h"
#include <sstream>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

#define METATABLENAME "Macaroni.Model.Cpp.Access"
#define GLOBALTABLENAME "Macaroni.Model.Cpp.Access"

static const struct luaL_Reg tableMethods[]=
{
	{nullptr, nullptr}
};


int AccessLuaMetaData::PutInstanceOnStack(lua_State * L, Access access)
{
	if (access == Access_Private)
	{
		lua_pushstring(L, "Access_Private");
	}
	else if (access == Access_Protected)
	{
		lua_pushstring(L, "Access_Protected");
	}
	else if (access == Access_Public)
	{
		lua_pushstring(L, "Access_Public");
	}
	else
	{
		lua_pushnil(L);
	}
	return 1;
}

int AccessLuaMetaData::OpenInLua(lua_State * L)
{	
	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	lua_pushlstring(L, "Private", 7);
	lua_pushstring(L, "Access_Private");
	lua_settable(L, -3);

	lua_pushlstring(L, "Protected", 9);
	lua_pushstring(L, "Access_Protected");
	lua_settable(L, -3);

	lua_pushlstring(L, "Public", 6);
	lua_pushstring(L, "Access_Public");
	lua_settable(L, -3);

	return 1;
}

END_NAMESPACE

#endif
