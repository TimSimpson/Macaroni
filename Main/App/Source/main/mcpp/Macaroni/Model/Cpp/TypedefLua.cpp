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
#ifndef MACARONI_MODEL_CPP_TYPEDEFLUA_CPP
#define MACARONI_MODEL_CPP_TYPEDEFLUA_CPP

#include "../../LuaGlueClear.h"
#include <Macaroni/Model/Cpp/Typedef.h>
#include "TypedefLua.h"
#include "../ElementLua.h"
#include "../NodeLua.h"
#include "../NodeListLua.h"
#include "../TypeLua.h"

#include <Macaroni/Lua.h>

struct lua_State;

#define LUAGLUE_STARTNAMESPACE	BEGIN_NAMESPACE(Macaroni, Model, Cpp)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE
#define LUAGLUE_CLASSNAME		Typedef
#define LUAGLUE_CLASSREFNAME	TypedefPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.Cpp.Typedef"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::Cpp::Typedef
#define LUAGLUE_REGISTRATIONCLASSNAME TypedefLuaMetaData
#define LUAGLUE_HELPERCLASS				TypedefLuaFunctions
#define LUAGLUE_OPENOTHERMODULES	//Macaroni::Model::NodeLuaMetaData::OpenInLua(L); //Macaroni::Model::TypeLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE YESPLEASE

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int FunctionArgumentsOpenInLua(lua_State * L);

int FunctionReturnTypeInfoOpenInLua(lua_State * L);

END_NAMESPACE

#include "../../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr,
									  const std::string & index)
	{
		if (index == "Type")
		{
			TypePtr type = ptr->GetType();
			TypeLuaMetaData::PutInstanceOnStack(L, type);
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		lua_pushstring(L, "Typedef");
		return 1;
	}

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		/*{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, */

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"New", LUAGLUE_HELPERCLASS::__new},*/

#include "../../LuaGlue2.hpp"

#endif
