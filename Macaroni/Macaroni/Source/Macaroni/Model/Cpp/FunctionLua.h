#ifndef MACARONI_MODEL_CPP_FUNCTIONLUA_H
#define MACARONI_MODEL_CPP_FUNCTIONLUA_H

#include "Function.h"
#include "../Member.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct FunctionLuaMetaData
{	
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static MemberPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const FunctionPtr & ptr, const std::string & index);
	static int NewIndex(lua_State * L, const FunctionPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const MemberPtr & ptr);
};


END_NAMESPACE

#endif
