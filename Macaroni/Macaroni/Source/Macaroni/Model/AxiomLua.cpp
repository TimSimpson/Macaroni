#ifndef MACARONI_MODEL_AXIOMLUA_CPP
#define MACARONI_MODEL_AXIOMLUA_CPP

//MARIO extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
//MARIO }
#include "AxiomLua.h"
#include "Axiom.h"
#include "../Environment/DebugLog.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

//class LuaControlledAxiom : public Axiom
//{
//public:
//	LuaControlledAxiom(lua_State * L)
//	{
//	}
//	virtual const std::string ToString() const
//	{
//		// Get registry table "AxiomCallbacks"
//		// 
//	}
//private:
//	lua_State * L;
//
//}

#define METATABLENAME "Macaroni.Model.Axiom"
#define GLOBALTABLENAME "Macaroni.Model.Axiom"
namespace {

	static inline void createAxiomPtrUserData(lua_State * L, const AxiomPtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(AxiomPtr));
		AxiomPtr * instance = new (memory) AxiomPtr();		
		(*instance).operator=(source);
	}

	static inline AxiomPtr & getInstance(lua_State * L, int index)
	{
		AxiomPtr * ptrToPtr = (AxiomPtr *) luaL_checkudata(L, index, METATABLENAME);
		AxiomPtr & ptr = dynamic_cast<AxiomPtr &>(*ptrToPtr);
		return ptr;
	}


	static inline AxiomPtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putAxiomInstanceOnStack(lua_State * L, const AxiomPtr & source)
	{
		createAxiomPtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct AxiomLuaFunctions
{
	static int luaGc(lua_State * L)
	{
		AxiomPtr * ptr = (AxiomPtr *) luaL_checkudata(L, 1, METATABLENAME);
		ptr->~AxiomPtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		AxiomPtr & a = getInstance(L, 1); 
		AxiomPtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		AxiomPtr & ptr = getInstance(L);
		std::string index(luaL_checkstring(L, 2));
		/*if (index == "ReferenceCount")
		{
			lua_pushinteger(L, ptr->GetReferenceCount());
		}
		else 
		{*/
			lua_pushnil(L);			
		//}
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		AxiomPtr & ptr = getInstance(L);
		lua_pushlstring(L, ptr->ToString().c_str(), ptr->ToString().length());
		return 1;
	}

	static int LuaCreate(lua_State * L)
	{
		std::string words(luaL_checkstring(L, 1));
		class LuaAxiom : public Axiom
		{
		public:
			LuaAxiom(const std::string & words): ref(0), words(words){}
			~LuaAxiom(){}			
			const std::string ToString() const { return words; }
		protected:
			bool onDereference(){ return ((-- ref) > 0);}
			void onReference(){ ref ++; }
		private:
			int ref;
			std::string words;
		};
		AxiomPtr newInstance = AxiomPtr(new LuaAxiom(words));
		putAxiomInstanceOnStack(L, newInstance);
		return 1;
	}
};

static const struct luaL_Reg tableMethods[]=
{
	{"LuaCreate", AxiomLuaFunctions::LuaCreate},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", AxiomLuaFunctions::__eq},
	{"__gc", AxiomLuaFunctions::luaGc},
	{"__index", AxiomLuaFunctions::__index},
	{"__tostring", AxiomLuaFunctions::__tostring},
	{nullptr, nullptr}
};

AxiomPtr & AxiomLuaMetaData::GetInstance(lua_State * L, int index)
{
	return getInstance(L, index);	
}

bool AxiomLuaMetaData::IsType(lua_State * L, int index)
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

int AxiomLuaMetaData::OpenInLua(lua_State * L)
{	
	luaL_getmetatable(L, METATABLENAME);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);

	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);

	return 1;
}

void AxiomLuaMetaData::PutInstanceOnStack(lua_State * L, const AxiomPtr & ptr)
{
	putAxiomInstanceOnStack(L, ptr);
}

END_NAMESPACE2

#endif
