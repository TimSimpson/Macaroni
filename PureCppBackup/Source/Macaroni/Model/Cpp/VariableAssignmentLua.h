#ifndef MACARONI_MODEL_CPP_VARIABLEASSIGNMENTLUA_H
#define MACARONI_MODEL_CPP_VARIABLEASSIGNMENTLUA_H

#include "VariableAssignment.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct VariableAssignmentLuaMetaData
{
	static int Index(lua_State * L, const VariableAssignmentPtr & var, const std::string & index);
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static VariableAssignmentPtr & GetInstance(lua_State * L, int index);
	static void PutInstanceOnStack(lua_State * L, const VariableAssignmentPtr & ptr);
};


END_NAMESPACE

#endif
