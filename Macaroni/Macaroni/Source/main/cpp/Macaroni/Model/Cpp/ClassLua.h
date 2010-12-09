#ifndef MACARONI_MODEL_CPP_CLASSLUA_H
#define MACARONI_MODEL_CPP_CLASSLUA_H

#include "Class.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct ClassLuaMetaData
{	
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static ClassPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const ClassPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const ClassPtr & ptr);
};


END_NAMESPACE

#endif
