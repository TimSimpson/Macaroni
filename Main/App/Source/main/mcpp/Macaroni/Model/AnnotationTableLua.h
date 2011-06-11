#ifndef MACARONI_MODEL_ATTRIBUTETABLELUA_H
#define MACARONI_MODEL_ATTRIBUTETABLELUA_H

#include "AnnotationTable.h"
#include "AnnotationValue.h"
#include "NodePtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct AnnotationTableLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static bool IsType(lua_State * L, int index, const char * type);
	static int OpenInLua(lua_State * L);
	static AnnotationTablePtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const AnnotationTablePtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const AnnotationTablePtr & ptr);
};


END_NAMESPACE2

#endif
