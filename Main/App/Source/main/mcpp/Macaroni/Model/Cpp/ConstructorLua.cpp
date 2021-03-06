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
#ifndef MACARONI_MODEL_CPP_CONSTRUCTORLUA_CPP
#define MACARONI_MODEL_CPP_CONSTRUCTORLUA_CPP

#include "../NodeLua.h"
#include "ConstructorLua.h"
#include "../ElementLua.h"
#include "FunctionLua.h"
#include "../NodeListLua.h"
#include "TypeInfoLua.h"
#include "VariableLua.h"
#include <Macaroni/Model/Cpp/VariableAssignmentLuaMetaData.h>

#include <Macaroni/Lua.h>

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int ConstructorLuaMetaData::Index(lua_State * L, const ConstructorPtr & ptr,
								  const std::string & index)
{
	return 0;
}

//
//ConstructorPtr & assignmentsGetInstance(lua_State * L, int position)
//{
//	ConstructorPtr * ptr = (ConstructorPtr *) luaL_checkudata(L, position, "Macaroni.Model.Cpp.Constructor.Assignments");
//	return *ptr;
//}
//
//int assignmentsEqual(lua_State * L)
//{
//	ConstructorPtr & ptr1 = assignmentsGetInstance(L, 1);
//	ConstructorPtr & ptr2 = assignmentsGetInstance(L, 2);
//	lua_pushboolean(L, ptr1.get() == ptr2.get());
//	return 1;
//}
//
//int assignmentsGc(lua_State * L)
//{
//	ConstructorPtr & ptr = assignmentsGetInstance(L, 1);
//	ptr.~ConstructorPtr();
//	return 1;
//}
//
//int assignmentsIndex(lua_State * L)
//{
//	ConstructorPtr & ptr = assignmentsGetInstance(L, 1);
//	int index = luaL_checkinteger(L, 2);
//	index --;
//	VariableAssignmentPtr luaData(new VariableAssignment(ptr->GetAssignment(index)));
//	Macaroni::Model::Cpp::VariableAssignmentLuaMetaData::PutInstanceOnStack(L, luaData);
//	return 1;
//}
//
//int assignmentsLen(lua_State * L)
//{
//	ConstructorPtr & ptr = assignmentsGetInstance(L, 1);
//	lua_pushinteger(L, ptr->GetAssignmentCount());
//	return 1;
//}
//
//int assignmentsToString(lua_State * L)
//{
//	ConstructorPtr & ptr = assignmentsGetInstance(L, 1);
//	std::stringstream ss;
//	ss << ptr->GetNode()->GetName() << "::Ctor Assignment List ";
//	ss << " Count = " << ptr->GetAssignmentCount();
//	lua_pushlstring(L, ss.str().c_str(), ss.str().length());
//	return 1;
//}

//
//static const struct luaL_Reg assignmentsMetaTableMethods[]=
//{
//	{"__eq", assignmentsEqual},
//	{"__gc", assignmentsGc},
//	{"__index", assignmentsIndex},
//	{"__len", assignmentsLen},
//	{"__tostring", assignmentsToString},
//	{nullptr, nullptr}
//};
//
//int ConstructorLuaMetaData::AssignmentListOpenInLua(lua_State * L)
//{
//	luaL_getmetatable(L, "Macaroni.Model.Cpp.Constructor.Assignments");
//	if (lua_isnil(L, -1) != 1)
//	{
//		return 0;
//	}
//	luaL_newmetatable(L, "Macaroni.Model.Cpp.Constructor.Assignments"); // create metaTable
//	luaL_register(L, nullptr, assignmentsMetaTableMethods);
//
//	return 1;
//}
END_NAMESPACE


#endif
