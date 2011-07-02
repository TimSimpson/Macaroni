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
#ifndef MACARONI_MODEL_TYPEARGUMENTLUA_CPP
#define MACARONI_MODEL_TYPEARGUMENTLUA_CPP

#include "Node.h"
#include "NodeLua.h"
#include "NodeListLua.h"
#include "TypeArgumentLua.h"
#include "TypeArgument.h"
#include "Type.h"
#include "TypeLua.h"
#include "TypeListLua.h"
#include <sstream>

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME TypeArgument
#define LUAGLUE_CLASSREFNAME TypeArgumentPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.TypeArgument"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::TypeArgument
#define LUAGLUE_REGISTRATIONCLASSNAME TypeArgumentLuaMetaData
#define LUAGLUE_OPENOTHERMODULES /*Macaroni::Model::NodeLuaMetaData::OpenInLua(L); Macaroni::Model::NodeListLuaMetaData::OpenInLua(L);*/
#define LUAGLUE_CREATEMETATABLE YESPLEASE
#define LUAGLUE_HELPERCLASS				TypeArgumentLuaGlueHelperClass

#include "../LuaGlue.hpp"

	static int __new(lua_State * L)
	{		
		if (!NodeLuaMetaData::IsType(L, 1))
		{			
			luaL_error(L, "Expected a Node for argument #1 in TypeArgument creator.");
		}
		NodePtr node = NodeLuaMetaData::GetInstance(L, 1);		

		TypeArgumentPtr typeArgument;

		if (TypeListLuaMetaData::IsType(L, 2))
		{
			TypeListPtr list = TypeListLuaMetaData::GetInstance(L, 2);
			typeArgument.reset(new TypeArgument(node, list));
		}
		else
		{
			typeArgument.reset(new TypeArgument(node));
		}
		
		TypeArgumentLuaMetaData::PutInstanceOnStack(L, typeArgument);
		return 1;
	}

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "Arguments")
		{
			TypeListPtr typeList = ptr->GetArguments();
			TypeListLuaMetaData::PutInstanceOnStack(L, typeList);
			return 1;
		}
		else if (index == "Node")
		{
			NodeLuaMetaData::PutInstanceOnStack(L, ptr->GetNode());
			return 1;
		}
		lua_pushnil(L);			
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		TypeArgumentPtr & ptr = getInstance(L);
		std::stringstream ss;
		ss << "(";
		ss << ptr->GetNode()->GetFullName();
		ss << " [TODO-list arguments here])";			
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"New", LUAGLUE_HELPERCLASS::__new},

#include "../LuaGlue2.hpp"

#endif