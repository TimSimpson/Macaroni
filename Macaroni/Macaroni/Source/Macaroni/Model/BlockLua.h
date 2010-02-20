#ifndef MACARONI_MODEL_BLOCKLUA_H
#define MACARONI_MODEL_BLOCKLUA_H

#include "Block.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct BlockLuaMetaData
{		
	static int Index(lua_State * L, const BlockPtr & ptr, const std::string & index);	
};


END_NAMESPACE2

#endif
