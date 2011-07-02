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
#ifndef MACARONI_MODEL_CPP_VARIABLEASSIGNMENTLUA_CPP
#define MACARONI_MODEL_CPP_VARIABLEASSIGNMENTLUA_CPP

#include "../Node.h"
#include "../NodeLua.h"
#include <sstream>
#include "VariableAssignmentLua.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

struct lua_State;

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE(Macaroni, Model, Cpp)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE
#define LUAGLUE_CLASSNAME VariableAssignment
#define LUAGLUE_CLASSREFNAME VariableAssignmentPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.Cpp.VariableAssignment"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::Cpp::VariableAssignment
#define LUAGLUE_REGISTRATIONCLASSNAME VariableAssignmentLuaMetaData
#define LUAGLUE_OPENOTHERMODULES //Macaroni::Model::NodeLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE YESPLEASE

#include "../../LuaGlue.hpp"

	static int __index(lua_State * L, const VariableAssignmentPtr ptr, 
									  const std::string & index)
	{
		if (index == "Expression")
		{
			lua_pushstring(L, ptr->Expression.c_str());
			return 1;
		}
		else if (index == "Variable")
		{
			Macaroni::Model::NodeLuaMetaData::PutInstanceOnStack(L, ptr->Variable);
			return 1;
		}
		return 0;
	}

	static int __tostring(lua_State * L)
	{
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		std::stringstream ss;
		ss << ptr->Variable->GetFullName();
		ss << " = ";
		ss << ptr->Expression;
		ss << ";";
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../../LuaGlue2.hpp"

#endif