#ifndef MACARONI_PARSER_PIPPY_PIPPYPARSERLUA_H
#define MACARONI_PARSER_PIPPY_PIPPYPARSERLUA_H

#include "../../ME.h"
#include "PippyParser.lh"

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Parser, Pippy)

struct PippyParserLuaMetaData
{
	int Index(lua_State * L, const PippyParserPtr & ptr, 
			  const std::string & index);
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static PippyParserPtr & GetInstance(lua_State * L, int index);
	static void PutInstanceOnStack(lua_State * L, const PippyParserPtr & ptr);
};

END_NAMESPACE

#endif