#ifndef MACARONI_MODEL_CPP_FUNCTIONOVERLOADLUA_H
#define MACARONI_MODEL_CPP_FUNCTIONOVERLOADLUA_H

#include "FunctionOverload.h"
#include "../Member.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct FunctionOverloadLuaMetaData
{	
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static MemberPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const FunctionOverloadPtr & ptr, const std::string & index);
	static int NewIndex(lua_State * L, const FunctionOverloadPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const FunctionOverloadPtr & ptr);
};


END_NAMESPACE

#endif
