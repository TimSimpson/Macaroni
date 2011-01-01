#ifndef MACARONI_MODEL_CONTEXTLUA_CPP
#define MACARONI_MODEL_CONTEXTLUA_CPP

//MARIO extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
//MARIO }
#include "Context.h"
#include "ContextLua.h"
#include "../Environment/DebugLog.h"
#include "LibraryLua.h"
#include "Node.h"
#include "NodeLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Context"
#define GLOBALTABLENAME "Macaroni.Model.Context"

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
	static int findLibrary(lua_State * L)
	{
		ContextPtr context = getInstance(L);
		std::string group(luaL_checkstring(L, 2));
		std::string name(luaL_checkstring(L, 3));
		std::string version(luaL_checkstring(L, 4));		
		LibraryPtr library = context->FindLibrary(group, name, version);
		LibraryLuaMetaData::PutInstanceOnStack(L, library);
		return 1;
	}

	static int findOrCreateLibrary(lua_State * L)
	{
		ContextPtr context = getInstance(L);
		std::string group(luaL_checkstring(L, 2));
		std::string name(luaL_checkstring(L, 3));
		std::string version(luaL_checkstring(L, 4));		
		LibraryPtr library = context->FindOrCreateLibrary(group, name, version);
		LibraryLuaMetaData::PutInstanceOnStack(L, library);
		return 1;
	}

	static int luaGc(lua_State * L)
	{
		ContextPtr * nsPtr = (ContextPtr *) luaL_checkudata(L, 1, METATABLENAME);
		nsPtr->~ContextPtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		ContextPtr & a = getInstance(L, 1);
		ContextPtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		ContextPtr & context = getInstance(L);

		std::string index(luaL_checkstring(L, 2));
		
		if (index == "FindLibrary")
		{
			lua_pushcfunction(L, findLibrary);
		}
		else if (index == "FindOrCreateLibrary")
		{
			lua_pushcfunction(L, findOrCreateLibrary);
		}
		else if (index == "GetReferenceCount")
		{
			lua_pushcfunction(L, GetReferenceCount);	
		}
		else if (index == "Root")
		{
			NodeLuaMetaData::PutInstanceOnStack(L, context->GetRoot());
		}	
		else if (index == "RootLibrary")
		{
			LibraryLuaMetaData::PutInstanceOnStack(L, context->GetRootLibrary());
		}
		else
		{
			lua_pushnil(L);
		}		
		return 1;
	}

	static int __tostring(lua_State * L)
	{
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
	}

	static int New(lua_State * L)
	{
		std::string rootName(luaL_checkstring(L, 1));
	
		ContextPtr ptr = new Context(rootName);		
		putContextInstanceOnStack(L, ptr);
		return 1;
	}

	static int GetReferenceCount(lua_State * L)
	{
		ContextPtr & context = getInstance(L);
		lua_pushinteger(L, context->GetReferenceCount());		
		return 1;
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
	luaL_register(L, nullptr, metaTableMethods);

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

	/** Now open dependent libraries. */
	NodeLuaMetaData::OpenInLua(L);

	return 1;
}

void ContextLuaMetaData::PutInstanceOnStack(lua_State * L, const ContextPtr & ptr)
{
	putContextInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif
