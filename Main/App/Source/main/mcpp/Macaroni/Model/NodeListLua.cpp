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
#ifndef MACARONI_MODEL_NODELISTLUA_CPP
#define MACARONI_MODEL_NODELISTLUA_CPP

#include "Cpp/ClassLua.h"
#include "Cpp/FunctionLua.h"
#include "MemberLua.h"
#include "Member.h"
#include "Node.h"
#include "NodeLua.h"
#include "NodeListLua.h"
#include <Macaroni/Model/ReasonLuaMetaData.h>
#include <sstream>
#include "Cpp/VariableLua.h"

#define LUAGLUE_STARTNAMESPACE			BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE			END_NAMESPACE2
#define LUAGLUE_CLASSNAME				NodeList
#define LUAGLUE_CLASSREFNAME			NodeListPtr
#define LUAGLUE_CLASSFULLLUANAME		"Macaroni.Model.NodeList"
#define LUAGLUE_CLASSFULLCPPNAME		Macaroni::Model::NodeList
#define LUAGLUE_REGISTRATIONCLASSNAME	NodeListLuaMetaData
#define LUAGLUE_OPENOTHERMODULES		/*Macaroni::Model::NodeLuaMetaData::OpenInLua(L); */
#define LUAGLUE_CREATEMETATABLE YESPLEASE
#define LUAGLUE_HELPERCLASS				NodeListLuaGlueHelperClass

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & indexAsString)
	{		
		unsigned int index = luaL_checkinteger(L, 2);
		if (index > 0 && index <= ptr->size())
		{
			index --;
			NodePtr node = (*ptr)[index];
			NodeLuaMetaData::PutInstanceOnStack(L, (*ptr)[index]);
			return 1;
		}
		lua_pushnil(L);			
		return 1;
	}

	static int __newindex(lua_State * L)
	{		
		NodeListPtr & ptr = getInstance(L);
		// 2 must be a numeric index
		const unsigned int newIndex = luaL_checkint(L, 2);
		// 3 must be a Node
		NodePtr & node = NodeLuaMetaData::GetInstance(L, 3);
		
		const unsigned int actualIndex = newIndex - 1;
		if (actualIndex > ptr->size()) 
		{
			lua_pushlstring(L, "Index is greater than number of elements + 1.", 45);
			lua_error(L);
		} 
		else if (actualIndex == ptr->size())
		{
			ptr->push_back(node);
		} 
		else if (actualIndex >= 0) 
		{
			ptr->assign(actualIndex, node);
		}
		else
		{
			lua_pushlstring(L, "Index is less than zero.", 24);
			lua_error(L);
		}
		return 0;
	}

	static int __len(lua_State * L)
	{
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		int size = (*ptr).size();
		lua_pushnumber(L, size);
		return 1;
	}

	static int __new(lua_State * L)
	{
		// First arg is an array.
		luaL_checktype(L, 1, LUA_TTABLE);

		NodeListPtr nodeList(new NodeList());

		bool stop = false;
		int index = 0;		
		while(!stop)
		{
			index ++;			
			lua_pushnumber(L, index);
			lua_gettable(L, 1);
			if (!lua_isnil(L, -1))
			{
				if (NodeLuaMetaData::IsType(L, -1))
				{
					NodePtr & node = NodeLuaMetaData::GetInstance(L, -1);
					nodeList->push_back(node);
				}
				else
				{
					std::stringstream ss;
					// Reusing variables is evil, but SO MUCH FUN!  YEAA!!
					ss << "When constructing new NodeList, array argument "
					   << "had unexpected type at index " << index 
					   << ". All types in array are expected to be Nodes.";					
					luaL_error(L, ss.str().c_str());
				}
			}
			else
			{
				stop = true;
			}
		}

		NodeListLuaMetaData::PutInstanceOnStack(L, nodeList);
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		std::stringstream ss;
		ss << "NodeList, size " << ptr->size();
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__len", LUAGLUE_HELPERCLASS::__len}, \
		{"__newindex", LUAGLUE_HELPERCLASS::__newindex}, \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"New", LUAGLUE_HELPERCLASS::__new},

#include "../LuaGlue2.hpp"

#endif