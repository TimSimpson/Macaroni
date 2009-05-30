#ifndef MACARONI_MODEL_TYPELUAREGISTER_CPP
#define MACARONI_MODEL_TYPELUAREGISTER_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}

#include "Scope.h"
#include "ScopeLua.h"
#include "Type.h"
#include "TypeLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Type"
#define GLOBALTABLENAME "Macaroni.Model.Type"

namespace {

	static inline void createTypePtrUserData(lua_State * L, const TypePtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(TypePtr));
		TypePtr * instance = new (memory) TypePtr();		
		(*instance).operator=(source);
	}

	static inline TypePtr & getInstance(lua_State * L, int index)
	{
		TypePtr * ptrToPtr = (TypePtr *) luaL_checkudata(L, index, METATABLENAME);
		TypePtr & ptr = dynamic_cast<TypePtr &>(*ptrToPtr);
		return ptr;
	}

	static inline TypePtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putTypeInstanceOnStack(lua_State * L, const TypePtr & source)
	{
		createTypePtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct TypeLuaFunctions
{
	static int luaGc(lua_State * L)
	{
		TypePtr * ptr = (TypePtr *) luaL_checkudata(L, 1, METATABLENAME);
		ptr->~TypePtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		TypePtr & a = getInstance(L, 1); 
		TypePtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		TypePtr & ptr = getInstance(L);
		std::string index(luaL_checkstring(L, 2));		
		return TypeLuaMetaData::Index(L, ptr, index);
	}

	static int __tostring(lua_State * L)
	{
		TypePtr & ptr = getInstance(L);	
		std::string typeName = ptr->GetTypeName();
		lua_pushlstring(L, typeName.c_str(), typeName.length());
		return 1;
	}

};

static const struct luaL_Reg tableMethods[]=
{
	//{"FindOrCreate", TypeLuaFunctions::FindOrCreate},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", TypeLuaFunctions::__eq},
	{"__gc", TypeLuaFunctions::luaGc},
	{"__index", TypeLuaFunctions::__index},
	{"__tostring", TypeLuaFunctions::__tostring},
	{nullptr, nullptr}
};

int TypeLuaMetaData::Index(lua_State * L, 
								  TypePtr & ptr, 
								  const std::string & index)
{
	if (index == "TypeName")
	{
		const std::string & typeName = ptr->GetTypeName();
		lua_pushlstring(L, typeName.c_str(), typeName.size());	
	}		
	else if (index == "TypeScope")
	{
		ScopePtr scopePtr = ptr->GetTypeScope();
		ScopeLuaMetaData::PutInstanceOnStack(L, scopePtr);
		return 1;
	}
	else
	{
		lua_pushnil(L);
	}		
	return 1;
}

int TypeLuaMetaData::OpenInLua(lua_State * L)
{	
	luaL_getmetatable(L, METATABLENAME);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);

	ScopeLuaMetaData::OpenInLua(L);
	

	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	return 1;
}

void TypeLuaMetaData::PutInstanceOnStack(lua_State * L, const TypePtr & ptr)
{
	putTypeInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif
