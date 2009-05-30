#ifndef MACARONI_MODEL_CLASSLUAREGISTER_CPP
#define MACARONI_MODEL_CLASSLUAREGISTER_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "Class.h"
#include "ClassLua.h"
#include "NamespaceLua.h"
#include "ScopeLua.h"
#include "Type.h"
#include "TypeLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Class"
#define GLOBALTABLENAME "Macaroni.Model.Class"

namespace {

	static inline void createClassPtrUserData(lua_State * L, const ClassPtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(ClassPtr));
		ClassPtr * instance = new (memory) ClassPtr();		
		(*instance).operator=(source);
	}

	static inline ClassPtr & getInstance(lua_State * L, int index)
	{
		ClassPtr * ptrToPtr = (ClassPtr *) luaL_checkudata(L, index, METATABLENAME);
		ClassPtr & ptr = dynamic_cast<ClassPtr &>(*ptrToPtr);
		return ptr;
	}

	static inline ClassPtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putClassInstanceOnStack(lua_State * L, const ClassPtr & source)
	{
		createClassPtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct ClassLuaFunctions
{
	static int luaGc(lua_State * L)
	{
		ClassPtr * ptr = (ClassPtr *) luaL_checkudata(L, 1, METATABLENAME);
		ptr->~ClassPtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		ClassPtr & a = getInstance(L, 1); 
		ClassPtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		ClassPtr & ptr = getInstance(L);

		std::string index(luaL_checkstring(L, 2));
		
		///*if (index == "FullName")
		//{
		//	lua_pushlstring(L, ptr->GetName().c_str(), ptr->GetName().size());	
		//}
		//else if (index == "Namespace")
		//{
		//	ScopePtr scope = ptr->GetScope();
		//	NamespacePtr ns = boost::dynamic_pointer_cast<Namespace>(scope);
		//	NamespaceLuaMetaData::PutInstanceOnStack(L, ns);
		//}		
		//else
		//{*/
			return ScopeLuaMetaData::Index(
				L, boost::static_pointer_cast<Scope>(ptr), index);
		//}		
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		ClassPtr & ptr = getInstance(L);
		/*std::stringstream ss;
		ss << ptr->GetScopeNamespace()->GetFullName();
		ss << "::" << ptr->GetName();
		lua_pushlstring(L, ss.str().c_str(), ss.str().length());*/
		std::string fullName = ptr->GetFullName();
		lua_pushlstring(L, fullName.c_str(), fullName.length());
		return 1;
	}

	///*static int AddBaseClass(lua_State * L)
	//{
	//	ClassPtr & ptr = getInstance(L);
	//	ClassPtr & baseClass = getInstance(L, 2);
	//	ptr->AddBaseClass(baseClass);
	//	return 0;
	//}*/

	static int FindOrCreate(lua_State * L)
	{
		NamespacePtr & ns = NamespaceLuaMetaData::GetInstance(L, 1);
		std::string complexName(luaL_checkstring(L, 2));
		Class::FindOrCreate(ns, complexName);
		return 0;
	}
};

static const struct luaL_Reg tableMethods[]=
{
	{"FindOrCreate", ClassLuaFunctions::FindOrCreate},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", ClassLuaFunctions::__eq},
	{"__gc", ClassLuaFunctions::luaGc},
	{"__index", ClassLuaFunctions::__index},
	{"__tostring", ClassLuaFunctions::__tostring},
	{nullptr, nullptr}
};

bool ClassLuaMetaData::IsType(lua_State * L, int index)
{
	return ScopeLuaMetaData::IsType(L, index, METATABLENAME);
}

int ClassLuaMetaData::OpenInLua(lua_State * L)
{	
	luaL_getmetatable(L, METATABLENAME);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);

	NamespaceLuaMetaData::OpenInLua(L);

	ScopeLuaMetaData::OpenInLua(L);

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
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	return 1;
}

void ClassLuaMetaData::PutInstanceOnStack(lua_State * L, const ClassPtr & ptr)
{
	putClassInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif
