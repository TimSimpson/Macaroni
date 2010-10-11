#ifndef MACARONI_PARSER_PARSERLUA_CPP
#define MACARONI_PARSER_PARSERLUA_CPP

#include "ParserLua.h"
#include "Parser.h"
#include "../Model/Context.h"
#include "../Model/ContextLua.h"
#include "../Model/Library.h"
#include "../Model/LibraryLua.h"
#include "ParserException.h"
#include "ParserExceptionLua.h"
#include "../Model/Source.h"
#include "../Model/SourceLua.h"
#include <sstream>

using Macaroni::Model::ContextPtr;
using Macaroni::Model::ContextLuaMetaData;
using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using Macaroni::Model::LibraryLuaMetaData;
using Macaroni::Model::SourcePtr;
using Macaroni::Model::SourceLuaMetaData;

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Parser)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME Parser
#define LUAGLUE_CLASSREFNAME ParserPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Parser.Parser"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Parser::Parser
#define LUAGLUE_REGISTRATIONCLASSNAME ParserLuaMetaData
#define LUAGLUE_HELPERCLASS ParserLuaFunctions
#define LUAGLUE_OPENOTHERMODULES /*ParserExceptionLuaMetaData::OpenInLua(L);*/
#define LUAGLUE_CREATEMETATABLE YESPLEASE

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "Read") 
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::Read);
		}
		else 
		{
			lua_pushnil(L);			
		}
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		//LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		lua_pushstring(L, "Parser"); 
		return 1;
	}

	static int Read(lua_State * L)
	{
		ParserPtr self = getInstance(L, 1);
		LibraryPtr library = LibraryLuaMetaData::GetInstance(L, 2);
		SourcePtr src = SourceLuaMetaData::GetInstance(L, 3);
		std::string text(luaL_checkstring(L, 4));

		try
		{
			int result = self->Read(library, src, text);
			lua_pushinteger(L, result);
			return 1;
		}
		catch(Macaroni::Parser::ParserException & pe)
		{
			Macaroni::Parser::ParserException * copy = new Macaroni::Parser::ParserException(pe.GetSource(), pe.GetMessage());

			ParserExceptionPtr newPtr(copy);
			ParserExceptionLuaMetaData::Throw(L, newPtr, 1); // DOES NOT RETURN			
			return 0;
		}
	}

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif