#ifndef MACARONI_MODEL_CPP_VARIABLELUA_CPP
#define MACARONI_MODEL_CPP_VARIABLELUA_CPP

#include "../NodeLua.h"
#include "VariableLua.h"
#include "../TypeLua.h"

extern "C" {
	#include "../../../Lua/lua.h"
	#include "../../../Lua/lauxlib.h"
	#include "../../../Lua/lualib.h"
}

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int VariableLuaMetaData::Index(lua_State * L, const VariablePtr var, 
								const std::string & index)
{
	///*if (index == "Const")
	//{
	//	lua_pushboolean(L, var->GetType()->IsConst() ? 1 : 0);
	//	return 1;
	//}
	//else if (index == "ConstPointer")
	//{
	//	lua_pushboolean(L, var->GetType()->IsConstPointer() ? 1 : 0);
	//	return 1;
	//}
	//else if (index == "Pointer")
	//{
	//	lua_pushboolean(L, var->GetType()->IsPointer() ? 1 : 0);
	//	return 1;
	//}
	//else if (index == "Reference")
	//{
	//	lua_pushboolean(L, var->GetType()->IsReference() ? 1 : 0);
	//	return 1;
	//}
	//else if (index == "TypeNode")
	//{
	//	Model::NodeLuaMetaData::PutInstanceOnStack(L, var->GetType()->GetNode());
	//	return 1;
	//}*/
	if (index == "Initializer")
	{
		lua_pushstring(L, var->GetInitializer().c_str());
		return 1;
	}
	else if (index == "Type")
	{
		Model::TypeLuaMetaData::PutInstanceOnStack(L, var->GetType());
		return 1;
	}
	return 0;
}


END_NAMESPACE

#endif
