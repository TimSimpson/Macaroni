#ifndef MACARONI_MODEL_SCOPELUA_H
#define MACARONI_MODEL_SCOPELUA_H

#include "Node.lh"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct NodeLuaMetaData
{
	/** Determines whether the variable on the given index of the Lua stack is
	 * a user data of type Node, and if so returns it. */
	static bool IsType(lua_State * L, int index);
	static bool IsType(lua_State * L, int index, const char * type);
	static int OpenInLua(lua_State * L);
	static NodePtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, NodePtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const NodePtr & ptr);
};


END_NAMESPACE2

#endif
