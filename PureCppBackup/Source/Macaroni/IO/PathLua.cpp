#ifndef MACARONI_IO_PATHLUA_CPP
#define MACARONI_IO_PATHLUA_CPP

#include "PathLua.h"

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}


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
#define LUAGLUE_OPENOTHERMODULES /*Macaroni::Generator::Output::PathLuaMetaData::OpenInLua(L); \
		luaL_newmetatable(L, "Macaroni.Generator.Output.Path.PathList"); \
		luaL_register(L, nullptr, PathsProperty_MetaTableMethods);*/

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

	static int createDirectory(lua_State * L)
	{
		PathPtr ptr = getInstance(L);
		ptr->CreateDirectory();
		return 0;
	}

	static int isFileOlderThan(lua_State * L)
	{
		PathPtr ptr = getInstance(L);
		std::string other(luaL_checkstring(L, 2));
		bool result = ptr->IsFileOlderThan(other);
		lua_pushboolean(L, result);
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
		else if (index == "CreateDirectory")
		{	
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::createDirectory);
			return 1;
		}
		else if (index == "Exists")
		{	bool exists = ptr->Exists();
			lua_pushboolean(L, exists);
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
		else if (index == "NewFileWriter")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::newFileWriter);
			return 1;			
		}	
		else if (index == "NewPath")
		{
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::newPath);
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

	static int newFileWriter(lua_State * L)
	{
		PathPtr ptr = getInstance(L);
		//std::stringstream code;
		//code << "io.open(\"" << ptr->ToString() << "\", 'w+');";
		//std::string str(code.str());
		//luaL_loadbuffer(L, str.c_str(), str.length(), "File Open Call");
		//lua_pcall(L, 0, 0, 0);
		// Never returns I think.
		lua_getglobal(L, "io");
		lua_pushstring(L, "open");
		lua_gettable(L, -2); // pushes function, removes key value "open" 
		lua_remove(L, -2); // remove "io"
		// function io.open should be at top of stack now.
		lua_pushstring(L, ptr->GetAbsolutePath().c_str()); // arg1
		lua_pushstring(L, "w+"); // arg2
		int stackSize = lua_gettop(L);
		lua_call(L, 2, 1);//LUA_MULTRET);
		int rtnCount = lua_gettop(L) - stackSize;

		return rtnCount;
	}

	static int newPath(lua_State * L)
	{
		PathPtr ptr = getInstance(L);
		std::string name(luaL_checkstring(L, 2));
		PathPtr newPath = ptr->NewPath(name);
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
