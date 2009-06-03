#ifndef MACARONI_MODEL_SCOPELUAREGISTER_CPP
#define MACARONI_MODEL_SCOPELUAREGISTER_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "../Exception.h"
#include "Node.h"
#include "ClassLua.h"
#include "NamespaceLua.h"
#include "NodeLua.h"
#include "ScopeMemberLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define METATABLENAME "Macaroni.Model.Node"
#define GLOBALTABLENAME "Macaroni.Model.Node"
#define MEMBERSPROPERTY_METATABLENAME "Macaroni.Model.Node.Properties.Members"

namespace {

	static inline void createNodePtrUserData(lua_State * L, const NodePtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(NodePtr));
		NodePtr * instance = new (memory) NodePtr();		
		(*instance).operator=(source);
	}

	static inline NodePtr & getInstance(lua_State * L, int index)
	{
		NodePtr * ptrToPtr = (NodePtr *) luaL_checkudata(L, index, METATABLENAME);
		NodePtr & ptr = dynamic_cast<NodePtr &>(*ptrToPtr);
		return ptr;
	}

	static inline NodePtr getInstanceFromSubclass(lua_State * L, int index)
	{
		void * p = lua_touserdata(L, index);	
		if (p != nullptr)
		{			
			if (ClassLuaMetaData::IsType(L, index))
			{
				ClassPtr * ptr = (ClassPtr *) p;
				ClassPtr & ref = dynamic_cast<ClassPtr &>(*ptr);
				return boost::static_pointer_cast<Node>(ref);
			}
			else if (NamespaceLuaMetaData::IsType(L, index))
			{
				NamespacePtr * ptr = (NamespacePtr *) p;
				NamespacePtr & ref = dynamic_cast<NamespacePtr &>(*ptr);
				return boost::static_pointer_cast<Node>(ref);
			}
			else if (NodeLuaMetaData::IsType(L, index))
			{
				NodePtr * ptr = (NodePtr *) p;
				NodePtr & ref = dynamic_cast<NodePtr &>(*ptr);
				return ref;
			}		
		}
		luaL_typerror(L, index, METATABLENAME);
		throw new Macaroni::Exception("Code will never reach this point.");		
	}

	static inline NodePtr & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putNodeInstanceOnStack(lua_State * L, const NodePtr & source)
	{
		createNodePtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}

} // End Anon namespace

struct NodeLuaFunctions
{
	static int luaGc(lua_State * L)
	{
		NodePtr * ptr = (NodePtr *) luaL_checkudata(L, 1, METATABLENAME);
		ptr->~NodePtr();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		NodePtr & a = getInstance(L, 1); 
		NodePtr & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		NodePtr & ptr = getInstance(L);
		std::string index(luaL_checkstring(L, 2));
		return NodeLuaMetaData::Index(L, ptr, index);
	}

	static int __tostring(lua_State * L)
	{
		NodePtr & ptr = getInstance(L);
		std::string fullName = ptr->GetFullName();
		lua_pushlstring(L, fullName.c_str(), fullName.length());
		return 1;
	}

	static int Find(lua_State * L)
	{
		NodePtr & ptr = getInstance(L, 1);
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

	///*static int FindOrCreateClass(lua_State * L)
	//{
	//	NodePtr & scope = getInstanceFromSubclass(L, 1);
	//	std::string name(luaL_checkstring(L, 2));
	//	ClassPtr classPtr = scope->FindOrCreateClass(name);
	//	ClassLuaMetaData::PutInstanceOnStack(L, classPtr);
	//	return 1;
	//}

	//static int FindOrCreateNamespace(lua_State * L)
	//{
	//	NodePtr & scope = getInstanceFromSubclass(L, 1);
	//	std::string name(luaL_checkstring(L, 2));
	//	NamespacePtr nsPtr = scope->FindOrCreateNamespace(name);
	//	NamespaceLuaMetaData::PutInstanceOnStack(L, nsPtr);
	//	return 1;
	//}*/

	static int FindOrCreateNode(lua_State * L)
	{
		NodePtr & scope = getInstanceFromSubclass(L, 1);
		std::string name(luaL_checkstring(L, 2));
		NodePtr scopePtr = scope->FindOrCreateNode(name);
		NodeLuaMetaData::PutInstanceOnStack(L, scopePtr);
		return 1;
	}

	static int IsComplexName(lua_State * L)
	{
		std::string name(luaL_checkstring(L, 1));
		lua_pushboolean(L, Node::IsComplexName(name) ? 1 : 0);
		return 1;
	}

	static int IsSimpleName(lua_State * L)
	{
		std::string name(luaL_checkstring(L, 1));
		lua_pushboolean(L, Node::IsSimpleName(name) ? 1 : 0);
		return 1;
	}

	static int ParseComplexName(lua_State * L)
	{
		NodePtr ptr = getInstanceFromSubclass(L, 1);
		std::string complexName(luaL_checkstring(L, 2));
		NodePtr resultNode;
		std::string simpleName;
		ptr->ParseComplexName(complexName, resultNode, simpleName);
		putNodeInstanceOnStack(L, resultNode);
		lua_pushstring(L, simpleName.c_str());
		return 2;
	}

	static int SplitFirstNameOffComplexName(lua_State * L)
	{
		std::string complexName(luaL_checkstring(L, 1));
		std::string firstPart, lastPart;
		Node::SplitFirstNameOffComplexName(complexName, firstPart, lastPart);
		lua_pushstring(L, firstPart.c_str());
		lua_pushstring(L, lastPart.c_str());
		return 2;
	}

	static int SplitNodeAndMemberName(lua_State * L)
	{
		std::string complexName(luaL_checkstring(L, 1));
		std::string scopeName, memberName;
		Node::SplitNodeAndMemberName(complexName, scopeName, memberName);
		lua_pushstring(L, scopeName.c_str());
		lua_pushstring(L, memberName.c_str());
		return 2;
	}

	static int SplitComplexName(lua_State * L)
	{
		std::string complexName(luaL_checkstring(L, 1));
		std::vector<std::string> subNames;
		Node::SplitComplexName(complexName, subNames);
		lua_createtable(L, subNames.size(), 0);
		for(unsigned int i = 0; i < subNames.size(); i ++)
		{
			lua_pushinteger(L, i + 1);
			lua_pushstring(L, subNames[i].c_str());
			lua_settable(L, -3);
		}
		return 1;
	}
	static inline NodePtr & MembersProperty_GetInstance(lua_State * L)
	{
		NodePtr * ptrToPtr = 
			(NodePtr *) luaL_checkudata(L, 1, MEMBERSPROPERTY_METATABLENAME);
		NodePtr & ptr = dynamic_cast<NodePtr &>(*ptrToPtr);
		return ptr;
	}

	static int MembersProperty__gc(lua_State * L)
	{
		NodePtr & instance = MembersProperty_GetInstance(L);
		instance.~NodePtr();
		return 0;
	}

	static int MembersProperty__index(lua_State * L)
	{
		NodePtr & ptr = MembersProperty_GetInstance(L);
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
		NodePtr & ptr = MembersProperty_GetInstance(L);
		lua_pushinteger(L, ptr->GetMemberCount());
		return 1;
	}

	static int MembersProperty__tostring(lua_State * L)
	{
		NodePtr & ptr = MembersProperty_GetInstance(L);
		std::stringstream ss;
		ss << "Node\"" << ptr->GetName() << "\" Member Count = ";
		ss << ptr->GetMemberCount();
		lua_pushlstring(L, ss.str().c_str(), ss.str().length());
		return 1;
	}
};

static const struct luaL_Reg tableMethods[]=
{
	{"Find", NodeLuaFunctions::Find},
	{"IsComplexName", NodeLuaFunctions::IsComplexName},
	{"IsSimpleName", NodeLuaFunctions::IsSimpleName},
	{"SplitFirstNameOffComplexName", NodeLuaFunctions::SplitFirstNameOffComplexName},
	{"SplitNodeAndMemberName", NodeLuaFunctions::SplitNodeAndMemberName},
	{"SplitComplexName", NodeLuaFunctions::SplitComplexName},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__eq", NodeLuaFunctions::__eq},
	{"__gc", NodeLuaFunctions::luaGc},
	{"__index", NodeLuaFunctions::__index},
	{"__tostring", NodeLuaFunctions::__tostring},
	{nullptr, nullptr}
};

static const struct luaL_Reg MembersProperty_MetaTableMethods[]=
{
	{"__gc", NodeLuaFunctions::MembersProperty__gc},
	{"__index", NodeLuaFunctions::MembersProperty__index},
	{"__len", NodeLuaFunctions::MembersProperty__len},
	{"__tostring", NodeLuaFunctions::MembersProperty__tostring},
	{nullptr, nullptr}
};

int NodeLuaMetaData::Index(lua_State * L, NodePtr & ptr, const std::string & index)
{
	/*if (index == "FindOrCreateClass")
	{
		lua_pushcfunction(L, NodeLuaFunctions::FindOrCreateClass);	
		return 1;
	}
	else if (index == "FindOrCreateNamespace")
	{
		lua_pushcfunction(L, NodeLuaFunctions::FindOrCreateNamespace);	
		return 1;
	}
	else*/ if (index == "FindOrCreateNode")
	{
		lua_pushcfunction(L, NodeLuaFunctions::FindOrCreateNode);	
		return 1;
	}	
	else if (index == "IsRoot")
	{
		lua_pushboolean(L, ptr->IsRoot());
		return 1;
	}
	else if (index == "Members")
	{
		// Put a lua pointer for this Node object on the Lua stack,
		// but associate it with a different meta table.
		createNodePtrUserData(L, ptr);
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

bool NodeLuaMetaData::IsType(lua_State * L, int index, const char * metaTableName)
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

bool NodeLuaMetaData::IsType(lua_State * L, int index)
{
	return IsType(L, index, METATABLENAME);
}

int NodeLuaMetaData::OpenInLua(lua_State * L)
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

void NodeLuaMetaData::PutInstanceOnStack(lua_State * L, const NodePtr & ptr)
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
		putNodeInstanceOnStack(L, ptr);
	}
}

END_NAMESPACE2

#endif
