#ifndef MACARONI_MODEL_TYPELUA_H
#define MACARONI_MODEL_TYPELUA_H

#include "../ME.h"
#include "Type.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct TypeLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static TypePtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const TypePtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const TypePtr & ptr);
};

END_NAMESPACE2

#endif