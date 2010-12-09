#ifndef MACARONI_MODEL_TYPEARGUMENTLUA_H
#define MACARONI_MODEL_TYPEARGUMENTLUA_H

#include "../ME.h"
#include "TypeArgumentPtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct TypeArgumentLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static TypeArgumentPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const TypeArgumentPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const TypeArgumentPtr & ptr);
};

END_NAMESPACE2

#endif