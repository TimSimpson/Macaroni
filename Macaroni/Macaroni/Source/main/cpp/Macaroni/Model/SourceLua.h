#ifndef MACARONI_MODEL_SOURCELUA_H
#define MACARONI_MODEL_SOURCELUA_H

#include "SourcePtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct SourceLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static SourcePtr GetInstance(lua_State * L, int index);
	static void PutInstanceOnStack(lua_State * L, const SourcePtr & ptr);
};


END_NAMESPACE2

#endif
