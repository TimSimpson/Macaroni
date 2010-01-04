#ifndef MACARONI_MODEL_CPP_FUNCTIONLUA_CPP
#define MACARONI_MODEL_CPP_FUNCTIONLUA_CPP

#include "../NodeLua.h"
#include "../../Environment/DebugLog.h"
#include "FunctionLua.h"
#include "../MemberLua.h"
#include "../NodeListLua.h"
#include "../TypeLua.h"
#include "VariableLua.h"

extern "C" {
	#include "../../../Lua/lua.h"
	#include "../../../Lua/lauxlib.h"
	#include "../../../Lua/lualib.h"
}


struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int FunctionLuaMetaData::Index(lua_State * L, const FunctionPtr & ptr, 
									  const std::string & index)
{		
	if (index == "Arguments")
	{
		NodeListPtr argList = ptr->GetArguments();
		Model::NodeListLuaMetaData::PutInstanceOnStack(L, argList);
		return 1;
	}
	else if (index == "CodeBlock")
	{
		lua_pushstring(L, ptr->GetCodeBlock().c_str());
		return 1;
	}
	else if (index == "Const")
	{
		lua_pushboolean(L, ptr->IsConst());
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
	return 0;
}

END_NAMESPACE

#endif
