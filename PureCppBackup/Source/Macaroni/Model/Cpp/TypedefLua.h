#ifndef MACARONI_MODEL_CPP_TYPEDEFLUA_H
#define MACARONI_MODEL_CPP_TYPEDEFLUA_H

#include "../../ME.h"
#include "TypedefPtr.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct TypedefLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static TypedefPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const TypedefPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const TypedefPtr & ptr);
};

END_NAMESPACE

#endif
