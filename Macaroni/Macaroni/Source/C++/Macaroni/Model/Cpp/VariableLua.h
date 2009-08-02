#ifndef MACARONI_MODEL_CPP_VARIABLELUA_H
#define MACARONI_MODEL_CPP_VARIABLELUA_H

#include "Variable.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct VariableLuaMetaData
{
	static int Index(lua_State * L, const VariablePtr var, const std::string & index);
};


END_NAMESPACE

#endif
