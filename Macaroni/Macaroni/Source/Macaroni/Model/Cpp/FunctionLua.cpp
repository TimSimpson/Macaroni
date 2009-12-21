#ifndef MACARONI_MODEL_CPP_FUNCTIONLUA_CPP
#define MACARONI_MODEL_CPP_FUNCTIONLUA_CPP

#include "../NodeLua.h"
#include "../../Environment/DebugLog.h"
#include "FunctionLua.h"
#include "../MemberLua.h"
#include "../TypeLua.h"
#include "VariableLua.h"

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
#define LUAGLUE_OPENOTHERMODULES Macaroni::Model::NodeLuaMetaData::OpenInLua(L); \
	FunctionArgumentsOpenInLua(L); \
	Macaroni::Model::TypeLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE 

#define PROPERTIES_METATABLENAME_ARGUMENTS "Macaroni.Model.Cpp.Function.Arguments"
#define PROPERTIES_METATABLENAME_RETURNTYPE "Macaroni.Model.Cpp.Function.ReturnType"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int FunctionArgumentsOpenInLua(lua_State * L);

int FunctionReturnTypeInfoOpenInLua(lua_State * L);

END_NAMESPACE

#include "../../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "Arguments")
		{
			createLUAGLUE_CLASSREFNAMEUserData(L, ptr);
			luaL_getmetatable(L, PROPERTIES_METATABLENAME_ARGUMENTS);
			lua_setmetatable(L, -2); 
			return 1;
		}
		else if (index == "CodeBlock")
		{
			lua_pushstring(L, ptr->GetCodeBlock().c_str());
			return 1;
		}
		else if (index == "ReturnType")
		{
			TypeLuaMetaData::PutInstanceOnStack(L, ptr->GetReturnType());
			///*createLUAGLUE_CLASSREFNAMEUserData(L, ptr);
			//luaL_getmetatable(L, PROPERTIES_METATABLENAME_RETURNTYPE);
			//lua_setmetatable(L, -2); */
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

int functionArgumentsIndex(lua_State * L)
{
	FunctionPtr & ptr = getInstance(L, 1, PROPERTIES_METATABLENAME_ARGUMENTS);
	int index = luaL_checkinteger(L, 2);	
	Macaroni::Model::MemberLuaMetaData::PutInstanceOnStack(L, ptr->GetArgument(index - 1));
	return 1;
}

int functionArgumentsLen(lua_State * L)
{
	FunctionPtr & ptr = getInstance(L, 1, PROPERTIES_METATABLENAME_ARGUMENTS);
	lua_pushinteger(L, ptr->GetArgumentCount());
	return 1;
}

static const struct luaL_Reg argumentsMetaTableMethods[]=
{
	{"__eq", LUAGLUE_HELPERCLASS::__eq},
	{"__gc", LUAGLUE_HELPERCLASS::luaGc},
	{"__index", functionArgumentsIndex},
	{"__len", functionArgumentsLen},
	{"__tostring", LUAGLUE_HELPERCLASS::__tostring},
};

int FunctionArgumentsOpenInLua(lua_State * L)
{
	DEBUGLOG_WRITE("FunctionArgumentsOpenInLua begins...");

	luaL_getmetatable(L, PROPERTIES_METATABLENAME_ARGUMENTS);
	if (lua_isnil(L, -1) != 1)
	{
		DEBUGLOG_WRITE("... SKIP! FunctionArgumentsOpenInLua.");
		return 0;
	}
	luaL_newmetatable(L, PROPERTIES_METATABLENAME_ARGUMENTS); // create metaTable
	luaL_register(L, nullptr, argumentsMetaTableMethods);

	DEBUGLOG_WRITE("... end FunctionArgumentsOpenInLua.");
	return 1;
}

//
//int rtnTypeIndex(lua_State * L)
//{
//	FunctionPtr & ptr = getInstance(L, 1, PROPERTIES_METATABLENAME_RETURNTYPE);
//	std::string index(luaL_checkstring(L, 2));
//	return TypeLuaMetaData::Index(L, ptr->GetReturnType(), index);
//}
//
//static const struct luaL_Reg returnTypeMetaTableMethods[]=
//{
//	{"__eq", LUAGLUE_HELPERCLASS::__eq},
//	{"__gc", LUAGLUE_HELPERCLASS::luaGc},
//	{"__index", rtnTypeIndex},
//	{"__tostring", LUAGLUE_HELPERCLASS::__tostring},
//	{nullptr, nullptr}
//};
//
//int FunctionReturnTypeInfoOpenInLua(lua_State * L)
//{
//	DEBUGLOG_WRITE("FunctionReturnTypeInfo OpenInLua begins...");
//
//	luaL_getmetatable(L, PROPERTIES_METATABLENAME_RETURNTYPE);
//	if (lua_isnil(L, -1) != 1)
//	{
//		DEBUGLOG_WRITE("... SKIP! FunctionReturnTypeInfo");
//		return 0;
//	}
//	luaL_newmetatable(L, PROPERTIES_METATABLENAME_RETURNTYPE); // create metaTable
//	luaL_register(L, nullptr, returnTypeMetaTableMethods);
//
//	DEBUGLOG_WRITE("... FunctionReturnTypeInfo ends.");
//	return 1;
//}


END_NAMESPACE

#endif
