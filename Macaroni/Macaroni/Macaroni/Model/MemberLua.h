#ifndef MACARONI_MODEL_MEMBERLUA_H
#define MACARONI_MODEL_MEMBERLUA_H

#include "../ME.h"
#include "Member.lh"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct MemberLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static MemberPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const MemberPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const MemberPtr & ptr);
};

END_NAMESPACE2

#endif