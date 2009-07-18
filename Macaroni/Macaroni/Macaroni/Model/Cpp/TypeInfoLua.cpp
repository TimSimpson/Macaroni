#ifndef MACARONI_MODEL_CPP_TYPEINFOLUA_CPP
#define MACARONI_MODEL_CPP_TYPEINFOLUA_CPP

#include "../NodeLua.h"
#include "TypeInfoLua.h"
extern "C" {
	#include "../../../Lua/lua.h"
	#include "../../../Lua/lauxlib.h"
	#include "../../../Lua/lualib.h"
}

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int TypeInfoLuaMetaData::Index(lua_State * L, const TypeInfo & var, 
								const std::string & index)
{
	if (index == "Const")
	{
		lua_pushboolean(L, var.IsConst ? 1 : 0);
		return 1;
	}
	else if (index == "ConstPointer")
	{
		lua_pushboolean(L, var.IsConstPointer ? 1 : 0);
		return 1;
	}
	else if (index == "Pointer")
	{
		lua_pushboolean(L, var.IsPointer ? 1 : 0);
		return 1;
	}
	else if (index == "Reference")
	{
		lua_pushboolean(L, var.IsReference ? 1 : 0);
		return 1;
	}
	else if (index == "Node")
	{
		Model::NodeLuaMetaData::PutInstanceOnStack(L, var.Node);
		return 1;
	}
	return 0;
}


END_NAMESPACE

#endif
