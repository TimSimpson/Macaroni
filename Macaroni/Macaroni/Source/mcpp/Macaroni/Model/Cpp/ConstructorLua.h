#ifndef MACARONI_MODEL_CPP_CONSTRUCTORLUA_H
#define MACARONI_MODEL_CPP_CONSTRUCTORLUA_H

#include "Constructor.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct ConstructorLuaMetaData
{	
	static int AssignmentListOpenInLua(lua_State * L);
	//static bool IsType(lua_State * L, int index);
	//static int OpenInLua(lua_State * L);
	//static ConstructorPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const ConstructorPtr & ptr, const std::string & index);
	//static void PutInstanceOnStack(lua_State * L, const ConstructorPtr & ptr);
};


END_NAMESPACE

#endif
