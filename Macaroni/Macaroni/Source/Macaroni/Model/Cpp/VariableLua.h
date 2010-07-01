#ifndef MACARONI_MODEL_CPP_VARIABLELUA_H
#define MACARONI_MODEL_CPP_VARIABLELUA_H

#include "Variable.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct VariableLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static MemberPtr & GetInstance(lua_State * L, int index);	
	static int Index(lua_State * L, const VariablePtr var, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const MemberPtr & ptr);	
};


END_NAMESPACE

#endif
