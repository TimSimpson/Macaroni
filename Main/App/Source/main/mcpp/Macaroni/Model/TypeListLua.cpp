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
#ifndef MACARONI_MODEL_TYPELISTLUA_CPP
#define MACARONI_MODEL_TYPELISTLUA_CPP

#include "TypeListLua.h"
#include "Node.h"
#include "NodeLua.h"
#include <sstream>
#include "Type.h"
#include "TypeLua.h"

#define LUAGLUE_STARTNAMESPACE			BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE			END_NAMESPACE2
#define LUAGLUE_CLASSNAME				TypeList
#define LUAGLUE_CLASSREFNAME			TypeListPtr
#define LUAGLUE_CLASSFULLLUANAME		"Macaroni.Model.TypeList"
#define LUAGLUE_CLASSFULLCPPNAME		Macaroni::Model::TypeList
#define LUAGLUE_REGISTRATIONCLASSNAME	TypeListLuaMetaData
#define LUAGLUE_OPENOTHERMODULES		//Macaroni::Model::TypeListLuaMetaData::OpenInLua(L); 
#define LUAGLUE_CREATEMETATABLE			YESPLEASE
#define LUAGLUE_HELPERCLASS				TypeListLuaGlueHelperClass

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & indexAsString)
	{		
		unsigned int index = luaL_checkinteger(L, 2);
		if (index > 0 && index <= ptr->size())
		{
			index --;			
			TypeLuaMetaData::PutInstanceOnStack(L, (*ptr)[index]);
			return 1;
		}
		lua_pushnil(L);			
		return 1;
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

		TypeListPtr typeList(new TypeList());

		bool stop = false;
		int index = 0;		
		while(!stop)
		{
			index ++;			
			lua_pushnumber(L, index);
			lua_gettable(L, 1);
			if (!lua_isnil(L, -1))
			{
				if (TypeLuaMetaData::IsType(L, -1))
				{
					TypePtr & type = TypeLuaMetaData::GetInstance(L, -1);
					typeList->push_back(type);
				}
				else
				{
					std::stringstream ss;
					ss << "When constructing new TypeList, array argument "
					   << "had unexpected type at index " << index 
					   << ". All types in array are expected to be Types.";
					luaL_error(L, ss.str().c_str());
				}
			}
			else
			{
				stop = true;
			}
		}

		TypeListLuaMetaData::PutInstanceOnStack(L, typeList);
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		//TODO: This-> LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		getInstance(L);
		std::stringstream ss;
		ss << "TypeList";
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__len", LUAGLUE_HELPERCLASS::__len}, \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"New", LUAGLUE_HELPERCLASS::__new},

#include "../LuaGlue2.hpp"

#endif