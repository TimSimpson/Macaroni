#ifndef MACARONI_MODEL_TYPELISTLUA_H
#define MACARONI_MODEL_TYPELISTLUA_H

#include "../ME.h"
#include "Type.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct TypeListLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static TypeListPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const TypeListPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const TypeListPtr & ptr);
};

END_NAMESPACE2

#endif