#ifndef MACARONI_MODEL_CPP_TYPEINFOLUA_H
#define MACARONI_MODEL_CPP_TYPEINFOLUA_H

#include "TypeInfo.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct TypeInfoLuaMetaData
{
	static int Index(lua_State * L, const TypeInfo & var, const std::string & index);
};


END_NAMESPACE

#endif
