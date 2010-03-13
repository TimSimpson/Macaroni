#ifndef MACARONI_MODEL_BLOCKLUA_CPP
#define MACARONI_MODEL_BLOCKLUA_CPP

#include "BlockLua.h"

//MARIO extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
//MARIO }


struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

int BlockLuaMetaData::Index(lua_State * L, const BlockPtr & ptr, 
									  const std::string & index)
{		
	if (index == "Code")
	{
		lua_pushstring(L, ptr->GetCode().c_str());
		return 1;
	}
	else if (index == "Id")
	{
		lua_pushstring(L, ptr->GetId().c_str());
		return 1;
	}		
	return 0;
}

END_NAMESPACE2

#endif
