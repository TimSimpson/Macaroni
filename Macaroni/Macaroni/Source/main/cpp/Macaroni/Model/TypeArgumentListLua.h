#ifndef MACARONI_MODEL_TYPEARGUMENTLISTLUA_H
#define MACARONI_MODEL_TYPEARGUMENTLISTLUA_H

#include "../ME.h"
#include "TypeArgumentPtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct TypeArgumentListLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static TypeArgumentListPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const TypeArgumentListPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const TypeArgumentListPtr & ptr);
};

END_NAMESPACE2

#endif