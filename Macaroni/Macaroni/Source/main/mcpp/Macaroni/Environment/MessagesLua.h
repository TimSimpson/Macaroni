#ifndef MACARONI_ENVIRONMENT_MESSAGESLUA_H
#define MACARONI_ENVIRONMENT_MESSAGESLUA_H

#include "../ME.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Environment)

struct MessagesLuaMetaData
{
	static int OpenInLua(lua_State * L);
};

END_NAMESPACE2

#endif