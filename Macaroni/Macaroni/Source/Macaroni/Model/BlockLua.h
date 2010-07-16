#ifndef MACARONI_MODEL_BLOCKLUA_H
#define MACARONI_MODEL_BLOCKLUA_H

#include "Block.h"
#include "Member.h"
#include "MemberPtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct BlockLuaMetaData
{		
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static MemberPtr & GetInstance(lua_State * L, int index);	
	static int Index(lua_State * L, const BlockPtr & ptr, const std::string & index);	
	static void PutInstanceOnStack(lua_State * L, const MemberPtr & ptr);
};


END_NAMESPACE2

#endif
