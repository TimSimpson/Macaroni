#ifndef MACARONI_PARSER_PARSEREXCEPTIONLUA_H
#define MACARONI_PARSER_PARSEREXCEPTIONLUA_H

#include "../ME.h"
#include "ParserExceptionPtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Parser)

struct ParserExceptionLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static ParserExceptionPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const ParserExceptionPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const ParserExceptionPtr & ptr);
};
 
END_NAMESPACE2

#endif