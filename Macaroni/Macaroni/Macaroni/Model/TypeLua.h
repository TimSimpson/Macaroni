#ifndef MACARONI_MODEL_TYPELUA_H
#define MACARONI_MODEL_TYPELUA_H

#include "Type.lh"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct TypeLuaMetaData
{	
	static TypePtr & GetInstance(lua_State * L, int index);
	// Subclasses of ScopeMember can call this after checking all possibilities
	// within their own index function.
	static int Index(lua_State * L, TypePtr & ptr, const std::string & index);
	static int OpenInLua(lua_State * L);
	static void PutInstanceOnStack(lua_State * L, const TypePtr & ptr);
};


END_NAMESPACE2

#endif
