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
#ifndef MACARONI_MODEL_CPP_FUNCTIONOVERLOADLUA_CPP
#define MACARONI_MODEL_CPP_FUNCTIONOVERLOADLUA_CPP

#include <Macaroni/Model/Cpp/AccessLuaMetaData.h>
#include "../NodeLua.h"
#include "../ElementLua.h"
#include "FunctionLua.h"
#include "FunctionOverloadLua.h"
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


#define GLOBALTABLENAME "Macaroni.Model.Cpp.FunctionOverload"

struct lua_State;

using Macaroni::Model::Cpp::Access;
using Macaroni::Model::Cpp::AccessLuaMetaData;
using Macaroni::Model::Element;
using Macaroni::Model::ElementLuaMetaData;
using Macaroni::Model::Cpp::Function;
using Macaroni::Model::Cpp::FunctionLuaMetaData;
using Macaroni::Model::Cpp::FunctionPtr;
using Macaroni::Model::Cpp::FunctionOverload;
using Macaroni::Model::Cpp::FunctionOverloadPtr;
using Macaroni::Model::ElementPtr;
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
	struct FunctionOverloadLuaFunctions
	{
		static int Create(lua_State * L)
		{
			try
			{
				//static FunctionPtr Create(NodePtr home, bool isInline,
				//	const Access access, const bool isStatic, const TypePtr rtnType,
				//	bool constMember, Model::ReasonPtr reason);
				ElementPtr memberPtr = FunctionLuaMetaData::GetInstance(L, 1);
				FunctionPtr home =
					boost::dynamic_pointer_cast<Function>(memberPtr);
				if (!home)
				{
					luaL_error(L, "Expected first argument to be a Function.");
				}
				bool isInline = lua_toboolean(L, 2) != 0;
				AccessPtr access = AccessLuaMetaData::GetInstance(L, 3);
				bool isStatic = lua_toboolean(L, 4) != 0;
				bool isVirtual = lua_toboolean(L, 5) != 0;
				TypePtr rtnType = TypeLuaMetaData::GetInstance(L, 6);
				bool constMember = lua_toboolean(L, 7) != 0;
				boost::optional<ExceptionSpecifier> exceptionSpecifier;
				if (lua_isstring(L, 8))
				{
					const std::string value(lua_tostring(L, 8));
					if (value == "throws()")
					{
						exceptionSpecifier = ExceptionSpecifier::EmptyThrows();
					}
					else if (value == "BOOST_NOEXCEPT")
					{
						exceptionSpecifier = ExceptionSpecifier::BoostNoExcept();
					}
					else if (value == "noexcept")
					{
						exceptionSpecifier = ExceptionSpecifier::NoExcept();
					}
				}
				ReasonPtr reason = ReasonLuaMetaData::GetInstance(L, 9);
				FunctionOverloadPtr newFO = FunctionOverload::Create(home,
													   isInline, access,
													   isStatic, isVirtual,
													   rtnType,
													   constMember,
													   exceptionSpecifier,
													   reason);
				ElementPtr rtnValue = boost::dynamic_pointer_cast<Element>(newFO);
				ElementLuaMetaData::PutInstanceOnStack(L, rtnValue);
				return 1;
			}
			catch(const ModelInconsistencyException & ex)
			{
				return luaL_error(L, "ModelInconsistencyException: %c", ex.what());
			}
		}

		static int SetCodeBlock(lua_State * L)
		{
			ElementPtr memberPtr = ElementLuaMetaData::GetInstance(L, 1);
			FunctionOverloadPtr ptr = boost::dynamic_pointer_cast<FunctionOverload>(memberPtr);
			std::string codeBlock(luaL_checkstring(L, 2));
			SourcePtr src = SourceLuaMetaData::GetInstance(L, 3);
			bool codeBlockAddRedirect = lua_toboolean(L, 4) != 0;
			ptr->SetCodeBlock(codeBlock, src, codeBlockAddRedirect);
			return 0;
		}

	}; // end FunctionLuaFunctions

	static const struct luaL_Reg tableMethods[]=
	{
		{"Create", FunctionOverloadLuaFunctions::Create},
		{nullptr, nullptr}
	};
} // end of anon namespace

ElementPtr & FunctionOverloadLuaMetaData::GetInstance(lua_State * L, int index)
{
	return ElementLuaMetaData::GetInstance(L, index);
}

bool FunctionOverloadLuaMetaData::IsType(lua_State * L, int index)
{
	return ElementLuaMetaData::IsType(L, index);
}

int FunctionOverloadLuaMetaData::OpenInLua(lua_State * L)
{
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	return 1;
}

void FunctionOverloadLuaMetaData::PutInstanceOnStack(lua_State * L, const FunctionOverloadPtr & ptr)
{
	ElementPtr memberPtr = boost::dynamic_pointer_cast<Element>(ptr);
	ElementLuaMetaData::PutInstanceOnStack(L, memberPtr);
}

int FunctionOverloadLuaMetaData::Index(lua_State * L,
									   const FunctionOverloadPtr & ptr,
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
	else if (index == "CodeBlockShouldAddRedirect")
	{
		lua_pushboolean(L, ptr->CodeBlockShouldAddRedirect());
		return 1;
	}
	else if (index == "CodeSource")
	{
		SourceLuaMetaData::PutInstanceOnStack(L, ptr->GetCodeSource());
		return 1;
	}
	else if (index == "Const")
	{
		lua_pushboolean(L, ptr->IsConst());
		return 1;
	}
	else if (index == "HasCodeBlock")
	{
		lua_pushboolean(L, ptr->HasCodeBlock());
		return 1;
	}
	else if (index == "Inline")
	{
		lua_pushboolean(L, ptr->IsInline());
		return 1;
	}
	else if (index == "IsPureVirtual")
	{
		lua_pushboolean(L, ptr->IsPureVirtual());
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
	else if (index == "SetCodeBlock")
	{
		lua_pushcfunction(L, FunctionOverloadLuaFunctions::SetCodeBlock);
		return 1;
	}
	else if (index == "ExceptionSpecifier")
	{
		auto es = ptr->GetExceptionSpecifier();
		if (!es)
		{
			lua_pushnil(L);
		}
		else
		{
			std::string text = es.get().GetCodeText();
			lua_pushstring(L, text.c_str());
		}
		return 1;
	}
	else if (index == "Virtual")
	{
		lua_pushboolean(L, ptr->IsVirtual());
		return 1;
	}
	return 0;
}



END_NAMESPACE

#endif
