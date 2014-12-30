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
#ifndef MACARONI_MODEL_CONTEXTLUA_CPP
#define MACARONI_MODEL_CONTEXTLUA_CPP

#include <Macaroni/Lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <Macaroni/Model/Context.h>
#include "ContextLua.h"
#include <Macaroni/Model/Project/Group.h>
#include <Macaroni/Model/Project/GroupPtr.h>
#include <Macaroni/Model/Project/GroupLuaMetaData.h>
#include "Node.h"
#include "NodeLua.h"
#include <Macaroni/Model/Project/ProjectVersionId.h>
#include <Macaroni/Model/Project/ProjectVersionLuaMetaData.h>
#include <Macaroni/Model/Project/ProjectVersion.h>
#include <sstream>
#include <Macaroni/LuaCompat.h>

using Macaroni::Model::Project::Group;
using Macaroni::Model::Project::GroupPtr;
using Macaroni::Model::Project::GroupLuaMetaData;
using Macaroni::Model::Project::ProjectVersionId;
using Macaroni::Model::Project::ProjectVersionLuaMetaData;
using Macaroni::Model::Project::ProjectVersionPtr;

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Context"
#define GLOBALTABLENAME "Macaroni.Model.Context"

#define L_BEGIN try {
#define L_END } catch(const std::exception & ex){ return luaL_error(L, ex.what()); }

namespace {

	// Taking a NamespacePtr, creates a Lua user data for a new NamespacePtr
	// controlled by Lua which is then put on top of the Lua Stack.
	// Note that this won't even be garbage collected as there's no table
	// associated with it.
	static inline void createContextPtrUserData(lua_State * L, const ContextPtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(ContextPtr));
		ContextPtr * instance = new (memory) ContextPtr();
		(*instance).operator=(source);
	}

	static inline ContextPtr & getInstance(lua_State * L, int index)
	{
		ContextPtr * ptrToPtr = (ContextPtr *) luaL_checkudata(L, index, METATABLENAME);
		ContextPtr & ptr = dynamic_cast<ContextPtr &>(*ptrToPtr);
		return ptr;
	}

	static inline ContextPtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putContextInstanceOnStack(lua_State * L, const ContextPtr & source)
	{
		createContextPtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2);
	}

} // End Anon namespace

struct ContextLuaFunctions
{
	static int findProjectVersion(lua_State * L)
	{
		L_BEGIN
		ContextPtr context = getInstance(L);
		std::string group(luaL_checkstring(L, 2));
		std::string name(luaL_checkstring(L, 3));
		std::string version(luaL_checkstring(L, 4));
		ProjectVersionId pvId(group, name, version);
		ProjectVersionPtr pv = context->FindProjectVersion(pvId);
		ProjectVersionLuaMetaData::PutInstanceOnStack(L, pv);
		return 1;
		L_END
	}


	// static int _library(lua_State * L)
	// {
	// 	L_BEGIN
	// 	ContextPtr context = getInstance(L);
	// 	std::string name(luaL_checkstring(L, 2));
	// 	if (!(lua_istable(L, 3)))
	// 	{
	// 		throw Macaroni::Exception("Lua table (array of FileSet) expected "
	// 								  .. "for argument #3.");
	// 	}
	// 	lua_gettable(L, 3);
	// 	std::vector<FileSetPtr> vec;

	// 	int index = 1;
	// 	lua_pushnil(L); // first key
	// 	const int tableIndex = -2;
	// 	while(lua_next(L, tableIndex)  != 0)
	// 	{
	// 		if (FileSetLuaMetaData::IsType(L, -1))
	// 		{
	// 			FileSetPtr fs = FileSetLuaMetaData::GetInstance(L, -1);
	// 			vec.push_back(fs);
	// 		} else {
	// 			string msg = str(format("Index %d of argument 3 is not a "
	// 			                        "FileSetPtr.") % index).c_str()
	// 			lua_error(msg.c_str());
	// 		}
	// 		lua_pop(state, 1); // pops off value, saves key
	// 		index ++;
	// 	}

	// 	return vec;
	// 	L_END
	// }

	static int _group(lua_State * L)
	{
		L_BEGIN
		ContextPtr context = getInstance(L);
		std::string name(luaL_checkstring(L, 2));
		GroupPtr group = context->Group(name);
		GroupLuaMetaData::PutInstanceOnStack(L, group);
		return 1;
		L_END
	}

	static int _node(lua_State * L)
	{
		L_BEGIN
		ContextPtr context = getInstance(L);
		std::string name(luaL_checkstring(L, 2));
		NodePtr node = context->Node(name);
		NodeLuaMetaData::PutInstanceOnStack(L, node);
		return 1;
		L_END
	}

	static int luaGc(lua_State * L)
	{
		L_BEGIN
		ContextPtr * nsPtr = (ContextPtr *) luaL_checkudata(L, 1, METATABLENAME);
		nsPtr->~ContextPtr();
		return 0;
		L_END
	}

	static int __eq(lua_State * L)
	{
		L_BEGIN
		ContextPtr & a = getInstance(L, 1);
		ContextPtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
		L_END
	}

	static int __index(lua_State * L)
	{
		L_BEGIN
		ContextPtr & context = getInstance(L);

		std::string index(luaL_checkstring(L, 2));

		if (index == "FindLibrary")
		{
			luaL_error(L, "FindLibrary is no longer supported. Please change "
				          "your code.");
		}
		else if (index == "FindOrCreateLibrary")
		{
			luaL_error(L, "FindOrCreateLibrary is no longer supported. "
				          "Please change your code.");
		}
		else if (index == "FindProjectVersion")
		{
			lua_pushcfunction(L, findProjectVersion);
		}
		else if (index == "GetReferenceCount")
		{
			lua_pushcfunction(L, GetReferenceCount);
		}
		else if (index == "Group")
		{
			lua_pushcfunction(L, _group);
		}
		else if (index == "Node")
		{
			lua_pushcfunction(L, _node);
		}
		else if (index == "Root")
		{
			NodeLuaMetaData::PutInstanceOnStack(L, context->GetRoot());
		}
		else if (index == "RootLibrary")
		{
			luaL_error(L, "RootLibrary is no longer supported. Please change "
				          "your code.");
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
		L_END
	}

	static int __tostring(lua_State * L)
	{
		L_BEGIN
		ContextPtr & context = getInstance(L);
		std::stringstream ss;
		ss << "Context[references:" << context->GetReferenceCount()
			<< ",Root:"
			<< (context->GetRoot() != false
				? context->GetRoot()->GetName()
				: "nullptr")
			<< "]";
		lua_pushlstring(L, ss.str().c_str(), ss.str().length());
		return 1;
		L_END
	}

	static int New(lua_State * L)
	{
		L_BEGIN
		std::string rootName(luaL_checkstring(L, 1));

		ContextPtr ptr = new Context(rootName);
		putContextInstanceOnStack(L, ptr);
		return 1;
		L_END
	}

	static int GetReferenceCount(lua_State * L)
	{
		L_BEGIN
		ContextPtr & context = getInstance(L);
		lua_pushinteger(L, context->GetReferenceCount());
		return 1;
		L_END
	}

};

static const struct luaL_Reg tableMethods[]=
{
	{"New", ContextLuaFunctions::New},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", ContextLuaFunctions::__eq},
	{"__gc", ContextLuaFunctions::luaGc},
	{"__index", ContextLuaFunctions::__index},
	{"__tostring", ContextLuaFunctions::__tostring},
	{nullptr, nullptr}
};

ContextPtr & ContextLuaMetaData::GetInstance(lua_State * L, int index)
{
	return getInstance(L, index);
}

int ContextLuaMetaData::OpenInLua(lua_State * L)
{
	luaL_getmetatable(L, METATABLENAME);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_setfuncs(L, metaTableMethods, 0);

	//lua_newtable(L);	// create __indexTable
	//luaL_register(L, nullptr, __indexTableMethods);
	//lua_setfield(L, -2, "__index"); // metaTable.__index = __indexTable;
	//lua_pushvalue(L, -1); // we duplicate __index table...
	//lua_setfield(L, -2, "__index"); // __indexTable.__index = __indexTable;
	// The second reference to __indexTable is off the stack now.

	/*
	// Duplicate the meta table to create the indexing metatable, assign it
	// to the __index of the first metatable, then register the instance
	// methods.  Pg 266 of the Lua book.
	lua_pushvalue(L, -1); // duplicates the metatable
	lua_setfield(L, -2, "__index"); // set stack[-2].__index = stack[-1]
	luaL_register(L, nullptr, metaTableMethods);
	*/


	// Creates or reuses a table called "Macaroni_File" and puts it in global
	// scope.
	MACARONI_LUA_REGISTER_FOR_RETURN(L, GLOBALTABLENAME, tableMethods);

	///** Now open dependent libraries. */
	//NodeLuaMetaData::OpenInLua(L);

	return 1;
}

void ContextLuaMetaData::PutInstanceOnStack(lua_State * L, const ContextPtr & ptr)
{
	putContextInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif
