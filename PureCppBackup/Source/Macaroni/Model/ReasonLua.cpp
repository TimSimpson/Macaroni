#ifndef MACARONI_MODEL_REASONLUA_CPP
#define MACARONI_MODEL_REASONLUA_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "AxiomLua.h"
#include "../Environment/DebugLog.h"
#include "ReasonLua.h"
#include "Reason.h"
#include "SourceLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Reason"
#define GLOBALTABLENAME "Macaroni.Model.Reason"
namespace {

	static inline void createReasonPtrUserData(lua_State * L, const ReasonPtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(ReasonPtr));
		ReasonPtr * instance = new (memory) ReasonPtr();		
		(*instance).operator=(source);
	}

	static inline ReasonPtr & getInstance(lua_State * L, int index)
	{
		ReasonPtr * ptrToPtr = (ReasonPtr *) luaL_checkudata(L, index, METATABLENAME);
		ReasonPtr & ptr = dynamic_cast<ReasonPtr &>(*ptrToPtr);
		return ptr;
	}


	static inline ReasonPtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putReasonInstanceOnStack(lua_State * L, const ReasonPtr & source)
	{
		createReasonPtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct ReasonLuaFunctions
{
	static int luaGc(lua_State * L)
	{
		ReasonPtr * ptr = (ReasonPtr *) luaL_checkudata(L, 1, METATABLENAME);
		ptr->~ReasonPtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		ReasonPtr & a = getInstance(L, 1); 
		ReasonPtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		ReasonPtr & ptr = getInstance(L);
		std::string index(luaL_checkstring(L, 2));
		if (index == "Axiom")
		{
			AxiomLuaMetaData::PutInstanceOnStack(L, ptr->GetAxiom());
		}
		else if (index == "ReferenceCount")
		{
			lua_pushinteger(L, ptr->GetReferenceCount());
		}
		else if (index == "Source")
		{
			SourceLuaMetaData::PutInstanceOnStack(L, ptr->GetSource());
		}
		else 
		{
			lua_pushnil(L);			
		}
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		ReasonPtr & ptr = getInstance(L);
		std::string fullName = ptr->ToString();
		lua_pushlstring(L, fullName.c_str(), fullName.length());
		return 1;
	}

	static int Create(lua_State * L)
	{
		AxiomPtr axiom = AxiomLuaMetaData::GetInstance(L, 1);
		SourcePtr source = SourceLuaMetaData::GetInstance(L, 2);
		ReasonPtr ptr = Reason::Create(axiom, source);
		putReasonInstanceOnStack(L, ptr);
		return 1;
	}

};

static const struct luaL_Reg tableMethods[]=
{
	{"Create", ReasonLuaFunctions::Create},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", ReasonLuaFunctions::__eq},
	{"__gc", ReasonLuaFunctions::luaGc},
	{"__index", ReasonLuaFunctions::__index},
	{"__tostring", ReasonLuaFunctions::__tostring},
	{nullptr, nullptr}
};

ReasonPtr & ReasonLuaMetaData::GetInstance(lua_State * L, int index)
{
	return getInstance(L, index);	
}

bool ReasonLuaMetaData::IsType(lua_State * L, int index)
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

int ReasonLuaMetaData::OpenInLua(lua_State * L)
{	
	DEBUGLOG_WRITE("OpenInLua begins...");
	luaL_getmetatable(L, METATABLENAME);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);
	
	AxiomLuaMetaData::OpenInLua(L);
	SourceLuaMetaData::OpenInLua(L);

	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);

	DEBUGLOG_WRITE("... open in lua ends.");
	return 1;
}

void ReasonLuaMetaData::PutInstanceOnStack(lua_State * L, const ReasonPtr & ptr)
{
	putReasonInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif
