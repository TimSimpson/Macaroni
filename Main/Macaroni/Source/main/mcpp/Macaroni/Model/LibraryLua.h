#ifndef MACARONI_MODEL_LIBRARYLUA_H
#define MACARONI_MODEL_LIBRARYLUA_H

#include "LibraryPtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct LibraryLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static bool IsType(lua_State * L, int index, const char * type);
	static int OpenInLua(lua_State * L);
	static LibraryPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, LibraryPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const LibraryPtr & ptr);
};


END_NAMESPACE2

#endif
