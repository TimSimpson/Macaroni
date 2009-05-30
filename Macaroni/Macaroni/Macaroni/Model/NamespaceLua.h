#ifndef MACARONI_MODEL_NAMESPACELUA_H
#define MACARONI_MODEL_NAMESPACELUA_H

#include "Namespace.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct NamespaceLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static NamespacePtr & GetInstance(lua_State * L, int index);
	static void PutInstanceOnStack(lua_State * L, const NamespacePtr & ptr);
};


END_NAMESPACE2

#endif
