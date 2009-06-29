#ifndef MACARONI_PARSER_CPP_CPPPARSERLUA_CPP
#define MACARONI_PARSER_CPP_CPPPARSERLUA_CPP

#include "CppParserLua.h"
#include "CppParser.h"
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

/** Because CREATEMETATABLE is not defined, there is no metatable or type info
 * for CppParser within Lua.  But we do have a global table, which we can use
 * to call a C++ function to create a CppParser and pass that back wrapped in
 * delicious Parser Lua glue.  So from Lua, it looks like any other ParserPtr,
 * but of course the virtual methods will be set to CppParser.
 * The flipside to this is that if there are additional methods in CppParser
 * we wish to access from Lua, we must modify the Parser Lua glue to type
 * check the instance, cast to CppParser if possible, and then call the 
 * functions. */
#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE(Macaroni, Parser, Cpp)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE
#define LUAGLUE_CLASSNAME CppParser
#define LUAGLUE_CLASSREFNAME CppParserPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Parser.Cpp.CppParser"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Parser::Cpp::CppParser
#define LUAGLUE_REGISTRATIONCLASSNAME CppParserLuaMetaData
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
		CppParserPtr newInstance = CppParser::Create();
		ParserPtr castPtr = boost::static_pointer_cast<Parser>(newInstance);
		ParserLuaMetaData::PutInstanceOnStack(L, newInstance);
		return 1;
	}

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		/*{"__tostring", LUAGLUE_CLASSNAMELuaFunctions::__tostring}, */

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"Create", LUAGLUE_CLASSNAMELuaFunctions::Create},

#include "../../LuaGlue2.hpp"

#endif