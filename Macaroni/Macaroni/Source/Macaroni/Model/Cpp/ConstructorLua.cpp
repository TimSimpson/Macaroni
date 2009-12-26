#ifndef MACARONI_MODEL_CPP_CONSTRUCTORLUA_CPP
#define MACARONI_MODEL_CPP_CONSTRUCTORLUA_CPP

#include "../NodeLua.h"
#include "ConstructorLua.h"
#include "../../Environment/DebugLog.h"
#include "FunctionLua.h"
#include "../MemberLua.h"
#include "TypeInfoLua.h"
#include "VariableLua.h"
#include "VariableAssignmentLua.h"

extern "C" {
	#include "../../../Lua/lua.h"
	#include "../../../Lua/lauxlib.h"
	#include "../../../Lua/lualib.h"
}


struct lua_State;

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE(Macaroni, Model, Cpp)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE
#define LUAGLUE_CLASSNAME Constructor
#define LUAGLUE_CLASSREFNAME ConstructorPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.Cpp.Constructor"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::Cpp::Constructor
#define LUAGLUE_REGISTRATIONCLASSNAME ConstructorLuaMetaData
#define LUAGLUE_HELPERCLASS ConstructorLuaFunctions
#define LUAGLUE_OPENOTHERMODULES //Macaroni::Model::NodeLuaMetaData::OpenInLua(L); FunctionLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE 

#define ASSIGNMENTS_METATABLENAME_ARGUMENTS "Macaroni.Model.Cpp.Constructor.Assignments"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int ConstructorAssignmentsOpenInLua(lua_State * L);

END_NAMESPACE

#include "../../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "Assignments")
		{
			createLUAGLUE_CLASSREFNAMEUserData(L, ptr);
			luaL_getmetatable(L, ASSIGNMENTS_METATABLENAME_ARGUMENTS);
			lua_setmetatable(L, -2); 
			return 1;
		}
		

		lua_pushnil(L);			
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		lua_pushstring(L, "Constructor");
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		/*{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, */

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../../LuaGlue2.hpp"


BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int assignmentsIndex(lua_State * L)
{
	ConstructorPtr & ptr = getInstance(L, 1, ASSIGNMENTS_METATABLENAME_ARGUMENTS);
	int index = luaL_checkinteger(L, 2);
	VariableAssignmentPtr luaData(new VariableAssignment(ptr->GetAssignment(index)));
	Macaroni::Model::Cpp::VariableAssignmentLuaMetaData::PutInstanceOnStack(L, luaData);
	return 1;
}

int assignmentsLen(lua_State * L)
{
	ConstructorPtr & ptr = getInstance(L, 1, ASSIGNMENTS_METATABLENAME_ARGUMENTS);
	lua_pushinteger(L, ptr->GetArgumentCount());
	return 1;
}

static const struct luaL_Reg assignmentsMetaTableMethods[]=
{
	{"__eq", LUAGLUE_HELPERCLASS::__eq},
	{"__gc", LUAGLUE_HELPERCLASS::luaGc},
	{"__index", assignmentsIndex},
	{"__len", assignmentsLen},
	{"__tostring", LUAGLUE_HELPERCLASS::__tostring},
};

int ConstructorAssignmentsOpenInLua(lua_State * L)
{
	DEBUGLOG_WRITE("ConstructorAssignmentsOpenInLua begins...");

	luaL_getmetatable(L, ASSIGNMENTS_METATABLENAME_ARGUMENTS);
	if (lua_isnil(L, -1) != 1)
	{
		return 0;
	}
	luaL_newmetatable(L, ASSIGNMENTS_METATABLENAME_ARGUMENTS); // create metaTable
	luaL_register(L, nullptr, assignmentsMetaTableMethods);

	DEBUGLOG_WRITE("... ConstructorAssignments open in lua ends...");
	return 1;
}



END_NAMESPACE

#endif
