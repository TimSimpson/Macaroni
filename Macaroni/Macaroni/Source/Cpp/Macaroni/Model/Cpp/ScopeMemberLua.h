#ifndef MACARONI_MODEL_SCOPEMEMBERLUA_H
#define MACARONI_MODEL_SCOPEMEMBERLUA_H

#include "ScopeMember.lh"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct ScopeMemberLuaMetaData
{	
	static ScopeMemberPtr & GetInstance(lua_State * L, int index);
	// Subclasses of ScopeMember can call this after checking all possibilities
	// within their own index function.
	static int Index(lua_State * L, ScopeMemberPtr & ptr, const std::string & index);
	static int OpenInLua(lua_State * L);
	static void PutInstanceOnStack(lua_State * L, const ScopeMemberPtr & ptr);
};


END_NAMESPACE2

#endif
