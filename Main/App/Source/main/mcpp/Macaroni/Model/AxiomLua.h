#ifndef MACARONI_MODEL_AXIOMLUA_H
#define MACARONI_MODEL_AXIOMLUA_H

#include "AxiomPtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct AxiomLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static AxiomPtr & GetInstance(lua_State * L, int index);
	static void PutInstanceOnStack(lua_State * L, const AxiomPtr & ptr);
};


END_NAMESPACE2

#endif
