#ifndef MACARONI_MODEL_CONTEXTLUA_H
#define MACARONI_MODEL_CONTEXTLUA_H

#include "Context.lh"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct ContextLuaMetaData
{
	/** Creates table in Lua. */
	static int OpenInLua(lua_State * L);
	/** Finds a userdata on the stack and converts it to a usable instance. */
	static ContextPtr & GetInstance(lua_State * L, int index);
	/** Puts an instance on the lua_State's stack. */
	static void PutInstanceOnStack(lua_State * L, const ContextPtr & ptr);
};


END_NAMESPACE2

#endif
