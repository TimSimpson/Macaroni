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
#ifndef MACARONI_MODEL_CPP_FUNCTIONLUA_CPP
#define MACARONI_MODEL_CPP_FUNCTIONLUA_CPP

#include <Macaroni/Model/Cpp/AccessLuaMetaData.h>
#include <Macaroni/Model/Element.h>
#include <Macaroni/Model/ElementLua.h>
#include "../NodeLua.h"
#include "../../Environment/DebugLog.h"
#include "FunctionLua.h"
#include <Macaroni/Model/Element.h>
#include "../ModelInconsistencyException.h"
#include "../NodeLua.h"
#include "../NodeListLua.h"
#include <Macaroni/Model/ReasonLuaMetaData.h>
#include <Macaroni/Model/SourceLuaMetaData.h>
#include "VariableLua.h"
#include "../TypeLua.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


#define GLOBALTABLENAME "Macaroni.Model.Cpp.Function"

struct lua_State;

using Macaroni::Model::Cpp::Access;
using Macaroni::Model::Cpp::AccessLuaMetaData;
using Macaroni::Model::Element;
using Macaroni::Model::ElementLuaMetaData;
using Macaroni::Model::Node;
using Macaroni::Model::NodeLuaMetaData;
using Macaroni::Model::ReasonLuaMetaData;
using Macaroni::Model::ReasonPtr;
using Macaroni::Model::Source;
using Macaroni::Model::SourceLuaMetaData;
using Macaroni::Model::SourcePtr;
using Macaroni::Model::Type;
using Macaroni::Model::TypeLuaMetaData;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

namespace 
{
	struct FunctionLuaFunctions
	{
		static int Create(lua_State * L) 
		{
			try
			{
				//static FunctionPtr Create(NodePtr home, bool isInline, 
				//	const Access access, const bool isStatic, const TypePtr rtnType, 
				//	bool constMember, Model::ReasonPtr reason);
				NodePtr home = NodeLuaMetaData::GetInstance(L, 1);
				ReasonPtr reason = ReasonLuaMetaData::GetInstance(L, 2);
				FunctionPtr newFunc = Function::Create(home, reason);
				ElementPtr rtnValue = boost::dynamic_pointer_cast<Element>(newFunc);
				ElementLuaMetaData::PutInstanceOnStack(L, rtnValue);
				return 1;
			} 
			catch(const ModelInconsistencyException & ex) 
			{
				return luaL_error(L, "ModelInconsistencyException: %c", ex.what()); 
			}
		}

		/*static int SetCodeBlock(lua_State * L)
		{
			MemberPtr memberPtr = MemberLuaMetaData::GetInstance(L, 1);
			FunctionPtr & ptr = boost::dynamic_pointer_cast<Function>(memberPtr);
			std::string codeBlock(luaL_checkstring(L, 2));
			SourcePtr src = SourceLuaMetaData::GetInstance(L, 3);
			ptr->SetCodeBlock(codeBlock, src);
			return 0;
		}*/

	}; // end FunctionLuaFunctions

	static const struct luaL_Reg tableMethods[]=
	{
		{"Create", FunctionLuaFunctions::Create},
		{nullptr, nullptr}
	};
} // end of anon namespace

ElementPtr & FunctionLuaMetaData::GetInstance(lua_State * L, int index)
{
	return ElementLuaMetaData::GetInstance(L, index);
}

bool FunctionLuaMetaData::IsType(lua_State * L, int index)
{
	return ElementLuaMetaData::IsType(L, index);
}

int FunctionLuaMetaData::OpenInLua(lua_State * L) 
{
	luaL_register(L, GLOBALTABLENAME, tableMethods);		
	return 1;
}

void FunctionLuaMetaData::PutInstanceOnStack(lua_State * L, const FunctionPtr & ptr)
{
	ElementPtr memberPtr = boost::dynamic_pointer_cast<Element>(ptr);
	ElementLuaMetaData::PutInstanceOnStack(L, memberPtr);
}

int FunctionLuaMetaData::Index(lua_State * L, const FunctionPtr & ptr, 
									  const std::string & index)
{		
	return 0;
}



END_NAMESPACE

#endif
