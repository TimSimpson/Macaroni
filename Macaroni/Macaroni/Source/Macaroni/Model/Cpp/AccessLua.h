#ifndef MACARONI_MODEL_CPP_ACCESSLUA_H
#define MACARONI_MODEL_CPP_ACCESSLUA_H

#include "Access.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct AccessLuaMetaData
{	
	static int OpenInLua(lua_State * L);
	static int PutInstanceOnStack(lua_State * L, Access access);
};


END_NAMESPACE

#endif
