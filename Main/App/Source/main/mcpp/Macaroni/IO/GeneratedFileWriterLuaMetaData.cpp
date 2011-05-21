#ifndef MACARONI_IO_GENERATEDFILEWRITERLUAMETADATALUA_CPP
#define MACARONI_IO_GENERATEDFILEWRITERLUAMETADATALUA_CPP

#include "GeneratedFileWriterLuaMetaData.h"

//extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
//}


struct lua_State;

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, IO)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME GeneratedFileWriter
#define LUAGLUE_CLASSREFNAME GeneratedFileWriterPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.IO.GeneratedFileWriter"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::IO::GeneratedFileWriter
#define LUAGLUE_REGISTRATIONCLASSNAME GeneratedFileWriterLuaMetaData
#define LUAGLUE_HELPERCLASS GeneratedFileWriterLuaFunctions
#define LUAGLUE_OPENOTHERMODULES /* */

#define LUAGLUE_CREATEMETATABLE 

#include "../LuaGlue.hpp"

	static int close(lua_State * L)
	{
		GeneratedFileWriterPtr ptr = getInstance(L);
		bool result = ptr->Close();
		lua_pushboolean(L, result);
		return 1;
	}

	static int write(lua_State * L)
	{
		GeneratedFileWriterPtr ptr = getInstance(L);
		std::string text(luaL_checkstring(L, 2));		
		ptr->Write(text);
		return 0;
	}

	static int writeLine(lua_State * L)
	{
		GeneratedFileWriterPtr ptr = getInstance(L);
		std::string text(luaL_checkstring(L, 2));		
		ptr->WriteLine(text);
		return 0;
	}

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "Close")
		{	
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::close);
			return 1;
		}
		else if (index == "Write")
		{	
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::write);
			return 1;
		}
		else if (index == "WriteLine")
		{	
			lua_pushcfunction(L, LUAGLUE_HELPERCLASS::writeLine);
			return 1;
		}		
		lua_pushnil(L);			
		return 1;
	}
	
	static int __tostring(lua_State * L)
	{
		GeneratedFileWriterPtr ptr = getInstance(L);
		std::stringstream ss;
		ss << "GeneratedFileWriter [" << ptr->GetFilePath().string() << "]";
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring},

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif
