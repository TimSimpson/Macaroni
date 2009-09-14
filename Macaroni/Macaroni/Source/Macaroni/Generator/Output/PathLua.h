#ifndef MACARONI_GENERATOR_OUTPUT_PATHLUA_H
#define MACARONI_GENERATOR_OUTPUT_PATHLUA_H

#include "Path.h"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Generator, Output)

struct PathLuaMetaData
{	
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static PathPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const PathPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const PathPtr & ptr);
};


END_NAMESPACE

#endif
