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
#ifndef MACARONI_IO_PATHLUA_CPP
#define MACARONI_IO_PATHLUA_CPP

#include <Macaroni/IO/GeneratedFileWriterLuaMetaData.h>
#include "PathLua.h"

//extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
//}


struct lua_State;

#define PATHSMETATABLENAME "Macaroni.IO.PathList"

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, IO)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME Path
#define LUAGLUE_CLASSREFNAME PathPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.IO.Path"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::IO::Path
#define LUAGLUE_REGISTRATIONCLASSNAME PathLuaMetaData
#define LUAGLUE_HELPERCLASS PathsLuaFunctions
#define LUAGLUE_OPENOTHERMODULES  \
		luaL_newmetatable(L, "Macaroni.IO.PathList"); \
		luaL_register(L, nullptr, PathsProperty_MetaTableMethods); \
		luaL_register(L, "Macaroni.IO.PathList", PathsProperty_GlobalTableMethods);

//#define LUAGLUE_OPENOTHERMODULES /*Macaroni::Generator::Output::PathLuaMetaData::OpenInLua(L); \
//		luaL_newmetatable(L, "Macaroni.Generator.Output.Path.PathList"); \
//		luaL_register(L, nullptr, PathsProperty_MetaTableMethods);*/

#define LUAGLUE_CREATEMETATABLE


BEGIN_NAMESPACE2(Macaroni, IO)

namespace {

	PathListPtr & getPathListPtrFromStack(lua_State * L, int index)
	{
		PathListPtr * ptrToPtr = (PathListPtr *) luaL_checkudata(L, index, PATHSMETATABLENAME);
		PathListPtr & ptr = dynamic_cast<PathListPtr &>(*ptrToPtr);
		return ptr;
	}

	void pushPathListPtrOntoStack(lua_State * L, PathListPtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(PathListPtr));
		PathListPtr * instance = new (memory) PathListPtr();
		(*instance).operator= (source);

		luaL_getmetatable(L, PATHSMETATABLENAME);
		lua_setmetatable(L, -2);
	}

	int paths__gc(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, 1);
		paths.~PathListPtr();
		return 0;
	}

	int paths__index(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, 1);
		unsigned int index = luaL_checkinteger(L, 2);
		index --; // Remeber that lua is indexed by 1.
		if (index >= 0 && index < paths->size())
		{
			PathPtr & element = paths->at(index);
			PathLuaMetaData::PutInstanceOnStack(L, element);
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int paths__newindex(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, 1);
		unsigned int index = luaL_checkinteger(L, 2);
		// Remember that lua is indexed by 1.
		if (index > (paths->size() + 1) || index < 1)
		{
			return luaL_error(L, "Index out of bounds.");
		}
		PathPtr newPath = PathLuaMetaData::GetInstance(L, 3);
		if (index == (paths->size() + 1))
		{
			paths->push_back(newPath);
		}
		else
		{
			(*paths)[index - 1] = newPath;
		}
		return 0;
	}


	static int paths__len(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, 1);
		lua_pushinteger(L, paths->size());
		return 1;
	}

	static int paths__tostring(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, 1);
		lua_pushstring(L, "Paths Array");
		return 1;
	}

	static int paths__new(lua_State * L)
	{
		PathListPtr paths(new PathList());
		pushPathListPtrOntoStack(L, paths);
		return 1;
	}

	static const struct luaL_Reg PathsProperty_MetaTableMethods[]=
	{
		{"__gc", paths__gc},
		{"__index", paths__index},
		{"__len", paths__len},
		{"__newindex", paths__newindex},
		{"__tostring", paths__tostring},
		{nullptr, nullptr}
	};

	static const struct luaL_Reg PathsProperty_GlobalTableMethods[]=
	{
		{"New", paths__new},
		{nullptr, nullptr}
	};
} // end anon namespace

END_NAMESPACE2

#define TRY try {
#define CATCH } catch(const std::exception & ex){ return luaL_error(L, ex.what()); }

#include "../LuaGlue.hpp"

	static int copyToDifferentRootPath(lua_State * L)
	{
		TRY
			PathPtr me = getInstance(L, 1);
			PathPtr rootPath = getInstance(L, 2);
			bool ovride = false;
			if (lua_isboolean(L, 3))
			{
				ovride = (lua_toboolean(L, 3) == 1);
			}
			me->CopyToDifferentRootPath(rootPath, ovride);
			return 0;
		CATCH
	}

	static int createDirectory(lua_State * L)
	{
		TRY
			PathPtr ptr = getInstance(L);
			ptr->CreateDirectory();
			return 0;
		CATCH
	}

	static int createWithCurrentAsRoot(lua_State * L)
	{
		TRY
			PathPtr ptr = getInstance(L);
			PathPtr rtn = ptr->CreateWithCurrentAsRoot();
			PathLuaMetaData::PutInstanceOnStack(L, rtn);
			return 1;
		CATCH
	}

	static int createWithDifferentRootPath(lua_State * L)
	{
		TRY
			PathPtr ptr = getInstance(L);
			PathPtr other = PathLuaMetaData::GetInstance(L, 2);
			PathPtr rtn = ptr->CreateWithDifferentRootPath(other);
			PathLuaMetaData::PutInstanceOnStack(L, rtn);
			return 1;
		CATCH
	}

	static int getPaths(lua_State * L)
	{
		TRY
			PathPtr path = getInstance(L);
			std::string matchesPattern(luaL_checkstring(L, 2));
			PathListPtr paths = path->GetPaths(matchesPattern);
			pushPathListPtrOntoStack(L, paths);
			return 1;
		CATCH
	}

	static int isFileOlderThan(lua_State * L)
	{
		TRY
			PathPtr ptr = getInstance(L);
			std::string other(luaL_checkstring(L, 2));
			bool result = ptr->IsFileOlderThan(other);
			lua_pushboolean(L, result);
			return 1;
		CATCH
	}

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr,
									  const std::string & index)
	{
		TRY
		if (index == "AbsolutePath")
		{
			std::string absPath = ptr->GetAbsolutePath();
			lua_pushstring(L, absPath.c_str());
			return 1;
		}
		if (index == "AbsolutePathForceSlash")
		{
			std::string absPath = ptr->GetAbsolutePathForceSlash();
			lua_pushstring(L, absPath.c_str());
			return 1;
		}
		else if (index == "CopyToDifferentRootPath")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::copyToDifferentRootPath);
			return 1;
		}
		else if (index == "ClearDirectoryContents")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::clearDirectoryContents);
			return 1;
		}
		else if (index == "CreateDirectory")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::createDirectory);
			return 1;
		}
		else if (index == "CreateFile")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::createFile);
			return 1;
		}
		else if (index == "CreateWithCurrentAsRoot")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::createWithCurrentAsRoot);
			return 1;
		}
		else if (index == "CreateWithDifferentRootPath")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::createWithDifferentRootPath);
			return 1;
		}
		else if (index == "DifferentSuffix")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::differentSuffix);
			return 1;
		}
		else if (index == "Exists")
		{	bool exists = ptr->Exists();
			lua_pushboolean(L, exists);
			return 1;
		}
		else if (index == "GetAbsolutePathForceSlash")
		{
			std::string absPath = ptr->GetAbsolutePathForceSlash();
			lua_pushstring(L, absPath.c_str());
			return 1;
		}
		else if (index == "FileName")
		{
			std::string fileName = ptr->GetFileName();
			lua_pushstring(L, fileName.c_str());
			return 1;
		}
		else if (index == "FileNameWithoutExtension")
		{
			std::string fileName = ptr->GetFileNameWithoutExtension();
			lua_pushstring(L, fileName.c_str());
			return 1;
		}
		else if (index == "GetPaths")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::getPaths);
			return 1;
		}
		else if (index == "IsDirectory")
		{
			bool isDirectory = ptr->IsDirectory();
			lua_pushboolean(L, isDirectory);
			return 1;
		}
		else if (index == "IsFileOlderThan")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::isFileOlderThan);
			return 1;
		}
		else if (index == "NewPath")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::newPath);
			return 1;
		}
		else if (index == "NewPathForceSlash")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::newPathForceSlash);
			return 1;
		}
		else if (index == "ParentPath")
		{
			PathPtr parent = ptr->GetParentPath();
			PathLuaMetaData::PutInstanceOnStack(L, parent);
			return 1;
		}
		else if (index == "Paths")
		{
			PathListPtr paths = ptr->GetPaths();
			pushPathListPtrOntoStack(L, paths);
			return 1;
		}
		else if (index == "RelativePath")
		{
			std::string relativePath = ptr->GetRelativePath();
			lua_pushstring(L, relativePath.c_str());
			return 1;
		}
		else if (index == "RenameRelative")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::renameRelative);
			return 1;
		}
		lua_pushnil(L);
		return 1;
		CATCH
	}

	static int clearDirectoryContents(lua_State * L)
	{
		TRY
			PathPtr ptr = getInstance(L);
			ptr->ClearDirectoryContents();
			return 0;
		CATCH
	}

	static int copyDirectoryContents(lua_State * L)
	{
		TRY
			PathPtr src = PathLuaMetaData::GetInstance(L, 1);
			PathPtr dst = PathLuaMetaData::GetInstance(L, 2);
			Path::CopyDirectoryContents(*src, *dst);
			return 0;
		CATCH
	}

	static int createFile(lua_State * L)
	{
		TRY
			PathPtr ptr = getInstance(L);
			GeneratedFileWriterPtr writer = ptr->CreateFile();
			GeneratedFileWriterLuaMetaData::PutInstanceOnStack(L, writer);
			return 1;
		CATCH
	}

	static int differentSuffix(lua_State * L)
	{
		TRY
			PathPtr ptr = getInstance(L);
			std::string suffix = luaL_checkstring(L, 2);
			PathPtr result = ptr->DifferentSuffix(suffix);
			PathLuaMetaData::PutInstanceOnStack(L, result);
			return 1;
		CATCH
	}

	static int newInstance(lua_State * L)
	{
		TRY
			std::string absolutePath(luaL_checkstring(L, 1));
			PathPtr newPath;
			if (lua_gettop(L) < 2)
			{
				newPath.reset(new Path(absolutePath));
			}
			else
			{
				std::string relativePath(luaL_checkstring(L, 2));
				boost::filesystem::path p1 = absolutePath;
				boost::filesystem::path p2 = relativePath;
				newPath.reset(new Path(p1, p2));
			}
			LUAGLUE_REGISTRATIONCLASSNAME::PutInstanceOnStack(L, newPath);
			return 1;
		CATCH
	}


	static int newPath(lua_State * L)
	{
		TRY
			PathPtr ptr = getInstance(L);
			std::string name(luaL_checkstring(L, 2));
			PathPtr newPath = ptr->NewPath(name);
			LUAGLUE_REGISTRATIONCLASSNAME::PutInstanceOnStack(L, newPath);
			return 1;
		CATCH
	}

	static int newPathForceSlash(lua_State * L)
	{
		TRY
			PathPtr ptr = getInstance(L);
			std::string name(luaL_checkstring(L, 2));
			PathPtr newPath = ptr->NewPathForceSlash(name);
			LUAGLUE_REGISTRATIONCLASSNAME::PutInstanceOnStack(L, newPath);
			return 1;
		CATCH
	}

	static int renameRelative(lua_State * L)
	{
		TRY
			PathPtr ptr = getInstance(L);
			std::string newRelative(luaL_checkstring(L, 2));
			ptr->RenameRelative(newRelative);
			return 0;
		CATCH
	}

	static int __tostring(lua_State * L)
	{
		TRY
			PathPtr path = getInstance(L);
			lua_pushstring(L, path->ToString().c_str());
			return 1;
		CATCH
	}

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring},

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"CopyDirectoryContents", LUAGLUE_HELPERCLASS::copyDirectoryContents}, \
		{"New", LUAGLUE_HELPERCLASS::newInstance},

#include "../LuaGlue2.hpp"

BEGIN_NAMESPACE2(Macaroni, IO)

bool PathListLuaMetaData::IsType(lua_State * L, int index)
{
	void * p = lua_touserdata(L, index);
	bool returnValue = false;
	if (p != nullptr) // is user data
	{
		// Compares metatable from user data to one in registry.
		if (lua_getmetatable(L, index))
		{
			lua_getfield(L, LUA_REGISTRYINDEX, PATHSMETATABLENAME);
			if (lua_rawequal(L, -1, -2))
			{
				returnValue = true;
			}
			lua_pop(L, 2); // remove meta tables
		}
	}
	return returnValue;
}

int PathListLuaMetaData::OpenInLua(lua_State * L)
{
	return PathLuaMetaData::OpenInLua(L);
}

PathListPtr & PathListLuaMetaData::GetInstance(lua_State * L, int index)
{
	return getPathListPtrFromStack(L, index);
}

void PathListLuaMetaData::PutInstanceOnStack(lua_State * L,
                                             PathListPtr & ptr)
{
	return pushPathListPtrOntoStack(L, ptr);
}

END_NAMESPACE2

#endif
