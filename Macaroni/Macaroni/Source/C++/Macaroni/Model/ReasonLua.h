#ifndef MACARONI_MODEL_REASONLUA_H
#define MACARONI_MODEL_REASONLUA_H

#include "Reason.lh"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct ReasonLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static ReasonPtr & GetInstance(lua_State * L, int index);
	static void PutInstanceOnStack(lua_State * L, const ReasonPtr & ptr);
};


END_NAMESPACE2

#endif
