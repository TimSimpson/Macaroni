//#ifndef MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_LUA_GLUE_CPP
//#define MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_LUA_GLUE_CPP
// extern "C" {
//	#include <Lua/lua.h>
//	#include <Lua/lauxlib.h>
//	#include <Lua/lualib.h>
//}
//#include "ClassParentLua.h"
//#include <Macaroni/Model/Cpp/_.h>
//#include <Macaroni/Model/Cpp/ClassParent.h>
//#include <Macaroni/Model/Cpp/Class.h>
//#include <Macaroni/Model/Cpp/ClassLua.h>
//#include <Macaroni/Model/Cpp/Class.h>
//#include <Macaroni/Model/Cpp/ClassLua.h>
//#include <Macaroni/Model/Node.h>
//#include <Macaroni/Model/NodeLua.h>
//#include <Macaroni/Model/Cpp/Access.h>
//#include <Macaroni/Model/Cpp/AccessLua.h>
//#include <Macaroni/Model/Cpp/Access.h>
//#include <Macaroni/Model/Cpp/AccessLua.h>
//#include <Macaroni/Model/Node.h>
//#include <Macaroni/Model/NodeLua.h>
//#include <Macaroni/Model/Node.h>
//#include <Macaroni/Model/NodeLua.h>
//#include <Macaroni/Model/Cpp/Access.h>
//#include <Macaroni/Model/Cpp/AccessLua.h>
//#include <Macaroni/Model/Cpp/Access.h>
//#include <Macaroni/Model/Cpp/AccessLua.h>
//#include <Macaroni/Model/Cpp/Class.h>
//#include <Macaroni/Model/Cpp/ClassLua.h>
//#include <Macaroni/Model/Cpp/Class.h>
//#include <Macaroni/Model/Cpp/ClassLua.h>
//namespace Macaroni { namespace Model { namespace Cpp { 
//
//namespace { 
//
//	static inline void createUserData(lua_State * L, const ClassParentPtr & source)
//	{ 
//		void * memory = lua_newuserdata(L, sizeof(ClassParentPtr));
//		ClassParentPtr * instance = new (memory) ClassParentPtr();		
//		(*instance).operator=(source);
//	}
//
//	static inline ClassParentPtr & getInstance(lua_State * L, int index)
//	{
//		ClassParentPtr * ptrToPtr = (ClassParentPtr *) luaL_checkudata(L, index, "Macaroni.Model.Cpp.ClassParent");
//		ClassParentPtr & ptr = dynamic_cast<ClassParentPtr &>(*ptrToPtr);
//		return ptr;
//	}        
//	
//	static inline ClassParentPtr & getInstance(lua_State * L)
//	{
//		return getInstance(L, 1);
//	}
//
//	static inline void putInstanceOnStack(lua_State * L, const ClassParentPtr & source)
//	{
//		if (!source) 
//		{
//			lua_pushnil(L);
//		}
//		else
//		{
//			createUserData(L, source);
//			luaL_getmetatable(L, "Macaroni.Model.Cpp.ClassParent");
//			lua_setmetatable(L, -2); 
//		}
//	}
//
//} // End Anon namespace
//
// struct ClassParentLuaFunctions {
// 
//    static int luaGc(lua_State * L)
//    {
//        ClassParentPtr * ptr = (ClassParentPtr *) luaL_checkudata(L, 1, "Macaroni.Model.Cpp.ClassParent");
//		ptr->~ClassParentPtr();
//		return 0;
//	}	
//
//	static int __eq(lua_State * L)
//	{
//		ClassParentPtr & a = getInstance(L, 1); 
//		ClassParentPtr & b = getInstance(L, 2);
//		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
//		return 1;
//	}
//
//	static int __index(lua_State * L)
//	{
//		ClassParentPtr & ptr = getInstance(L);
//		std::string index(luaL_checkstring(L, 2));
//		return ClassParentLuaMetaData::Index(L, ptr, index);
//	}
//	
//	static int GetAccess(lua_State * L)		
//	{
//		ClassParentPtr & ptr = getInstance(L);
//		// TODO: Write out arguments here!
////WTF:Macaroni::Model::Cpp::Access
//Macaroni::Model::Cpp::Access __rtnValue = ptr->GetAccess(/* TODO: capabiltiy of arguments */);
//AccessLuaMetaData::PutInstanceOnStack(L, __rtnValue);
//		return 1;
//	}
//}; // end of function wrappers
// 
//static const struct luaL_Reg tableMethods[] =
//{
//	{NULL, NULL}
//};
//static const struct luaL_Reg metaTableMethods[] =
//{
//	{"GetAccess", ClassParentLuaFunctions::GetAccess},
//	{NULL, NULL}
//};
//
//int ClassParentLuaMetaData::Index(lua_State * L, ClassParentPtr & ptr, const std::string & index)
//{
//	if (index == "GetAccess")
//	{
//		lua_pushcfunction(L, ClassParentLuaFunctions::GetAccess);
//		return 1;
//	}
//	lua_pushnil(L);
//	return 1;
//}
//
//ClassParentPtr & ClassParentLuaMetaData::GetInstance(lua_State * L, int index)
//{
//	return getInstance(L, index);
//}        
//
//bool ClassParentLuaMetaData::IsType(lua_State * L, int index)
//{
//    // Copied this from the luaL_checkudata function
//	void * p = lua_touserdata(L, index);
//	bool returnValue = false;
//	if (p != NULL) // is user data
//	{
//		// Compares metatable from user data to one in registry.
//		if (lua_getmetatable(L, index))
//		{
//			lua_getfield(L, LUA_REGISTRYINDEX, "Macaroni.Model.Cpp.ClassParent");
//			if (lua_rawequal(L, -1, -2))
//			{
//				returnValue = true;
//			}
//			lua_pop(L, 2); // remove meta tables
//		}
//	}
//	return returnValue;
//}
//
//int ClassParentLuaMetaData::OpenInLua(lua_State * L)
//{	
//	luaL_getmetatable(L, "Macaroni.Model.Cpp.ClassParent");
//	if (lua_isnil(L, -1) != 1)
//	{
//		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
//	}
//
//	luaL_newmetatable(L, "Macaroni.Model.Cpp.ClassParent"); // create metaTable
//	luaL_register(L, NULL, metaTableMethods);
//
//	// Creates or reuses a table called "Macaroni.Model.Cpp.ClassParent" 
//    // and puts it in global scope.
//	luaL_register(L, "Macaroni.Model.Cpp.ClassParent", tableMethods);
//	return 1;
//}
//        
//} } } // End namespace 
//#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_LUA_GLUE_CPP