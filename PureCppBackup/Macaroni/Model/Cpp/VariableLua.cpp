#ifndef MACARONI_MODEL_CPP_VARIABLELUA_CPP
#define MACARONI_MODEL_CPP_VARIABLELUA_CPP

#include "AccessLua.h"
#include "../NodeLua.h"
#include "../../Environment/DebugLog.h"
#include "FunctionLua.h"
#include "../MemberLua.h"
#include "../ModelInconsistencyException.h"
#include "../NodeLua.h"
#include "../NodeListLua.h"
#include "../ReasonLua.h"
#include "VariableLua.h"
#include "../TypeLua.h"

//MARIO extern "C" {
	#include "../../../Lua/lua.h"
	#include "../../../Lua/lauxlib.h"
	#include "../../../Lua/lualib.h"
//MARIO }

#define GLOBALTABLENAME "Macaroni.Model.Cpp.Variable"

struct lua_State;

using Macaroni::Model::Cpp::Access;
using Macaroni::Model::Cpp::AccessLuaMetaData;
using Macaroni::Model::Member;
using Macaroni::Model::MemberLuaMetaData;
using Macaroni::Model::Node;
using Macaroni::Model::NodeLuaMetaData;
using Macaroni::Model::ReasonLuaMetaData;
using Macaroni::Model::ReasonPtr;
using Macaroni::Model::Type;
using Macaroni::Model::TypeLuaMetaData;
using Macaroni::Model::Cpp::Variable;
using Macaroni::Model::Cpp::VariablePtr;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

namespace 
{
	struct LuaWrappers
	{
		static int Create(lua_State * L) 
		{
			try
			{
				//static VariablePtr Create(NodePtr home, Access access, 
				//	bool isStatic, const TypePtr type, std::string initializer, 
				//Model::ReasonPtr reason);
				NodePtr home = NodeLuaMetaData::GetInstance(L, 1);
				Access access = AccessLuaMetaData::GetInstance(L, 2);
				bool isStatic = lua_toboolean(L, 3) != 0;
				TypePtr type = TypeLuaMetaData::GetInstance(L, 4);
				std::string initializer(luaL_checkstring(L, 5));			
				ReasonPtr reason = ReasonLuaMetaData::GetInstance(L, 6);
				VariablePtr variable = Variable::Create(home, access,
														isStatic, type, 
														initializer, reason);
				MemberPtr rtnValue = boost::dynamic_pointer_cast<Member>(variable);
				MemberLuaMetaData::PutInstanceOnStack(L, rtnValue);
				return 1;
			} 
			catch(const ModelInconsistencyException & ex) 
			{
				luaL_error(L, "ModelInconsistencyException: %c", ex.what()); 
			}
		}

	}; // end FunctionLuaFunctions

	static const struct luaL_Reg tableMethods[]=
	{
		{"Create", LuaWrappers::Create},
		{nullptr, nullptr}
	};
} // end of anon namespace


MemberPtr & VariableLuaMetaData::GetInstance(lua_State * L, int index)
{
	return MemberLuaMetaData::GetInstance(L, index);
}

bool VariableLuaMetaData::IsType(lua_State * L, int index)
{
	return MemberLuaMetaData::IsType(L, index);
}


int VariableLuaMetaData::OpenInLua(lua_State * L) 
{
	luaL_register(L, GLOBALTABLENAME, tableMethods);		
	return 1;
}


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