#ifndef MACARONI_MODEL_PRIMITIVELUA_H
#define MACARONI_MODEL_PRIMITIVELUA_H

#include "Primitive.lh"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct PrimitiveLuaMetaData
{
	static int OpenInLua(lua_State * L);
	static PrimitivePtr & GetInstance(lua_State * L, int index);
	static void PutInstanceOnStack(lua_State * L, const PrimitivePtr & ptr);
};


END_NAMESPACE2

#endif
