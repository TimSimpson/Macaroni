#ifndef MACARONI_PARSER_PARSERLUA_H
#define MACARONI_PARSER_PARSERLUA_H

#include "../ME.h"
#include "ParserPtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Parser)

// Register the table for "Parser"
// Whenever the indexing function is called, dynamic cast to determine which 
// indexing functions in subclasses need to be called.
// This means this module must know of all the subclasses.
struct ParserLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static ParserPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const ParserPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const ParserPtr & ptr);
};

END_NAMESPACE2

#endif