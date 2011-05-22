#ifndef MACARONI_MODEL_CPP_CLASSPARENTLISTLUAMETADATA_H
#define MACARONI_MODEL_CPP_CLASSPARENTLISTLUAMETADATA_H

#include <Macaroni/ME.h>
#include <Macaroni/Model/Cpp/_.h>

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

struct ClassParentListLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static ClassParentListPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const ClassParentListPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const ClassParentListPtr & ptr);
};

END_NAMESPACE

#endif