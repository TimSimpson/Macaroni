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
#ifndef MACARONI_MODEL_ELEMENTLUA_CPP
#define MACARONI_MODEL_ELEMENTLUA_CPP

#include <Macaroni/Model/Cpp/AccessLuaMetaData.h>
#include "BlockLua.h"
#include "Cpp/ClassLua.h"
#include "Cpp/ConstructorLua.h"
#include "Cpp/ConstructorOverloadLua.h"
 #include <Macaroni/Model/Cpp/Enum.h>
 #include <Macaroni/Model/Cpp/EnumPtr.h>
#include <Macaroni/Model/Cpp/EnumLuaMetaData.h>
#include "Cpp/FunctionLua.h"
#include "Cpp/FunctionOverloadLua.h"
#include "LibraryLua.h"
#include "ElementLua.h"
#include <Macaroni/Model/Element.h>
#include "Node.h"
#include "NodeLua.h"
#include <Macaroni/Model/ReasonLuaMetaData.h>
#include <Macaroni/Model/Cpp/Scope.h>
#include <sstream>
#include "Cpp/Typedef.h"
#include "Cpp/TypedefLua.h"
#include "Cpp/VariableLua.h"
#include <Macaroni/Model/Project/Target.h>
#include <Macaroni/Model/Project/TargetPtr.h>
#include <Macaroni/Model/Project/TargetLuaMetaData.h>

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME Element
#define LUAGLUE_CLASSREFNAME ElementPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.Element"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::Element
#define LUAGLUE_REGISTRATIONCLASSNAME ElementLuaMetaData
#define LUAGLUE_OPENOTHERMODULES \
	Macaroni::Model::Cpp::ConstructorOverloadLuaMetaData::OpenInLua(L); \
	Macaroni::Model::Cpp::ConstructorOverloadLuaMetaData::AssignmentListOpenInLua(L);
	//Macaroni::Model::NodeLuaMetaData::OpenInLua(L); \
	//Macaroni::Model::Cpp::FunctionLuaMetaData::OpenInLua(L); \
	//Macaroni::Model::Cpp::ClassLuaMetaData::OpenInLua(L);  \
	//Macaroni::Model::Cpp::TypedefLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE YESPLEASE


#include "../LuaGlue.hpp"

	static int ownedBy(lua_State * L)
	{
		LUA_GLUE_TRY
		ElementPtr element = getInstance(L);
		if (Macaroni::Model::Project::TargetLuaMetaData::IsType(L, 2))
		{
			Macaroni::Model::Project::TargetPtr target =
				Macaroni::Model::Project::TargetLuaMetaData::GetInstance(L, 2);
			bool result = element->OwnedBy(target);
			lua_pushboolean(L, result ? 1 : 0);
			return 1;
		}
		else if (LibraryLuaMetaData::IsType(L, 2))
		{
			// The Lua generators used to work by checking member.Library to
			// see if it was equal to the target Library.
			// Now there is the superior "OwnedBy" method that accepts a target,
			// not an old-style Library, but to keep compatability with the
			// manifest system this function can be given a Library in
			// Lua Glue and will check if its equal to the Library owned by the
			// element, if any.
			LibraryPtr lib = LibraryLuaMetaData::GetInstance(L, 2);
			LibraryElement * lm;
			if ((lm = dynamic_cast<LibraryElement *>(element.get())) != nullptr)
			{
				LibraryPtr otherLib = lm->GetLibrary();
				const bool result = lib->GetId() == otherLib->GetId();
				lua_pushboolean(L, result ? 1 : 0);
				return 1;
			}
			lua_pushboolean(L, 0);
			return 1;
		}
		else
		{
			return luaL_error(L, "Expected a Target or (old-style) Library for "
				                 "argument 2.");
		}

		LUA_GLUE_CATCH
	}

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr,
									  const std::string & index)
	{
		LUA_GLUE_TRY

		if (index == "Access")
		{
			Cpp::ScopeMemberPtr smPtr = boost::dynamic_pointer_cast<Cpp::ScopeMember>(ptr);
			if (!!smPtr)
			{
				Cpp::AccessPtr access = smPtr->GetAccess();
				Cpp::AccessLuaMetaData::PutInstanceOnStack(L, access);
				return 1;
			}
		}
		else if (index == "Static")
		{
			Cpp::ScopeMemberPtr smPtr = boost::dynamic_pointer_cast<Cpp::ScopeMember>(ptr);
			if (!!smPtr)
			{
				bool isStatic = smPtr->IsStatic();
				lua_pushboolean(L, isStatic);
				return 1;
			}
		}
		else if (index == "Node")
		{
			NodeLuaMetaData::PutInstanceOnStack(L, ptr->GetNode());
			return 1;
		}
		else if (index == "OwnedBy")
		{
			lua_pushcfunction(L, ownedBy);
			return 1;
		}
		else if (index == "Owner")
		{
			Macaroni::Model::Project::TargetLuaMetaData::PutInstanceOnStack(
				L, ptr->GetOwner());
			return 1;
		}
		else if (index == "ReasonCreated")
		{
			ReasonLuaMetaData::PutInstanceOnStack(L, ptr->GetReasonCreated());
			return 1;
		}
		else if (index == "RequiresCppFile")
		{
			lua_pushboolean(L, ptr->RequiresCppFile());
			return 1;
		}
		else if (index == "RequiresHFile")
		{
			lua_pushboolean(L, ptr->RequiresHFile());
			return 1;
		}
		else if (index == "SwitchOwner")
		{
			lua_pushcfunction(L, switchOwner);
			return 1;
		}
		else if (index == "TypeName")
		{
			if (ptr->GetTypeName() == nullptr)
			{
				lua_pushnil(L);
			} else {
				lua_pushstring(L, ptr->GetTypeName());
			}
			return 1;
		}

		LibraryElement * lm;
		if ((lm = dynamic_cast<LibraryElement *>(ptr.get())) != nullptr)
		{
			if (index == "Library")
			{
				LibraryLuaMetaData::PutInstanceOnStack(L, lm->GetLibrary());
				return 1;
			}
		}

		if (!!boost::dynamic_pointer_cast<Block>(ptr))
		{
			int rtnCnt = BlockLuaMetaData::Index(L, boost::dynamic_pointer_cast<Block>(ptr), index);
			if (rtnCnt > 0)
			{
				return rtnCnt;
			}
		}
		if (!!boost::dynamic_pointer_cast<Cpp::Class>(ptr))
		{
			int rtnCnt = Cpp::ClassLuaMetaData::Index(L, boost::dynamic_pointer_cast<Cpp::Class>(ptr), index);
			if (rtnCnt > 0)
			{
				return rtnCnt;
			}
		}
		if (!!boost::dynamic_pointer_cast<Cpp::Enum>(ptr))
		{
			int rtnCnt = Cpp::EnumLuaMetaData::Index(L, boost::dynamic_pointer_cast<Cpp::Enum>(ptr), index);
			if (rtnCnt > 0)
			{
				return rtnCnt;
			}
		}
		if (!!boost::dynamic_pointer_cast<Cpp::Variable>(ptr))
		{
			int rtnCnt = Cpp::VariableLuaMetaData::Index(L, boost::dynamic_pointer_cast<Cpp::Variable>(ptr), index);
			if (rtnCnt > 0)
			{
				return rtnCnt;
			}
		}
		if (!!boost::dynamic_pointer_cast<Cpp::Function>(ptr))
		{
			int rtnCnt = Cpp::FunctionLuaMetaData::Index(L, boost::dynamic_pointer_cast<Cpp::Function>(ptr), index);
			if (rtnCnt > 0)
			{
				return rtnCnt;
			}
		}
		if (!!boost::dynamic_pointer_cast<Cpp::ConstructorOverload>(ptr))
		{
			int rtnCnt = Cpp::ConstructorOverloadLuaMetaData::Index(L, boost::dynamic_pointer_cast<Cpp::ConstructorOverload>(ptr), index);
			if (rtnCnt > 0)
			{
				return rtnCnt;
			}
		}
		if (!!boost::dynamic_pointer_cast<Cpp::FunctionOverload>(ptr))
		{
			int rtnCnt = Cpp::FunctionOverloadLuaMetaData::Index(L, boost::dynamic_pointer_cast<Cpp::FunctionOverload>(ptr), index);
			if (rtnCnt > 0)
			{
				return rtnCnt;
			}
		}
		if (!!boost::dynamic_pointer_cast<Cpp::Constructor>(ptr))
		{
			int rtnCnt = Cpp::ConstructorLuaMetaData::Index(L, boost::dynamic_pointer_cast<Cpp::Constructor>(ptr), index);
			if (rtnCnt > 0)
			{
				return rtnCnt;
			}
		}
		if (!!boost::dynamic_pointer_cast<Cpp::Typedef>(ptr))
		{
			int rtnCnt = Cpp::TypedefLuaMetaData::Index(L, boost::dynamic_pointer_cast<Cpp::Typedef>(ptr), index);
			if (rtnCnt > 0)
			{
				return rtnCnt;
			}
		}

		lua_pushnil(L);
		return 1;

		LUA_GLUE_CATCH
	}

	static int switchOwner(lua_State * L)
	{
		LUA_GLUE_TRY
			ElementPtr element = getInstance(L);
		Macaroni::Model::Project::TargetPtr newTarget =
			Macaroni::Model::Project::TargetLuaMetaData::GetInstance(L, 2);
		element->SwitchOwner(newTarget);
		return 0;
		LUA_GLUE_CATCH
	}

	static int __tostring(lua_State * L)
	{
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		std::stringstream ss;
		ss << "[";
		ss << ptr->GetTypeName();
		ss << "]";
		ss << ptr->GetNode()->GetFullName();
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring},

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif
