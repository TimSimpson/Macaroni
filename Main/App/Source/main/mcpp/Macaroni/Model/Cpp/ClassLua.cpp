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
#ifndef MACARONI_MODEL_CPP_CLASSLUA_CPP
#define MACARONI_MODEL_CPP_CLASSLUA_CPP

#include "ClassLua.h"
#include <Macaroni/Model/Cpp/AccessLuaMetaData.h>
#include <Macaroni/Model/Cpp/ClassParentListLuaMetaData.h>
#include "../ElementLua.h"
#include <Macaroni/Model/LibraryLua.h>
#include "../NodeLua.h"
#include "../NodeListLua.h"
#include <Macaroni/Model/ReasonLuaMetaData.h>
 #include <Macaroni/Model/Project/Target.h>
 #include <Macaroni/Model/Project/TargetPtr.h>
#include <Macaroni/Model/Project/TargetLuaMetaData.h>
#include "TypeInfoLua.h"
#include "VariableLua.h"

//extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
//}


struct lua_State;

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE(Macaroni, Model, Cpp)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE
#define LUAGLUE_CLASSNAME Class
#define LUAGLUE_CLASSREFNAME ClassPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.Cpp.Class"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::Cpp::Class
#define LUAGLUE_REGISTRATIONCLASSNAME ClassLuaMetaData
#define LUAGLUE_HELPERCLASS ClassLuaFunctions
#define LUAGLUE_OPENOTHERMODULES // Macaroni::Model::NodeLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int FunctionArgumentsOpenInLua(lua_State * L);

int FunctionReturnTypeInfoOpenInLua(lua_State * L);

END_NAMESPACE

#include "../../LuaGlue.hpp"

	static int Create(lua_State * L)
	{
		using Macaroni::Model::Library;
		using Macaroni::Model::LibraryPtr;
		using Macaroni::Model::LibraryLuaMetaData;
		using Macaroni::Model::Element;
		using Macaroni::Model::ElementLuaMetaData;
		using Macaroni::Model::Node;
		using Macaroni::Model::NodeLuaMetaData;
		using Macaroni::Model::NodePtr;
		using Macaroni::Model::NodeList;
		using Macaroni::Model::NodeListLuaMetaData;
		using Macaroni::Model::NodeListPtr;
		using Macaroni::Model::Reason;
		using Macaroni::Model::ReasonLuaMetaData;
		using Macaroni::Model::ReasonPtr;
		using Macaroni::Model::Project::TargetPtr;
		using Macaroni::Model::Project::TargetLuaMetaData;

		LibraryPtr library;
		TargetPtr target;
		if (LibraryLuaMetaData::IsType(L, 1))
		{
			library = LibraryLuaMetaData::GetInstance(L, 1);
		}
		else if (TargetLuaMetaData::IsType(L, 1))
		{
			target = TargetLuaMetaData::GetInstance(L, 1);
		}
		else
		{
			luaL_error(L, "Expected Target for argument 1.");
		}


		if (!NodeLuaMetaData::IsType(L, 2))
		{
			luaL_error(L, "Expected Node for argument 2.");
		}
		NodePtr node = NodeLuaMetaData::GetInstance(L, 2);

		if (!AccessLuaMetaData::IsType(L, 3))
		{
			luaL_error(L, "Expected Access for argument 3.");
		}
		AccessPtr access = AccessLuaMetaData::GetInstance(L, 3);

		if (!NodeListLuaMetaData::IsType(L, 4))
		{
			luaL_error(L, "Expected NodeList for argument 4.");
		}
		NodeListPtr imports = NodeListLuaMetaData::GetInstance(L, 4);

		if (!ReasonLuaMetaData::IsType(L, 5))
		{
			luaL_error(L, "Expected Reason for argument 5.");
		}
		ReasonPtr reason = ReasonLuaMetaData::GetInstance(L, 5);

		ClassPtr newInstance;
		if (target)
		{
			newInstance = Class::Create(target, node, access, imports, reason);
		}
		else
		{
			newInstance = Class::Create(library, node, access, imports, reason);
		}
		ElementPtr memberPtr = boost::dynamic_pointer_cast<Element>(newInstance);
		ElementLuaMetaData::PutInstanceOnStack(L, memberPtr);
		return 1;
	}

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr,
									  const std::string & index)
	{
		if (index == "FriendNodes")
		{
			NodeListPtr list = ptr->GetFriendNodes();
			NodeListLuaMetaData::PutInstanceOnStack(L, list);
		}
		else if (index == "GlobalNodes")
		{
			NodeListPtr list = ptr->GetGlobalNodes();
			NodeListLuaMetaData::PutInstanceOnStack(L, list);
		}
		else if (index == "ImportedNodes")
		{
			NodeListPtr list = ptr->GetImportedNodes();
			NodeListLuaMetaData::PutInstanceOnStack(L, list);
		}
		else if (index == "Parents")
		{
			ClassParentListPtr list = ptr->GetParents();
			ClassParentListLuaMetaData::PutInstanceOnStack(L, list);
		}
		else
		{
			lua_pushnil(L);
		}

		/*if (index == "Arguments")
		{
			createLUAGLUE_CLASSREFNAMEUserData(L, ptr);
			luaL_getmetatable(L, PROPERTIES_METATABLENAME_ARGUMENTS);
			lua_setmetatable(L, -2);
			return 1;
		}
		else if (index == "ReturnType")
		{
			createLUAGLUE_CLASSREFNAMEUserData(L, ptr);
			luaL_getmetatable(L, PROPERTIES_METATABLENAME_RETURNTYPE);
			lua_setmetatable(L, -2);
			return 1;
		}*/
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		lua_pushstring(L, "Class");
		return 1;
	}

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		/*{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, */

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"Create", LUAGLUE_HELPERCLASS::Create},

#include "../../LuaGlue2.hpp"

#endif
