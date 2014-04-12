/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MACARONI_PARSER_PARSERLUA_CPP
#define MACARONI_PARSER_PARSERLUA_CPP

#include "ParserLua.h"
#include <Macaroni/Parser/Parser.h>
#include <boost/format.hpp>
#include <Macaroni/Model/Context.h>
#include "../Model/ContextLua.h"
#include "../Model/Library.h"
#include "../Model/LibraryLua.h"
#include <Macaroni/Parser/ParserException.h>
#include "ParserExceptionLua.h"
#include <Macaroni/Model/Source.h>
#include <Macaroni/Model/SourceLuaMetaData.h>
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
#define LUAGLUE_OPENOTHERMODULES ParserExceptionLuaMetaData::OpenInLua(L); lua_pop(L, 1);
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
			Macaroni::Parser::ParserException * copy =
				new Macaroni::Parser::ParserException(pe.GetSource(),
													  pe.GetMessage(),
													  boost::none);

			ParserExceptionPtr newPtr(copy);
			ParserExceptionLuaMetaData::Throw(L, newPtr, 1); // DOES NOT RETURN
			return 0;
		}
		catch(const std::exception & ex)
		{
			std::string msg = str(boost::format(
				"Error running Parser: %s") % ex.what());
			return luaL_error(L, msg.c_str());
		}
		catch(...)
		{
			return luaL_error(L,
				"Error running Parser: unknown exception type.");
		}
	}

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring},

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif
