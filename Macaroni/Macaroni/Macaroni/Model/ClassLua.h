#ifndef MACARONI_MODEL_CLASSLUA_H
#define MACARONI_MODEL_CLASSLUA_H

#include "Class.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct ClassLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static void PutInstanceOnStack(lua_State * L, const ClassPtr & ptr);
};


END_NAMESPACE2

#endif
