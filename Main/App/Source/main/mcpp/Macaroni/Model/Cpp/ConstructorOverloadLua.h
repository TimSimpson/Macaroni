#ifndef MACARONI_MODEL_CPP_CONSTRUCTOROVERLOADLUA_H
#define MACARONI_MODEL_CPP_CONSTRUCTOROVERLOADLUA_H

#include "ConstructorOverload.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct ConstructorOverloadLuaMetaData
{	
	static int AssignmentListOpenInLua(lua_State * L);
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static MemberPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const ConstructorOverloadPtr & ptr, const std::string & index);
	static int NewIndex(lua_State * L, const FunctionOverloadPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const ConstructorOverloadPtr & ptr);
};


END_NAMESPACE

#endif


