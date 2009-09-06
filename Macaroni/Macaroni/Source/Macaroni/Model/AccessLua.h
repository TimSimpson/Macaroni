#ifndef MACARONI_MODEL_ACCESSLUA_H
#define MACARONI_MODEL_ACCESSLUA_H

#include "Node.lh"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct AccessLuaMetaData
{	
	static int OpenInLua(lua_State * L);
};


END_NAMESPACE2

#endif
