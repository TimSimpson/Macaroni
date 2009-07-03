#ifndef MACARONI_PARSER_PARSEREXCEPTIONLUA_CPP
#define MACARONI_PARSER_PARSEREXCEPTIONLUA_CPP

#include "ParserExceptionLua.h"
#include "ParserException.h"
#include "../Model/Source.h"
#include "../Model/SourceLua.h"
#include <sstream>

using Macaroni::Model::SourcePtr;
using Macaroni::Model::SourceLuaMetaData;

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Parser)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME ParserException
#define LUAGLUE_CLASSREFNAME ParserExceptionPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Parser.ParserException"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Parser::ParserException
#define LUAGLUE_REGISTRATIONCLASSNAME ParserExceptionLuaMetaData
#define LUAGLUE_OPENOTHERMODULES Macaroni::Model::SourceLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE YESPLEASE

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "Message")
		{
			lua_pushstring(L, ptr->GetMessage());
		}
		else if (index == "Source")
		{
			Macaroni::Model::SourceLuaMetaData::PutInstanceOnStack(L, ptr->GetSource());
		}	
		else 
		{
			lua_pushnil(L);			
		}
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		lua_pushstring(L, ptr->GetMessage());
		return 1;
	}


	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_CLASSNAMELuaFunctions::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_CLASSNAMELuaFunctions::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif