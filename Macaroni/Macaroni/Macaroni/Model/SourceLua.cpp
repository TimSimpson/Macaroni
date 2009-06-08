#ifndef MACARONI_MODEL_SOURCELUA_CPP
#define MACARONI_MODEL_SOURCELUA_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "SourceLua.h"
#include "Source.h"
#include "FileName.h"
#include "FileNameLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Source"
#define GLOBALTABLENAME "Macaroni.Model.Source"
namespace {

	static inline void createSourcePtrUserData(lua_State * L, const SourcePtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(SourcePtr));
		SourcePtr * instance = new (memory) SourcePtr();		
		(*instance).operator=(source);
	}

	static inline SourcePtr & getInstance(lua_State * L, int index)
	{
		SourcePtr * ptrToPtr = (SourcePtr *) luaL_checkudata(L, index, METATABLENAME);
		SourcePtr & ptr = dynamic_cast<SourcePtr &>(*ptrToPtr);
		return ptr;
	}


	static inline SourcePtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putSourceInstanceOnStack(lua_State * L, const SourcePtr & source)
	{
		createSourcePtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct SourceLuaFunctions
{
	static int luaGc(lua_State * L)
	{
		SourcePtr * ptr = (SourcePtr *) luaL_checkudata(L, 1, METATABLENAME);
		ptr->~SourcePtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		SourcePtr & a = getInstance(L, 1); 
		SourcePtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		SourcePtr & ptr = getInstance(L);
		std::string index(luaL_checkstring(L, 2));
		if (index == "FileName")
		{
			FileNameLuaMetaData::PutInstanceOnStack(L, ptr->GetFileName());
		}
		else if (index == "LineNumber")
		{
			lua_pushinteger(L, ptr->GetLineNumber());
		}
		else if (index == "ReferenceCount")
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
		SourcePtr & ptr = getInstance(L);
		std::string str = ptr->ToString();
		lua_pushlstring(L, str.c_str(), str.length());
		return 1;
	}

	static int Create(lua_State * L)
	{	
		FileNamePtr file = FileNameLuaMetaData::GetInstance(L, 1);
		int line = luaL_checkinteger(L, 2);
		SourcePtr ptr = Source::Create(file, line);
		putSourceInstanceOnStack(L, ptr);
		return 1;
	}
};

static const struct luaL_Reg tableMethods[]=
{
	{"Create", SourceLuaFunctions::Create},	
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", SourceLuaFunctions::__eq},
	{"__gc", SourceLuaFunctions::luaGc},
	{"__index", SourceLuaFunctions::__index},
	{"__tostring", SourceLuaFunctions::__tostring},
	{nullptr, nullptr}
};

SourcePtr SourceLuaMetaData::GetInstance(lua_State * L, int index)
{
	return getInstance(L, index);
}

bool SourceLuaMetaData::IsType(lua_State * L, int index)
{
	// Copied this from the luaL_checkudata function
	void * p = lua_touserdata(L, index);
	bool returnValue = false;
	if (p != nullptr) // is user data
	{
		// Compares metatable from user data to one in registry.
		if (lua_getmetatable(L, index))
		{
			lua_getfield(L, LUA_REGISTRYINDEX, METATABLENAME);
			if (lua_rawequal(L, -1, -2))
			{
				returnValue = true;
			}
			lua_pop(L, 2); // remove meta tables
		}
	}
	return returnValue;
}

int SourceLuaMetaData::OpenInLua(lua_State * L)
{	
	luaL_getmetatable(L, METATABLENAME);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);

	FileNameLuaMetaData::OpenInLua(L);
	
	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	return 1;
}

void SourceLuaMetaData::PutInstanceOnStack(lua_State * L, const SourcePtr & ptr)
{
	putSourceInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif
