#ifndef MACARONI_IO_PATHLUA_CPP
#define MACARONI_IO_PATHLUA_CPP

#include "GeneratedFileWriterLuaMetaData.h"
#include "PathLua.h"

//extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
//}


struct lua_State;

#define PATHSMETATABLENAME "Macaroni.IO.Path.PathList"

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, IO)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME Path
#define LUAGLUE_CLASSREFNAME PathPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.IO.Path"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::IO::Path
#define LUAGLUE_REGISTRATIONCLASSNAME PathLuaMetaData
#define LUAGLUE_HELPERCLASS PathsLuaFunctions
#define LUAGLUE_OPENOTHERMODULES  \
		luaL_newmetatable(L, "Macaroni.IO.Path.PathList"); \
		luaL_register(L, nullptr, PathsProperty_MetaTableMethods);

//#define LUAGLUE_OPENOTHERMODULES /*Macaroni::Generator::Output::PathLuaMetaData::OpenInLua(L); \
//		luaL_newmetatable(L, "Macaroni.Generator.Output.Path.PathList"); \
//		luaL_register(L, nullptr, PathsProperty_MetaTableMethods);*/

#define LUAGLUE_CREATEMETATABLE 


BEGIN_NAMESPACE2(Macaroni, IO)

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
		PathListPtr paths = getPathListPtrFromStack(L, 1);
		paths.~PathListPtr();
		return 0;
	}

	int paths__index(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, 1);
		unsigned int index = luaL_checkinteger(L, 2);
		index --; // Remeber that lua is indexed by 1.
		if (index >= 0 && index < paths->size())
		{
			PathPtr & element = paths->at(index);
			PathLuaMetaData::PutInstanceOnStack(L, element);			
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	
	static int paths__len(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, 1);
		lua_pushinteger(L, paths->size());
		return 1;
	}	

	static int paths__tostring(lua_State * L)
	{
		PathListPtr paths = getPathListPtrFromStack(L, 1);
		lua_pushstring(L, "Paths Array");
		return 1;
	}	

	static const struct luaL_Reg PathsProperty_MetaTableMethods[]=
	{
		{"__gc", paths__gc},
		{"__index", paths__index},
		{"__len", paths__len},
		{"__tostring", paths__tostring},
		{nullptr, nullptr}
	};
} // end anon namespace

END_NAMESPACE2

#include "../LuaGlue.hpp"

	static int copyToDifferentRootPath(lua_State * L) 
	{
		PathPtr me = getInstance(L, 1);
		PathPtr rootPath = getInstance(L, 2);
		try 
		{
			me->CopyToDifferentRootPath(rootPath);
		} 
		catch(const std::exception & ex) 
		{
			lua_pushstring(L, ex.what());
			lua_error(L);		
		}
		return 0;
	}	
	
	static int createDirectory(lua_State * L)
	{
		PathPtr ptr = getInstance(L);
		ptr->CreateDirectory();
		return 0;
	}

	static int getPaths(lua_State * L)
	{
		PathPtr path = getInstance(L);
		std::string matchesPattern(luaL_checkstring(L, 2));		
		PathListPtr paths = path->GetPaths(matchesPattern);
		pushPathListPtrOntoStack(L, paths);
		return 1;		
	}
		
	static int isFileOlderThan(lua_State * L)
	{
		try 
		{
			PathPtr ptr = getInstance(L);
			std::string other(luaL_checkstring(L, 2));
			bool result = ptr->IsFileOlderThan(other);
			lua_pushboolean(L, result);
		} 
		catch(...)
		{
			lua_pushstring(L, "Error comparing files!");
			lua_error(L);
		}
		return 1;
	}

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "AbsolutePath")
		{	
			std::string absPath = ptr->GetAbsolutePath();
			lua_pushstring(L, absPath.c_str());
			return 1;
		}
		else if (index == "CopyToDifferentRootPath")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::copyToDifferentRootPath);
			return 1;
		}
		else if (index == "CreateDirectory")
		{	
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::createDirectory);
			return 1;
		}
		else if (index == "CreateFile")
		{	
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::createFile);
			return 1;
		}
		else if (index == "Exists")
		{	bool exists = ptr->Exists();
			lua_pushboolean(L, exists);
			return 1;
		}
		else if (index == "GetPaths")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::getPaths);
			return 1;		
		}
		else if (index == "IsDirectory")
		{
			bool isDirectory = ptr->IsDirectory();
			lua_pushboolean(L, isDirectory);
			return 1; 
		}
		else if (index == "IsFileOlderThan")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::isFileOlderThan);
			return 1; 
		} 
		else if (index == "NewPath")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::newPath);
			return 1;			
		}
		else if (index == "NewPathForceSlash")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::newPathForceSlash);
			return 1;			
		}
		else if (index == "Paths")
		{
			PathListPtr paths = ptr->GetPaths();			
			pushPathListPtrOntoStack(L, paths);
			return 1;
		}
		lua_pushnil(L);			
		return 1;
	}

	static int createFile(lua_State * L)
	{
		PathPtr ptr = getInstance(L);
		try 
		{
			GeneratedFileWriterPtr writer = ptr->CreateFile();
			GeneratedFileWriterLuaMetaData::PutInstanceOnStack(L, writer);
			return 1;
		} 
		catch(const std::exception & ex)
		{
			luaL_error(L, ex.what());
		}
		return 0; // line will never be reached
	}

	static int newPath(lua_State * L)
	{
		PathPtr ptr = getInstance(L);
		std::string name(luaL_checkstring(L, 2));
		PathPtr newPath = ptr->NewPath(name);
		LUAGLUE_REGISTRATIONCLASSNAME::PutInstanceOnStack(L, newPath);
		return 1;
	}

	static int newPathForceSlash(lua_State * L)
	{
		PathPtr ptr = getInstance(L);
		std::string name(luaL_checkstring(L, 2));
		PathPtr newPath = ptr->NewPathForceSlash(name);
		LUAGLUE_REGISTRATIONCLASSNAME::PutInstanceOnStack(L, newPath);
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

#include "../LuaGlue2.hpp"

#endif
