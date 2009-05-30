#ifndef MACARONI_MODEL_SCOPELUAREGISTER_CPP
#define MACARONI_MODEL_SCOPELUAREGISTER_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "../Exception.h"
#include "Scope.h"
#include "ClassLua.h"
#include "NamespaceLua.h"
#include "ScopeLua.h"
#include "ScopeMemberLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Scope"
#define GLOBALTABLENAME "Macaroni.Model.Scope"
#define MEMBERSPROPERTY_METATABLENAME "Macaroni.Model.Scope.Properties.Members"

namespace {

	static inline void createScopePtrUserData(lua_State * L, const ScopePtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(ScopePtr));
		ScopePtr * instance = new (memory) ScopePtr();		
		(*instance).operator=(source);
	}

	static inline ScopePtr & getInstance(lua_State * L, int index)
	{
		ScopePtr * ptrToPtr = (ScopePtr *) luaL_checkudata(L, index, METATABLENAME);
		ScopePtr & ptr = dynamic_cast<ScopePtr &>(*ptrToPtr);
		return ptr;
	}

	static inline ScopePtr getInstanceFromSubclass(lua_State * L, int index)
	{
		void * p = lua_touserdata(L, index);	
		if (p != nullptr)
		{			
			if (ClassLuaMetaData::IsType(L, index))
			{
				ClassPtr * ptr = (ClassPtr *) p;
				ClassPtr & ref = dynamic_cast<ClassPtr &>(*ptr);
				return boost::static_pointer_cast<Scope>(ref);
			}
			else if (NamespaceLuaMetaData::IsType(L, index))
			{
				NamespacePtr * ptr = (NamespacePtr *) p;
				NamespacePtr & ref = dynamic_cast<NamespacePtr &>(*ptr);
				return boost::static_pointer_cast<Scope>(ref);
			}
			else if (ScopeLuaMetaData::IsType(L, index))
			{
				ScopePtr * ptr = (ScopePtr *) p;
				ScopePtr & ref = dynamic_cast<ScopePtr &>(*ptr);
				return ref;
			}		
		}
		luaL_typerror(L, index, METATABLENAME);
		throw new Macaroni::Exception("Code will never reach this point.");		
	}

	static inline ScopePtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putScopeInstanceOnStack(lua_State * L, const ScopePtr & source)
	{
		createScopePtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct ScopeLuaFunctions
{
	static int luaGc(lua_State * L)
	{
		ScopePtr * ptr = (ScopePtr *) luaL_checkudata(L, 1, METATABLENAME);
		ptr->~ScopePtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		ScopePtr & a = getInstance(L, 1); 
		ScopePtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		ScopePtr & ptr = getInstance(L);
		std::string index(luaL_checkstring(L, 2));
		return ScopeLuaMetaData::Index(L, ptr, index);
	}

	static int __tostring(lua_State * L)
	{
		ScopePtr & ptr = getInstance(L);
		std::string fullName = ptr->GetFullName();
		lua_pushlstring(L, fullName.c_str(), fullName.length());
		return 1;
	}

	static int Find(lua_State * L)
	{
		ScopePtr & ptr = getInstance(L, 1);
		std::string name(luaL_checkstring(L, 2));
		ScopeMemberPtr member = ptr->Find(name);
		if (member == false)
		{
			lua_pushnil(L);
		} 
		else 
		{
			ScopeMemberLuaMetaData::PutInstanceOnStack(L, member);
		}
		return 1;
	}

	static int FindOrCreateClass(lua_State * L)
	{
		ScopePtr & scope = getInstanceFromSubclass(L, 1);
		std::string name(luaL_checkstring(L, 2));
		ClassPtr classPtr = scope->FindOrCreateClass(name);
		ClassLuaMetaData::PutInstanceOnStack(L, classPtr);
		return 1;
	}

	static int FindOrCreateNamespace(lua_State * L)
	{
		ScopePtr & scope = getInstanceFromSubclass(L, 1);
		std::string name(luaL_checkstring(L, 2));
		NamespacePtr nsPtr = scope->FindOrCreateNamespace(name);
		NamespaceLuaMetaData::PutInstanceOnStack(L, nsPtr);
		return 1;
	}

	static int FindOrCreateScope(lua_State * L)
	{
		ScopePtr & scope = getInstanceFromSubclass(L, 1);
		std::string name(luaL_checkstring(L, 2));
		ScopePtr scopePtr = scope->FindOrCreateScope(name);
		ScopeLuaMetaData::PutInstanceOnStack(L, scopePtr);
		return 1;
	}

	static int IsComplexName(lua_State * L)
	{
		std::string name(luaL_checkstring(L, 1));
		lua_pushboolean(L, Scope::IsComplexName(name) ? 1 : 0);
		return 1;
	}

	static int IsSimpleName(lua_State * L)
	{
		std::string name(luaL_checkstring(L, 1));
		lua_pushboolean(L, Scope::IsSimpleName(name) ? 1 : 0);
		return 1;
	}

	static int ParseComplexName(lua_State * L)
	{
		ScopePtr ptr = getInstanceFromSubclass(L, 1);
		std::string complexName(luaL_checkstring(L, 2));
		ScopePtr resultScope;
		std::string simpleName;
		Scope::ParseComplexName(ptr, complexName, resultScope, simpleName);
		putScopeInstanceOnStack(L, resultScope);
		lua_pushstring(L, simpleName.c_str());
		return 2;
	}

	static int SplitFirstNameOffComplexName(lua_State * L)
	{
		std::string complexName(luaL_checkstring(L, 1));
		std::string firstPart, lastPart;
		Scope::SplitFirstNameOffComplexName(complexName, firstPart, lastPart);
		lua_pushstring(L, firstPart.c_str());
		lua_pushstring(L, lastPart.c_str());
		return 2;
	}

	static int SplitScopeAndMemberName(lua_State * L)
	{
		std::string complexName(luaL_checkstring(L, 1));
		std::string scopeName, memberName;
		Scope::SplitScopeAndMemberName(complexName, scopeName, memberName);
		lua_pushstring(L, scopeName.c_str());
		lua_pushstring(L, memberName.c_str());
		return 2;
	}

	static int SplitComplexName(lua_State * L)
	{
		std::string complexName(luaL_checkstring(L, 1));
		std::vector<std::string> subNames;
		Scope::SplitComplexName(complexName, subNames);
		lua_createtable(L, subNames.size(), 0);
		for(unsigned int i = 0; i < subNames.size(); i ++)
		{
			lua_pushinteger(L, i + 1);
			lua_pushstring(L, subNames[i].c_str());
			lua_settable(L, -3);
		}
		return 1;
	}
	static inline ScopePtr & MembersProperty_GetInstance(lua_State * L)
	{
		ScopePtr * ptrToPtr = 
			(ScopePtr *) luaL_checkudata(L, 1, MEMBERSPROPERTY_METATABLENAME);
		ScopePtr & ptr = dynamic_cast<ScopePtr &>(*ptrToPtr);
		return ptr;
	}

	static int MembersProperty__gc(lua_State * L)
	{
		ScopePtr & instance = MembersProperty_GetInstance(L);
		instance.~ScopePtr();
		return 0;
	}

	static int MembersProperty__index(lua_State * L)
	{
		ScopePtr & ptr = MembersProperty_GetInstance(L);
		int index = luaL_checkinteger(L, 2);
		if (index > 0 && index <= ptr->GetMemberCount())
		{
			ScopeMemberLuaMetaData::PutInstanceOnStack(L, ptr->GetMember(index - 1));
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	/** Its so frustrating to have to write all of this code.
	 * I really hope it will be easy to auto-gen this with Macaroni in the
	 * future... a world where code can be custom auto-gened with the power
	 * of Macaroni... would be ideal... */
	static int MembersProperty__len(lua_State * L)
	{
		ScopePtr & ptr = MembersProperty_GetInstance(L);
		lua_pushinteger(L, ptr->GetMemberCount());
		return 1;
	}

	static int MembersProperty__tostring(lua_State * L)
	{
		ScopePtr & ptr = MembersProperty_GetInstance(L);
		std::stringstream ss;
		ss << "Scope\"" << ptr->GetName() << "\" Member Count = ";
		ss << ptr->GetMemberCount();
		lua_pushlstring(L, ss.str().c_str(), ss.str().length());
		return 1;
	}
};

static const struct luaL_Reg tableMethods[]=
{
	{"Find", ScopeLuaFunctions::Find},
	{"IsComplexName", ScopeLuaFunctions::IsComplexName},
	{"IsSimpleName", ScopeLuaFunctions::IsSimpleName},
	{"ParseComplexName", ScopeLuaFunctions::ParseComplexName},
	{"SplitFirstNameOffComplexName", ScopeLuaFunctions::SplitFirstNameOffComplexName},
	{"SplitScopeAndMemberName", ScopeLuaFunctions::SplitScopeAndMemberName},
	{"SplitComplexName", ScopeLuaFunctions::SplitComplexName},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", ScopeLuaFunctions::__eq},
	{"__gc", ScopeLuaFunctions::luaGc},
	{"__index", ScopeLuaFunctions::__index},
	{"__tostring", ScopeLuaFunctions::__tostring},
	{nullptr, nullptr}
};

static const struct luaL_Reg MembersProperty_MetaTableMethods[]=
{
	{"__gc", ScopeLuaFunctions::MembersProperty__gc},
	{"__index", ScopeLuaFunctions::MembersProperty__index},
	{"__len", ScopeLuaFunctions::MembersProperty__len},
	{"__tostring", ScopeLuaFunctions::MembersProperty__tostring},
	{nullptr, nullptr}
};

int ScopeLuaMetaData::Index(lua_State * L, ScopePtr & ptr, const std::string & index)
{
	if (index == "FindOrCreateClass")
	{
		lua_pushcfunction(L, ScopeLuaFunctions::FindOrCreateClass);	
		return 1;
	}
	else if (index == "FindOrCreateNamespace")
	{
		lua_pushcfunction(L, ScopeLuaFunctions::FindOrCreateNamespace);	
		return 1;
	}
	else if (index == "FindOrCreateScope")
	{
		lua_pushcfunction(L, ScopeLuaFunctions::FindOrCreateScope);	
		return 1;
	}	
	else if (index == "IsRoot")
	{
		lua_pushboolean(L, ptr->IsRoot());
		return 1;
	}
	else if (index == "Members")
	{
		// Put a lua pointer for this Scope object on the Lua stack,
		// but associate it with a different meta table.
		createScopePtrUserData(L, ptr);
		luaL_getmetatable(L, MEMBERSPROPERTY_METATABLENAME);
		lua_setmetatable(L, -2);
		return 1;
	}
	// This is very dangerous, because if any Index functions believe our
	// pointer is a lua user data and try to muck with it, they could
	// destabalize the app!
	// I'd like to find a way to enforce that this does not happen in the 
	// future, especially if this gets too nested...
	return ScopeMemberLuaMetaData::Index(
			L, boost::static_pointer_cast<ScopeMember>(ptr), index);		
}

bool ScopeLuaMetaData::IsType(lua_State * L, int index, const char * metaTableName)
{
	// Copied this from the luaL_checkudata function
	void * p = lua_touserdata(L, index);
	bool returnValue = false;
	if (p != nullptr) // is user data
	{
		// Compares metatable from user data to one in registry.
		if (lua_getmetatable(L, index))
		{
			lua_getfield(L, LUA_REGISTRYINDEX, metaTableName);
			if (lua_rawequal(L, -1, -2))
			{
				returnValue = true;
			}
			lua_pop(L, 2); // remove meta tables
		}
	}
	return returnValue;
}

bool ScopeLuaMetaData::IsType(lua_State * L, int index)
{
	return IsType(L, index, METATABLENAME);
}

int ScopeLuaMetaData::OpenInLua(lua_State * L)
{	
	luaL_getmetatable(L, METATABLENAME);
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);

	luaL_newmetatable(L, MEMBERSPROPERTY_METATABLENAME);
	luaL_register(L, nullptr, MembersProperty_MetaTableMethods);

	ScopeMemberLuaMetaData::OpenInLua(L);
	
	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	return 1;
}

void ScopeLuaMetaData::PutInstanceOnStack(lua_State * L, const ScopePtr & ptr)
{
	if (boost::dynamic_pointer_cast<Class>(ptr) != nullptr)
	{
		ClassLuaMetaData::PutInstanceOnStack(L, boost::dynamic_pointer_cast<Class>(ptr));
	}
	else if (boost::dynamic_pointer_cast<Namespace>(ptr) != nullptr)
	{
		NamespaceLuaMetaData::PutInstanceOnStack(L, boost::dynamic_pointer_cast<Namespace>(ptr));
	}
	else
	{
		putScopeInstanceOnStack(L, ptr);
	}
}

END_NAMESPACE2

#endif
