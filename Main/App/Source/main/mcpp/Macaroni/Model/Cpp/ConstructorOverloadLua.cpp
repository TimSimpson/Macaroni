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
#ifndef MACARONI_MODEL_CPP_CONSTRUCTOROVERLOADLUA_CPP
#define MACARONI_MODEL_CPP_CONSTRUCTOROVERLOADLUA_CPP

#include <Macaroni/Model/Cpp/AccessLuaMetaData.h>
#include "../NodeLua.h"
#include "../../Environment/DebugLog.h"
#include "ConstructorOverloadLua.h"
#include "../ElementLua.h"
#include "FunctionLua.h"
#include "FunctionOverloadLua.h"
#include "../ModelInconsistencyException.h"
#include "../NodeLua.h"
#include "../NodeListLua.h"
#include <Macaroni/Model/ReasonLuaMetaData.h>
#include <Macaroni/Model/SourceLuaMetaData.h>
#include "VariableLua.h"
#include "VariableAssignmentLua.h"
#include "../TypeLua.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


#define GLOBALTABLENAME "Macaroni.Model.Cpp.ConstructorOverload"

struct lua_State;

using Macaroni::Model::Cpp::Access;
using Macaroni::Model::Cpp::AccessLuaMetaData;
using Macaroni::Model::Cpp::ConstructorOverload;
using Macaroni::Model::Cpp::ConstructorOverloadPtr;
using Macaroni::Model::Cpp::Function;
using Macaroni::Model::Cpp::FunctionLuaMetaData;
using Macaroni::Model::Cpp::FunctionPtr;
using Macaroni::Model::Cpp::FunctionOverload;
using Macaroni::Model::Cpp::FunctionOverloadPtr;
using Macaroni::Model::Element;
using Macaroni::Model::ElementLuaMetaData;
using Macaroni::Model::Node;
using Macaroni::Model::NodeLuaMetaData;
using Macaroni::Model::ReasonLuaMetaData;
using Macaroni::Model::ReasonPtr;
using Macaroni::Model::Source;
using Macaroni::Model::SourceLuaMetaData;
using Macaroni::Model::SourcePtr;
using Macaroni::Model::Type;
using Macaroni::Model::TypeLuaMetaData;
using Macaroni::Model::Cpp::VariableAssignmentLuaMetaData;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

namespace 
{
	//TODO: Actually register the creation methods in Lua.

	static const struct luaL_Reg tableMethods[]=
	{
	//	{"Create", FunctionOverloadLuaFunctions::Create},
		{nullptr, nullptr}
	};
} // end of anon namespace

ElementPtr & ConstructorOverloadLuaMetaData::GetInstance(lua_State * L, int index)
{
	return ElementLuaMetaData::GetInstance(L, index);
}

bool ConstructorOverloadLuaMetaData::IsType(lua_State * L, int index)
{
	return ElementLuaMetaData::IsType(L, index);
}

int ConstructorOverloadLuaMetaData::OpenInLua(lua_State * L) 
{
	luaL_register(L, GLOBALTABLENAME, tableMethods);		
	return 1;
}

void ConstructorOverloadLuaMetaData::PutInstanceOnStack(lua_State * L, const ConstructorOverloadPtr & ptr)
{
	ElementPtr memberPtr = boost::dynamic_pointer_cast<Element>(ptr);
	ElementLuaMetaData::PutInstanceOnStack(L, memberPtr);
}

int ConstructorOverloadLuaMetaData::Index(lua_State * L, 
										  const ConstructorOverloadPtr & ptr, 
										  const std::string & index)
{	
	if (index == "Arguments")
	{
		NodeListPtr argList = ptr->GetArguments();
		Model::NodeListLuaMetaData::PutInstanceOnStack(L, argList);
		return 1;
	}		
	if (index == "Assignments")
	{
		void * memory = lua_newuserdata(L, sizeof(ConstructorOverloadPtr));
		ConstructorOverloadPtr * instance = new (memory) ConstructorOverloadPtr();
		(*instance).operator=(ptr);
		luaL_getmetatable(L, "Macaroni.Model.Cpp.ConstructorOverload.Assignments");
		lua_setmetatable(L, -2); 
		return 1;
	}				
	return 0;
}


ConstructorOverloadPtr & assignmentsGetInstance(lua_State * L, int position)
{
	ConstructorOverloadPtr * ptr = (ConstructorOverloadPtr *) luaL_checkudata(L, position, "Macaroni.Model.Cpp.ConstructorOverload.Assignments");
	return *ptr;
}

int assignmentsEqual(lua_State * L)
{
	ConstructorOverloadPtr & ptr1 = assignmentsGetInstance(L, 1);
	ConstructorOverloadPtr & ptr2 = assignmentsGetInstance(L, 2);
	lua_pushboolean(L, ptr1.get() == ptr2.get());
	return 1;
}


int assignmentsGc(lua_State * L)
{
	ConstructorOverloadPtr & ptr = assignmentsGetInstance(L, 1);
	ptr.~ConstructorOverloadPtr();
	return 1;
}

int assignmentsIndex(lua_State * L)
{
	ConstructorOverloadPtr & ptr = assignmentsGetInstance(L, 1);
	int index = luaL_checkinteger(L, 2);
	index --;
	VariableAssignmentPtr luaData(new VariableAssignment(ptr->GetAssignment(index)));
	VariableAssignmentLuaMetaData::PutInstanceOnStack(L, luaData);
	return 1;
}

int assignmentsLen(lua_State * L)
{
	ConstructorOverloadPtr & ptr = assignmentsGetInstance(L, 1);
	lua_pushinteger(L, ptr->GetAssignmentCount());
	return 1;
}

int assignmentsToString(lua_State * L)
{
	ConstructorOverloadPtr & ptr = assignmentsGetInstance(L, 1);
	std::stringstream ss;
	ss << ptr->GetNode()->GetName() << "::Ctor Assignment List ";
	ss << " Count = " << ptr->GetAssignmentCount();
	lua_pushlstring(L, ss.str().c_str(), ss.str().length());
	return 1;
}

static const struct luaL_Reg assignmentsMetaTableMethods[]=
{
	{"__eq", assignmentsEqual},
	{"__gc", assignmentsGc},
	{"__index", assignmentsIndex},
	{"__len", assignmentsLen},
	{"__tostring", assignmentsToString},
	{nullptr, nullptr}
};


int ConstructorOverloadLuaMetaData::AssignmentListOpenInLua(lua_State * L)
{
	luaL_getmetatable(L, "Macaroni.Model.Cpp.ConstructorOverload.Assignments");
	if (lua_isnil(L, -1) != 1)
	{
		return 0;
	}
	luaL_newmetatable(L, "Macaroni.Model.Cpp.ConstructorOverload.Assignments"); // create metaTable
	luaL_register(L, nullptr, assignmentsMetaTableMethods);

	return 1;
}

END_NAMESPACE

#endif
