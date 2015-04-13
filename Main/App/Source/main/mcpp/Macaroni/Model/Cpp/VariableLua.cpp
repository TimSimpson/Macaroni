/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MACARONI_MODEL_CPP_VARIABLELUA_CPP
#define MACARONI_MODEL_CPP_VARIABLELUA_CPP

#include <Macaroni/Model/Cpp/AccessLuaMetaData.h>
#include "../NodeLua.h"
#include "../ElementLua.h"
#include "FunctionLua.h"
#include "../ModelInconsistencyException.h"
#include "../NodeLua.h"
#include "../NodeListLua.h"
#include <Macaroni/Model/ReasonLuaMetaData.h>
#include "VariableLua.h"
#include <Macaroni/Model/Project/Target.h>
#include <Macaroni/Model/Type.h>
#include <Macaroni/Model/TypeLuaMetaData.h>

#include <Macaroni/LuaCompat.h>
#include <Macaroni/Lua.h>

#define GLOBALTABLENAME "Macaroni.Model.Cpp.Variable"

struct lua_State;

using Macaroni::Model::Cpp::Access;
using Macaroni::Model::Cpp::AccessLuaMetaData;
using Macaroni::Model::Element;
using Macaroni::Model::ElementLuaMetaData;
using Macaroni::Model::Node;
using Macaroni::Model::NodeLuaMetaData;
using Macaroni::Model::ReasonLuaMetaData;
using Macaroni::Model::ReasonPtr;
using Macaroni::Model::Project::Target;
using Macaroni::Model::Project::TargetPtr;
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
				AccessPtr access = AccessLuaMetaData::GetInstance(L, 2);
				bool isStatic = lua_toboolean(L, 3) != 0;
				TypePtr type = TypeLuaMetaData::GetInstance(L, 4);
				std::string initializer(luaL_checkstring(L, 5));
				ReasonPtr reason = ReasonLuaMetaData::GetInstance(L, 6);
				TargetPtr tHome;
				VariablePtr variable = Variable::Create(tHome, home, access,
														isStatic, type,
														initializer, reason);
				ElementPtr rtnValue = boost::dynamic_pointer_cast<Element>(variable);
				ElementLuaMetaData::PutInstanceOnStack(L, rtnValue);
				return 1;
			}
			catch(const ModelInconsistencyException & ex)
			{
				return luaL_error(L, "ModelInconsistencyException: %c", ex.what());
			}
		}

	}; // end FunctionLuaFunctions

	static const struct luaL_Reg tableMethods[]=
	{
		{"Create", LuaWrappers::Create},
		{nullptr, nullptr}
	};
} // end of anon namespace


ElementPtr & VariableLuaMetaData::GetInstance(lua_State * L, int index)
{
	return ElementLuaMetaData::GetInstance(L, index);
}

bool VariableLuaMetaData::IsType(lua_State * L, int index)
{
	return ElementLuaMetaData::IsType(L, index);
}


int VariableLuaMetaData::OpenInLua(lua_State * L)
{
	MACARONI_LUA_REGISTER_FOR_RETURN(L, GLOBALTABLENAME, tableMethods);
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
