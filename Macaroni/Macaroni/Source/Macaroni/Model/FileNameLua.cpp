#ifndef MACARONI_MODEL_FILENAMELUA_CPP
#define MACARONI_MODEL_FILENAMELUA_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "FileNameLua.h"
#include "FileName.h"
#include "../Environment/DebugLog.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME_FN "Macaroni.Model.FileName"
#define GLOBALTABLENAME_FN "Macaroni.Model.FileName"
namespace {

	static inline void createFileNamePtrUserData(lua_State * L, const FileNamePtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(FileNamePtr));
		FileNamePtr * instance = new (memory) FileNamePtr();		
		(*instance).operator=(source);
	}

	static inline FileNamePtr & getInstance(lua_State * L, int index)
	{
		FileNamePtr * ptrToPtr = (FileNamePtr *) luaL_checkudata(L, index, METATABLENAME_FN);
		FileNamePtr & ptr = dynamic_cast<FileNamePtr &>(*ptrToPtr);
		return ptr;
	}


	static inline FileNamePtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putFileNameInstanceOnStack(lua_State * L, const FileNamePtr & source)
	{
		createFileNamePtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME_FN);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct FileNameLuaFunctions
{
	static int luaGc(lua_State * L)
	{
		FileNamePtr * ptr = (FileNamePtr *) luaL_checkudata(L, 1, METATABLENAME_FN);
		ptr->~FileNamePtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		FileNamePtr & a = getInstance(L, 1); 
		FileNamePtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		FileNamePtr & ptr = getInstance(L);
		std::string index(luaL_checkstring(L, 2));
		if (index == "ReferenceCount")
		{
			lua_pushinteger(L, ptr->GetReferenceCount());
		}
		else 
		{
			lua_pushnil(L);			
		}
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		FileNamePtr & ptr = getInstance(L);
		std::string fullName = ptr->GetName();
		lua_pushlstring(L, fullName.c_str(), fullName.length());
		return 1;
	}

	static int Create(lua_State * L)
	{
		std::string name(luaL_checkstring(L, 1));
		FileNamePtr ptr = FileName::Create(name);
		putFileNameInstanceOnStack(L, ptr);
		return 1;
	}

};

static const struct luaL_Reg tableMethods[]=
{
	{"Create", FileNameLuaFunctions::Create},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", FileNameLuaFunctions::__eq},
	{"__gc", FileNameLuaFunctions::luaGc},
	{"__index", FileNameLuaFunctions::__index},
	{"__tostring", FileNameLuaFunctions::__tostring},
	{nullptr, nullptr}
};

FileNamePtr & FileNameLuaMetaData::GetInstance(lua_State * L, int index)
{
	return getInstance(L, index);	
}

bool FileNameLuaMetaData::IsType(lua_State * L, int index)
{
	// Copied this from the luaL_checkudata function
	void * p = lua_touserdata(L, index);
	bool returnValue = false;
	if (p != nullptr) // is user data
	{
		// Compares metatable from user data to one in registry.
		if (lua_getmetatable(L, index))
		{
			lua_getfield(L, LUA_REGISTRYINDEX, METATABLENAME_FN);
			if (lua_rawequal(L, -1, -2))
			{
				returnValue = true;
			}
			lua_pop(L, 2); // remove meta tables
		}
	}
	return returnValue;
}

int FileNameLuaMetaData::OpenInLua(lua_State * L)
{	
	DEBUGLOG_WRITE("OpenInLua begins...");

	luaL_getmetatable(L, METATABLENAME_FN);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, METATABLENAME_FN); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);

	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME_FN, tableMethods);

	DEBUGLOG_WRITE("... open in lua ends.");
	return 1;
}

void FileNameLuaMetaData::PutInstanceOnStack(lua_State * L, const FileNamePtr & ptr)
{
	putFileNameInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif
