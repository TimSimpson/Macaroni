#ifndef MACARONI_MODEL_NAMESPACELUAREGISTER_CPP
#define MACARONI_MODEL_NAMESPACELUAREGISTER_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "Namespace.h"
#include "NamespaceLua.h"
#include "ScopeLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Namespace"
#define GLOBALTABLENAME "Macaroni.Model.Namespace"

namespace {

	// Taking a NamespacePtr, creates a Lua user data for a new NamespacePtr 
	// controlled by Lua which is then put on top of the Lua Stack.
	static inline void createNamespacePtrUserData(lua_State * L, const NamespacePtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(NamespacePtr));
		NamespacePtr * instance = new (memory) NamespacePtr();		
		(*instance).operator=(source);
	}

	static inline NamespacePtr & getInstance(lua_State * L, int index)
	{
		NamespacePtr * ptrToPtr = (NamespacePtr *) luaL_checkudata(L, index, METATABLENAME);
		NamespacePtr & ptr = dynamic_cast<NamespacePtr &>(*ptrToPtr);
		return ptr;
	}

	static inline NamespacePtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putNamespaceInstanceOnStack(lua_State * L, const NamespacePtr & source)
	{
		createNamespacePtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct NamespaceLuaFunctions
{
	static int CreateRoot(lua_State * L)
	{
		/*const char * name = luaL_checkstring(L, 1);
		NamespacePtr root = Namespace::CreateRoot(std::string(name));
		
		putNamespaceInstanceOnStack(L, root);
		return 1;*/
		return 0;
	}

	///*static int FindOrCreate(lua_State * L)
	//{
	//	NamespacePtr & ns = getInstance(L);
	//	std::string name(luaL_checkstring(L, 2));
	//	NamespacePtr ptr = ns->FindOrCreate(name);
	//	putNamespaceInstanceOnStack(L, ptr);
	//	return 1;
	//}*/

	static int luaGc(lua_State * L)
	{
		NamespacePtr * nsPtr = (NamespacePtr *) luaL_checkudata(L, 1, METATABLENAME);
		nsPtr->~NamespacePtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		NamespacePtr & a = getInstance(L, 1);
		NamespacePtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		NamespacePtr & ns = getInstance(L);

		std::string index(luaL_checkstring(L, 2));
		
		/*if (index == "FindOrCreate")
		{
			lua_pushcfunction(L, FindOrCreate);	
		}*/
		///*else if (index == "FullName")
		//{
		//	lua_pushlstring(L, ns->GetFullName().c_str(), ns->GetFullName().length());	
		//}
		//else if (index == "IsRoot")
		//{
		//	lua_pushboolean(L, ns->IsRoot());
		//}
		//else if (index == "Name")
		//{
		//	lua_pushlstring(L, ns->GetName().c_str(), ns->GetName().length());
		//}
	    if (index == "Parent" && ns->GetParent() != false)
		{
			putNamespaceInstanceOnStack(L, ns->GetParent());
		}
		else
		{
			return ScopeLuaMetaData::Index(
				L, boost::static_pointer_cast<Scope>(ns), index);
		}		
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		NamespacePtr & ns = getInstance(L);
		lua_pushlstring(L, ns->GetFullName().c_str(), ns->GetFullName().length());
		return 1;
	}


};

static const struct luaL_Reg tableMethods[]=
{
	{"CreateRoot", NamespaceLuaFunctions::CreateRoot},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", NamespaceLuaFunctions::__eq},
	{"__gc", NamespaceLuaFunctions::luaGc},
	{"__index", NamespaceLuaFunctions::__index},
	{"__tostring", NamespaceLuaFunctions::__tostring},
	{nullptr, nullptr}
};

static const struct luaL_Reg namespacesPropertyMetaTable[]=
{
	{"__gc", NamespaceLuaFunctions::luaGc},
	{"__index", NamespaceLuaFunctions::__index},
	{nullptr, nullptr}
};

NamespacePtr & NamespaceLuaMetaData::GetInstance(lua_State * L, int index)
{
	return getInstance(L, index);
}

bool NamespaceLuaMetaData::IsType(lua_State * L, int index)
{
	return ScopeLuaMetaData::IsType(L, index, METATABLENAME);
}

int NamespaceLuaMetaData::OpenInLua(lua_State * L)
{
	luaL_getmetatable(L, METATABLENAME);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);

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

void NamespaceLuaMetaData::PutInstanceOnStack(lua_State * L, const NamespacePtr & ptr)
{
	putNamespaceInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif