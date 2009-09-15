#ifndef MACARONI_GENERATOR_OUTPUT_PATHLUA_CPP
#define MACARONI_GENERATOR_OUTPUT_PATHLUA_CPP

#include "PathLua.h"

extern "C" {
	#include "../../../Lua/lua.h"
	#include "../../../Lua/lauxlib.h"
	#include "../../../Lua/lualib.h"
}


struct lua_State;

#define PATHSMETATABLENAME "Macaroni.Generator.Output.Path.PathList"

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE(Macaroni, Generator, Output)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE
#define LUAGLUE_CLASSNAME Path
#define LUAGLUE_CLASSREFNAME PathPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Generator.Output.Path"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Generator::Output::Path
#define LUAGLUE_REGISTRATIONCLASSNAME PathLuaMetaData
#define LUAGLUE_HELPERCLASS PathsLuaFunctions
#define LUAGLUE_OPENOTHERMODULES Macaroni::Generator::Output::PathLuaMetaData::OpenInLua(L); \
		luaL_newmetatable(L, PATHSMETATABLENAME); \
		luaL_register(L, nullptr, PathsProperty_MetaTableMethods);

#define LUAGLUE_CREATEMETATABLE 


BEGIN_NAMESPACE(Macaroni, Generator, Output)

namespace {

	PathListPtr getPathListPtrFromStack(lua_State * L, int index)
	{
		PathListPtr * ptrToPtr = (PathListPtr *) luaL_checkudata(L, index, PATHSMETATABLENAME);
		PathListPtr & ptr = dynamic_cast<PathListPtr &>(*ptrToPtr);
		return ptr;
	}

	void pushPathListPtrOntoStack(lua_State * L, PathListPtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(PathListPtr));
		PathListPtr * instance = new (memory) PathListPtr();
		(*instance).operator= (source);

		luaL_getmetatable(L, PATHSMETATABLENAME);
		lua_setmetatable(L, -2);
	}

	int paths__gc(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, -1);
		paths.~PathListPtr();
		return 0;
	}

	int paths__index(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, -1);
		unsigned int index = luaL_checkinteger(L, 2);
		if (index > 0 && index <= paths->size())
		{
			Path & element = paths->at(index);
			PathPtr elementPtr(&element);
			PathLuaMetaData::PutInstanceOnStack(L, elementPtr);			
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	
	static int paths__len(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, -1);
		lua_pushinteger(L, paths->size());
		return 1;
	}	

	static const struct luaL_Reg PathsProperty_MetaTableMethods[]=
	{
		{"__gc", paths__gc},
		{"__index", paths__index},
		{"__len", paths__len},
		{nullptr, nullptr}
	};
} // end anon namespace

END_NAMESPACE

#include "../../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "Paths")
		{
			PathListPtr paths = ptr->GetPaths();			
			pushPathListPtrOntoStack(L, paths);
			return 1;
		}
		lua_pushnil(L);			
		return 1;
	}


	static int __tostring(lua_State * L)
	{
		PathPtr path = getInstance(L);
		lua_pushstring(L, path->ToString().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring},

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../../LuaGlue2.hpp"

#endif
