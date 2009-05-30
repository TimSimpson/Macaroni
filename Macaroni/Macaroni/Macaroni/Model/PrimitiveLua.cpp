#ifndef MACARONI_MODEL_PRIMITIVELUA_CPP
#define MACARONI_MODEL_PRIMITIVELUA_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "Primitive.h"
#include "PrimitiveLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Primitive"
#define GLOBALTABLENAME "Macaroni.Model.Primitive"

namespace {
 
	static inline void createPrimitivePtrUserData(lua_State * L, const PrimitivePtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(PrimitivePtr));
		PrimitivePtr * instance = new (memory) PrimitivePtr();		
		(*instance).operator=(source);
	}

	static inline PrimitivePtr & getInstance(lua_State * L, int index)
	{
		PrimitivePtr * ptrToPtr = (PrimitivePtr *) luaL_checkudata(L, index, METATABLENAME);
		PrimitivePtr & ptr = dynamic_cast<PrimitivePtr &>(*ptrToPtr);
		return ptr;
	}

	static inline PrimitivePtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putPrimitiveInstanceOnStack(lua_State * L, const PrimitivePtr & source)
	{
		createPrimitivePtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct PrimitiveLuaFunctions
{
	static int New(lua_State * L)
	{
		/*const char * name = luaL_checkstring(L, 1);
		NamespacePtr root = Namespace::CreateRoot(std::string(name));
		
		putNamespaceInstanceOnStack(L, root);
		return 1;*/
		return 0;
	}

	
	static int luaGc(lua_State * L)
	{
		PrimitivePtr * nsPtr = (PrimitivePtr *) luaL_checkudata(L, 1, METATABLENAME);
		nsPtr->~PrimitivePtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		PrimitivePtr & a = getInstance(L, 1);
		PrimitivePtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		PrimitivePtr & p = getInstance(L);

		std::string index(luaL_checkstring(L, 2));
		
		if (index == "FullName")
		{
			lua_pushlstring(L, p->GetFullName().c_str(), p->GetFullName().length());	
		}
		else if (index == "Name")
		{
			lua_pushlstring(L, p->GetName().c_str(), p->GetName().length());
		}
		///*else if (index == "Scope" && p->GetScope() != false)
		//{
		//	putNamespaceInstanceOnStack(L, p->GetScope());
		//}*/
		else
		{
			lua_pushnil(L);
		}		
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		PrimitivePtr & p = getInstance(L);
		lua_pushlstring(L, p->GetFullName().c_str(), p->GetFullName().length());
		return 1;
	}


};

static const struct luaL_Reg tableMethods[]=
{
	{"New", PrimitiveLuaFunctions::New},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", PrimitiveLuaFunctions::__eq},
	{"__gc", PrimitiveLuaFunctions::luaGc},
	{"__index", PrimitiveLuaFunctions::__index},
	{"__tostring", PrimitiveLuaFunctions::__tostring},
	{nullptr, nullptr}
};

static const struct luaL_Reg primitivePropertyMetaTable[]=
{
	{"__gc", PrimitiveLuaFunctions::luaGc},
	{"__index", PrimitiveLuaFunctions::__index},
	{nullptr, nullptr}
};

PrimitivePtr & PrimitiveLuaMetaData::GetInstance(lua_State * L, int index)
{
	return getInstance(L, index);
}

int PrimitiveLuaMetaData::OpenInLua(lua_State * L)
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
	return 1;
}

void PrimitiveLuaMetaData::PutInstanceOnStack(lua_State * L, const PrimitivePtr & ptr)
{
	putPrimitiveInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif