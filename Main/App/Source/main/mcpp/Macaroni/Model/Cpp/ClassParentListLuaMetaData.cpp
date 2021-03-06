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
#ifndef MACARONI_MODEL_CPP_CLASSPARENTLUAMETADATA_CPP
#define MACARONI_MODEL_CPP_CLASSPARENTLUAMETADATA_CPP

#include "ClassParentListLuaMetaData.h"
#include <Macaroni/Model/Cpp/ClassLua.h>
#include <Macaroni/Model/Cpp/ClassParent.h>
#include <Macaroni/Model/Cpp/ClassParentLuaMetaData.h>
#include <Macaroni/Model/Node.h>
#include <Macaroni/Model/NodeLua.h>
#include <Macaroni/Model/NodeListLua.h>
#include <Macaroni/Model/ReasonLuaMetaData.h>
#include <sstream>

#define LUAGLUE_STARTNAMESPACE			BEGIN_NAMESPACE(Macaroni, Model, Cpp)
#define LUAGLUE_ENDNAMESPACE			END_NAMESPACE
#define LUAGLUE_CLASSNAME				ClassParentList
#define LUAGLUE_CLASSREFNAME			ClassParentListPtr
#define LUAGLUE_CLASSFULLLUANAME		"Macaroni.Model.Cpp.ClassParentList"
#define LUAGLUE_CLASSFULLCPPNAME		Macaroni::Model::Cpp::ClassParentList
#define LUAGLUE_REGISTRATIONCLASSNAME	ClassParentListLuaMetaData
#define LUAGLUE_OPENOTHERMODULES		/*Macaroni::Model::NodeLuaMetaData::OpenInLua(L); */
#define LUAGLUE_CREATEMETATABLE YESPLEASE
#define LUAGLUE_HELPERCLASS				ClassParentListLuaGlueHelperClass

#include <Macaroni/LuaGlue.hpp>

static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
				   const std::string & indexAsString)
{		
	unsigned int index = luaL_checkinteger(L, 2);
	if (index > 0 && index <= ptr->size())
	{
		index --;
		ClassParentPtr elementPtr(&((*ptr)[index]));
		ClassParentLuaMetaData::PutInstanceOnStack(L, elementPtr);
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

static int __tostring(lua_State * L)
{
	//TODO: Use -> LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
	getInstance(L);
	std::stringstream ss;
	ss << "ClassParentList";
	lua_pushstring(L, ss.str().c_str());
	return 1;
}	

#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
{"__len", LUAGLUE_HELPERCLASS::__len}, \
{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

#define LUAGLUE_ADDITIONALTABLEMETHODS /* None */

#include <Macaroni/LuaGlue2.hpp>

#endif