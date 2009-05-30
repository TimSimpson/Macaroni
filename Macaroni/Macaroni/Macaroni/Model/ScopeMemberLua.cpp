#ifndef MACARONI_MODEL_SCOPEMEMBERLUAREGISTER_CPP
#define MACARONI_MODEL_SCOPEMEMBERLUAREGISTER_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}

#include "Node.h"
#include "NodeLua.h"
#include "ScopeMember.h"
#include "ScopeMemberLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.ScopeMember"
#define GLOBALTABLENAME "Macaroni.Model.ScopeMember"

namespace {

	static inline void createScopeMemberPtrUserData(lua_State * L, const ScopeMemberPtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(ScopeMemberPtr));
		ScopeMemberPtr * instance = new (memory) ScopeMemberPtr();		
		(*instance).operator=(source);
	}

	static inline ScopeMemberPtr & getInstance(lua_State * L, int index)
	{
		ScopeMemberPtr * ptrToPtr = (ScopeMemberPtr *) luaL_checkudata(L, index, METATABLENAME);
		ScopeMemberPtr & ptr = dynamic_cast<ScopeMemberPtr &>(*ptrToPtr);
		return ptr;
	}

	static inline ScopeMemberPtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putScopeMemberInstanceOnStack(lua_State * L, const ScopeMemberPtr & source)
	{
		createScopeMemberPtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct ScopeMemberLuaFunctions
{
	static int luaGc(lua_State * L)
	{
		ScopeMemberPtr * ptr = (ScopeMemberPtr *) luaL_checkudata(L, 1, METATABLENAME);
		ptr->~ScopeMemberPtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		ScopeMemberPtr & a = getInstance(L, 1); 
		ScopeMemberPtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		ScopeMemberPtr & ptr = getInstance(L);
		std::string index(luaL_checkstring(L, 2));		
		return ScopeMemberLuaMetaData::Index(L, ptr, index);
	}

	static int __tostring(lua_State * L)
	{
		ScopeMemberPtr & ptr = getInstance(L);	
		std::string fullName = ptr->GetFullName();
		lua_pushlstring(L, fullName.c_str(), fullName.length());
		return 1;
	}

};

static const struct luaL_Reg tableMethods[]=
{
	//{"FindOrCreate", ScopeMemberLuaFunctions::FindOrCreate},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", ScopeMemberLuaFunctions::__eq},
	{"__gc", ScopeMemberLuaFunctions::luaGc},
	{"__index", ScopeMemberLuaFunctions::__index},
	{"__tostring", ScopeMemberLuaFunctions::__tostring},
	{nullptr, nullptr}
};

int ScopeMemberLuaMetaData::Index(lua_State * L, 
								  ScopeMemberPtr & ptr, 
								  const std::string & index)
{
	if (index == "FullName")
	{
		lua_pushlstring(L, ptr->GetFullName().c_str(), ptr->GetFullName().size());	
	}		
	else if (index == "Name")
	{
		lua_pushlstring(L, ptr->GetName().c_str(), ptr->GetName().length());
	}
	else if (index == "Node")
	{
		NodePtr scope = ptr->GetNode();
		NodeLuaMetaData::PutInstanceOnStack(L, scope);
	}
	else
	{
		lua_pushnil(L);
	}		
	return 1;
}

int ScopeMemberLuaMetaData::OpenInLua(lua_State * L)
{	
	luaL_getmetatable(L, METATABLENAME);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);

	ScopeMemberLuaMetaData::OpenInLua(L);
	
	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	return 1;
}

void ScopeMemberLuaMetaData::PutInstanceOnStack(lua_State * L, const ScopeMemberPtr & ptr)
{
	if (boost::dynamic_pointer_cast<Node>(ptr) != nullptr)
	{
		NodeLuaMetaData::PutInstanceOnStack(L, boost::dynamic_pointer_cast<Node>(ptr));
	}
	else
	{
		putScopeMemberInstanceOnStack(L, ptr);
	}
	
}

END_NAMESPACE2

#endif
