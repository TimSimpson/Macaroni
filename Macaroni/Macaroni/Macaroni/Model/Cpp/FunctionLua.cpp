#ifndef MACARONI_MODEL_CPP_FUNCTIONLUA_CPP
#define MACARONI_MODEL_CPP_FUNCTIONLUA_CPP

#include "../NodeLua.h"
#include "FunctionLua.h"
#include "TypeInfoLua.h"

extern "C" {
	#include "../../../Lua/lua.h"
	#include "../../../Lua/lauxlib.h"
	#include "../../../Lua/lualib.h"
}


struct lua_State;

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE(Macaroni, Model, Cpp)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE
#define LUAGLUE_CLASSNAME Function
#define LUAGLUE_CLASSREFNAME FunctionPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.Cpp.Function"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::Cpp::Function
#define LUAGLUE_REGISTRATIONCLASSNAME FunctionLuaMetaData
#define LUAGLUE_HELPERCLASS FunctionLuaFunctions
#define LUAGLUE_OPENOTHERMODULES Macaroni::Model::NodeLuaMetaData::OpenInLua(L); FunctionReturnTypeInfoOpenInLua(L);
#define LUAGLUE_CREATEMETATABLE 

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int FunctionReturnTypeInfoOpenInLua(lua_State * L);

END_NAMESPACE

#include "../../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "ReturnType")
		{
			createLUAGLUE_CLASSREFNAMEUserData(L, ptr);
			luaL_getmetatable(L, "Macaroni.Model.Cpp.Function.ReturnType");
			return 1;
		}
		lua_pushnil(L);			
		return 1;
	}




	static int __tostring(lua_State * L)
	{
		lua_pushstring(L, "Function");
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		/*{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, */

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../../LuaGlue2.hpp"


BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int rtnTypeIndex(lua_State * L)
{
	FunctionPtr & ptr = getInstance(L);
	std::string index(luaL_checkstring(L, 2));
	return TypeInfoLuaMetaData::Index(L, ptr->GetReturnType(), index);
}

static const struct luaL_Reg returnTypeMetaTableMethods[]=
{
	{"__eq", LUAGLUE_HELPERCLASS::__eq},
	{"__gc", LUAGLUE_HELPERCLASS::luaGc},
	{"__index", rtnTypeIndex},
	{"__tostring", LUAGLUE_HELPERCLASS::__tostring},
	{nullptr, nullptr}
};

int FunctionReturnTypeInfoOpenInLua(lua_State * L)
{
	luaL_getmetatable(L, "Macaroni.Model.Cpp.Function.ReturnType");
	if (lua_isnil(L, -1) != 1)
	{
		return 0;
	}
	luaL_newmetatable(L, "Macaroni.Model.Cpp.Function.ReturnType"); // create metaTable
	luaL_register(L, nullptr, returnTypeMetaTableMethods);
}


END_NAMESPACE

#endif
