#ifndef MACARONI_MODEL_CPP_CLASSLUA_CPP
#define MACARONI_MODEL_CPP_CLASSLUA_CPP

#include "ClassLua.h"
#include "../NodeLua.h"
#include "../MemberLua.h"
#include "TypeInfoLua.h"
#include "VariableLua.h"

extern "C" {
	#include "../../../Lua/lua.h"
	#include "../../../Lua/lauxlib.h"
	#include "../../../Lua/lualib.h"
}


struct lua_State;

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE(Macaroni, Model, Cpp)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE
#define LUAGLUE_CLASSNAME Class
#define LUAGLUE_CLASSREFNAME ClassPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.Cpp.Class"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::Cpp::Class
#define LUAGLUE_REGISTRATIONCLASSNAME ClassLuaMetaData
#define LUAGLUE_HELPERCLASS ClassLuaFunctions
#define LUAGLUE_OPENOTHERMODULES Macaroni::Model::NodeLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE 

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int FunctionArgumentsOpenInLua(lua_State * L);

int FunctionReturnTypeInfoOpenInLua(lua_State * L);

END_NAMESPACE

#include "../../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		/*if (index == "Arguments")
		{
			createLUAGLUE_CLASSREFNAMEUserData(L, ptr);
			luaL_getmetatable(L, PROPERTIES_METATABLENAME_ARGUMENTS);
			lua_setmetatable(L, -2); 
			return 1;
		}
		else if (index == "ReturnType")
		{
			createLUAGLUE_CLASSREFNAMEUserData(L, ptr);
			luaL_getmetatable(L, PROPERTIES_METATABLENAME_RETURNTYPE);
			lua_setmetatable(L, -2); 
			return 1;
		}*/
		lua_pushnil(L);			
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		lua_pushstring(L, "Class");
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		/*{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, */

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../../LuaGlue2.hpp"

#endif
