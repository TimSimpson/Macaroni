#ifndef MACARONI_MODEL_ATTRIBUTETABLELUA_H
#define MACARONI_MODEL_ATTRIBUTETABLELUA_H

#include "AttributeTable.h"
#include "AttributeValue.h"
#include "NodePtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct AttributeTableLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static bool IsType(lua_State * L, int index, const char * type);
	static int OpenInLua(lua_State * L);
	static AttributeTablePtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const AttributeTablePtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const AttributeTablePtr & ptr);
};


END_NAMESPACE2

#endif
