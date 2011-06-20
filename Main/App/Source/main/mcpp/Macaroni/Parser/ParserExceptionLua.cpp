#ifndef MACARONI_PARSER_PARSEREXCEPTIONLUA_CPP
#define MACARONI_PARSER_PARSEREXCEPTIONLUA_CPP

#include "ParserExceptionLua.h"
#include "ParserException.h"
#include <Macaroni/Model/Source.h>
#include <Macaroni/Model/SourceLuaMetaData.h>
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
#define LUAGLUE_HELPERCLASS ParserExceptionLuaFunctions
#define LUAGLUE_OPENOTHERMODULES /*Macaroni::Model::SourceLuaMetaData::OpenInLua(L);*/
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
		int LUAGLUE_CLASSNAME = 5;
		int HELPERCLASS = 1134;
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		std::stringstream ss;
		ss << "Description:" << ptr->GetMessage();
		ss << " ParsedSource:" << ptr->GetSource()->ToString();
		ss << " Thrown from:" << ptr->GetThrowLocation();		
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}


	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/	

#include "../LuaGlue2.hpp"

	LUAGLUE_STARTNAMESPACE

	void ParserExceptionLuaMetaData::Throw(lua_State *L, const ParserExceptionPtr & ptr, int level) 
	{		
		luaL_where(L, level);		
		ParserExceptionPtr newInstance(
				new ParserException(ptr->GetSource(), ptr->GetMessage(), lua_tostring(L, -1))
			);		
		ParserExceptionLuaMetaData::PutInstanceOnStack(L, newInstance);
		lua_error(L);
		// DOES NOT RETURN
	}

	LUAGLUE_ENDNAMESPACE

#endif