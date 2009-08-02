#ifndef MACARONI_PARSER_PIPPY_PIPPYPARSERLUA_CPP
#define MACARONI_PARSER_PIPPY_PIPPYPARSERLUA_CPP

#include "PippyParserLua.h"
#include "PippyParser.h"
#include "../../Model/Context.h"
#include "../../Model/ContextLua.h"
#include "../Parser.h"
#include "../ParserLua.h"
#include "../../Model/Source.h"
#include "../../Model/SourceLua.h"
#include <sstream>

using Macaroni::Model::ContextPtr;
using Macaroni::Model::ContextLuaMetaData;
using Macaroni::Model::SourcePtr;
using Macaroni::Model::SourceLuaMetaData;

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE(Macaroni, Parser, Pippy)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE
#define LUAGLUE_CLASSNAME PippyParser
#define LUAGLUE_CLASSREFNAME PippyParserPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Parser.Pippy.PippyParser"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Parser::Pippy::PippyParser
#define LUAGLUE_REGISTRATIONCLASSNAME PippyParserLuaMetaData
#define LUAGLUE_OPENOTHERMODULES Macaroni::Parser::ParserLuaMetaData::OpenInLua(L);

#include "../../LuaGlue.hpp"

	/*static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{
		ParserPtr p = boost::dynamic_pointer_cast<Parser>(ptr);
		return ParserLuaMetaData::Index(L, p, index);		
	}

	static int __tostring(lua_State * L)
	{
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		lua_pushstring(L, "CppParser");
		return 1;
	}*/

	static int Create(lua_State * L)
	{
		PippyParserPtr newInstance = PippyParser::Create();
		ParserPtr castPtr = boost::static_pointer_cast<Parser>(newInstance);
		ParserLuaMetaData::PutInstanceOnStack(L, newInstance);
		return 1;
	}

	static int RunTests(lua_State * L)
	{
		PippyParser::RunTests();
		return 0;
	}

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		/*{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, */

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"Create", LUAGLUE_HELPERCLASS::Create}, \
		{"RunTests", LUAGLUE_HELPERCLASS::RunTests},

#include "../../LuaGlue2.hpp"

#endif