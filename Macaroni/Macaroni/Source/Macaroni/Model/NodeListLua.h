#ifndef MACARONI_MODEL_NODELISTLUA_H
#define MACARONI_MODEL_NODELISTLUA_H

#include "../ME.h"
#include "Node.lh"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct NodeListLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static NodeListPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const NodeListPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const NodeListPtr & ptr);
};

END_NAMESPACE2

#endif