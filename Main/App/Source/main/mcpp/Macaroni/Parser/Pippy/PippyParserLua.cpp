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
#ifndef MACARONI_PARSER_PIPPY_PIPPYPARSERLUA_CPP
#define MACARONI_PARSER_PIPPY_PIPPYPARSERLUA_CPP

#include "PippyParserLua.h"
#include "PippyParser.h"
#include "../../Model/Context.h"
#include "../../Model/ContextLua.h"
#include "../Parser.h"
#include "../ParserLua.h"
#include <Macaroni/Model/Source.h>
#include <Macaroni/Model/SourceLuaMetaData.h>
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
#define LUAGLUE_OPENOTHERMODULES //Macaroni::Parser::ParserLuaMetaData::OpenInLua(L);

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
		try
		{
			PippyParser::RunTests();
		} 
		catch(const std::exception & ex)
		{
			luaL_error(L, ex.what());
		}
		return 0;
	}

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		/*{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, */

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"Create", LUAGLUE_HELPERCLASS::Create}, \
		{"RunTests", LUAGLUE_HELPERCLASS::RunTests},

#include "../../LuaGlue2.hpp"

#endif