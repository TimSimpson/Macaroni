#ifndef MACARONI_MODEL_LIBRARYLUAREGISTER_CPP
#define MACARONI_MODEL_LIBRARYLUAREGISTER_CPP

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "ContextLua.h"
#include "../Exception.h"
#include "../Environment/DebugLog.h"
#include <boost/foreach.hpp>
#include <Macaroni/Model/FileNameLuaMetaData.h>
#include "Library.h"
#include "LibraryLua.h"
#include "Node.h"
#include "NodeLua.h"
#include "NodeListLua.h"
#include <Macaroni/IO/PathLua.h>
#include <sstream>

using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;
using Macaroni::IO::PathLuaMetaData;

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Library"
#define GLOBALTABLENAME "Macaroni.Model.Library"

namespace {

	static inline void createLibraryPtrUserData(lua_State * L, const LibraryPtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(LibraryPtr));
		LibraryPtr * instance = new (memory) LibraryPtr();		
		(*instance).operator=(source);
	}

	static inline LibraryPtr & getInstance(lua_State * L, int index)
	{
		LibraryPtr * ptrToPtr = (LibraryPtr *) luaL_checkudata(L, index, METATABLENAME);
		LibraryPtr & ptr = dynamic_cast<LibraryPtr &>(*ptrToPtr);
		return ptr;
	}
	
	static inline LibraryPtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putLibraryInstanceOnStack(lua_State * L, const LibraryPtr & source)
	{
		if (!source) 
		{
			lua_pushnil(L);
		}
		else
		{
			createLibraryPtrUserData(L, source);
			luaL_getmetatable(L, METATABLENAME);
			lua_setmetatable(L, -2); 
		}
	}

} // End Anon namespace

struct LibraryLuaFunctions
{
	static int luaGc(lua_State * L)
	{
		LibraryPtr * ptr = (LibraryPtr *) luaL_checkudata(L, 1, METATABLENAME);
		ptr->~LibraryPtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		LibraryPtr & a = getInstance(L, 1); 
		LibraryPtr & b = getInstance(L, 2);
		if (!a && !b) 
		{
			lua_pushboolean(L, 1);
		}
		else if (!a || !b)
		{
			lua_pushboolean(L, 0);
		}
		else
		{
			lua_pushboolean(L, a->GetId() == b->GetId() ? 1 : 0);
		}
		return 1;
	}

	static int findInstallPath(lua_State * L)
	{
		try
		{
			LibraryPtr & ptr = getInstance(L, 1); 
			PathPtr path = ptr->FindInstallPath();
			PathLuaMetaData::PutInstanceOnStack(L, path);
			return 1;
		} 
		catch(const std::exception & ex)
		{
			lua_pushstring(L, ex.what());
			return lua_error(L);			
		}
	}

	static int getCId(lua_State * L)
	{
		try
		{
			LibraryPtr & ptr = getInstance(L, 1); 
			std::string cid = ptr->GetCId();
			lua_pushstring(L, cid.c_str());
			return 1;
		} 
		catch(const std::exception & ex)
		{
			lua_pushstring(L, ex.what());
			return lua_error(L);			
		}
	}

	static int __index(lua_State * L)
	{
		LibraryPtr & ptr = getInstance(L);
		std::string index(luaL_checkstring(L, 2));
		return LibraryLuaMetaData::Index(L, ptr, index);
	}

	static int __tostring(lua_State * L)
	{
		LibraryPtr ptr = getInstance(L);
		std::stringstream ss;
		ss << ptr->GetName() << ", version " << ptr->GetVersion();
		lua_pushlstring(L, ss.str().c_str(), ss.str().length());
		return 1;
	}
	
};

static const struct luaL_Reg tableMethods[]=
{	
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", LibraryLuaFunctions::__eq},
	{"__gc", LibraryLuaFunctions::luaGc},
	{"__index", LibraryLuaFunctions::__index},
	{"__tostring", LibraryLuaFunctions::__tostring},
	{nullptr, nullptr}
};

int LibraryLuaMetaData::Index(lua_State * L, LibraryPtr & ptr, const std::string & index)
{	
	if (index == "Context")
	{
		ContextLuaMetaData::PutInstanceOnStack(L, ptr->GetContext());
		return 1;
	}
	else if (index == "Dependencies")
	{
		lua_newtable(L);
		int index = 1;
		BOOST_FOREACH(LibraryPtr dep, ptr->GetDependencies())
		{
			lua_pushinteger(L, index);
			putLibraryInstanceOnStack(L, dep);						
			lua_settable(L, -3);
			index ++;
		}
		return 1;
	}
	else if (index == "FindInstallPath")
	{
		lua_pushcfunction(L, LibraryLuaFunctions::findInstallPath);
		return 1;
	}
	else if (index == "GetCId")
	{
		lua_pushcfunction(L, LibraryLuaFunctions::getCId);
		return 1;
	}
	else if (index == "Group") 
	{
		lua_pushstring(L, ptr->GetGroup().c_str());
		return 1;
	}
	else if (index == "Installed")
	{
		lua_pushboolean(L, ptr->IsInstalled());
		return 1;
	}
	else if (index == "Name") 
	{
		lua_pushstring(L, ptr->GetName().c_str());
		return 1;
	}
	else if (index == "Version")
	{
		lua_pushstring(L, ptr->GetVersion().c_str());
		return 1;
	}	
	else
	{
		lua_pushnil(L);
		return 1;
	}				
}

LibraryPtr & LibraryLuaMetaData::GetInstance(lua_State * L, int index)
{
	return getInstance(L, index);
}

bool LibraryLuaMetaData::IsType(lua_State * L, int index, const char * metaTableName)
{
	// Copied this from the luaL_checkudata function
	void * p = lua_touserdata(L, index);
	bool returnValue = false;
	if (p != nullptr) // is user data
	{
		// Compares metatable from user data to one in registry.
		if (lua_getmetatable(L, index))
		{
			lua_getfield(L, LUA_REGISTRYINDEX, metaTableName);
			if (lua_rawequal(L, -1, -2))
			{
				returnValue = true;
			}
			lua_pop(L, 2); // remove meta tables
		}
	}
	return returnValue;
}

bool LibraryLuaMetaData::IsType(lua_State * L, int index)
{
	return IsType(L, index, METATABLENAME);
}

int LibraryLuaMetaData::OpenInLua(lua_State * L)
{		
	luaL_getmetatable(L, METATABLENAME);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}

	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);
	luaL_register(L, GLOBALTABLENAME, tableMethods);

	return 1;
}

void LibraryLuaMetaData::PutInstanceOnStack(lua_State * L, const LibraryPtr & ptr)
{	
	putLibraryInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif
