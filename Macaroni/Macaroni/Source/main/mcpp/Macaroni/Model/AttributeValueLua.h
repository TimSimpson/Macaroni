#ifndef MACARONI_MODEL_ATTRIBUTEVALUELUA_H
#define MACARONI_MODEL_ATTRIBUTEVALUELUA_H

#include "AttributeTable.h"
#include "AttributeValue.h"
#include "NodePtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct AttributeValueLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static bool IsType(lua_State * L, int index, const char * type);
	static int OpenInLua(lua_State * L);
	static AttributeValuePtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const AttributeValuePtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const AttributeValuePtr & ptr);
};


END_NAMESPACE2

#endif
