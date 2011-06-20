#ifndef MACARONI_MODEL_CPP_FUNCTIONOVERLOADLUA_CPP
#define MACARONI_MODEL_CPP_FUNCTIONOVERLOADLUA_CPP

#include <Macaroni/Model/Cpp/AccessLuaMetaData.h>
#include "../NodeLua.h"
#include "../../Environment/DebugLog.h"
#include "FunctionLua.h"
#include "FunctionOverloadLua.h"
#include "../MemberLua.h"
#include "../ModelInconsistencyException.h"
#include "../NodeLua.h"
#include "../NodeListLua.h"
#include <Macaroni/Model/ReasonLuaMetaData.h>
#include "../SourceLua.h"
#include "VariableLua.h"
#include "../TypeLua.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


#define GLOBALTABLENAME "Macaroni.Model.Cpp.FunctionOverload"

struct lua_State;

using Macaroni::Model::Cpp::Access;
using Macaroni::Model::Cpp::AccessLuaMetaData;
using Macaroni::Model::Cpp::Function;
using Macaroni::Model::Cpp::FunctionLuaMetaData;
using Macaroni::Model::Cpp::FunctionPtr;
using Macaroni::Model::Cpp::FunctionOverload;
using Macaroni::Model::Cpp::FunctionOverloadPtr;
using Macaroni::Model::Member;
using Macaroni::Model::MemberLuaMetaData;
using Macaroni::Model::MemberPtr;
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
				MemberPtr memberPtr = FunctionLuaMetaData::GetInstance(L, 1);
				FunctionPtr home = 
					boost::dynamic_pointer_cast<Function>(memberPtr);
				if (!home)
				{
					luaL_error(L, "Expected first argument to be a Function.");
				}
				bool isInline = lua_toboolean(L, 2) != 0;
				AccessPtr access = AccessLuaMetaData::GetInstance(L, 3);
				bool isStatic = lua_toboolean(L, 4) != 0;
				TypePtr rtnType = TypeLuaMetaData::GetInstance(L, 5);
				bool constMember = lua_toboolean(L, 6) != 0;
				ReasonPtr reason = ReasonLuaMetaData::GetInstance(L, 7);
				FunctionOverloadPtr newFO = FunctionOverload::Create(home, 
													   isInline, access, 
													   isStatic, rtnType, 
													   constMember, reason);
				MemberPtr rtnValue = boost::dynamic_pointer_cast<Member>(newFO);
				MemberLuaMetaData::PutInstanceOnStack(L, rtnValue);
				return 1;
			} 
			catch(const ModelInconsistencyException & ex) 
			{
				return luaL_error(L, "ModelInconsistencyException: %c", ex.what()); 
			}
		}

		static int SetCodeBlock(lua_State * L)
		{
			MemberPtr memberPtr = MemberLuaMetaData::GetInstance(L, 1);
			FunctionOverloadPtr & ptr = boost::dynamic_pointer_cast<FunctionOverload>(memberPtr);
			std::string codeBlock(luaL_checkstring(L, 2));
			SourcePtr src = SourceLuaMetaData::GetInstance(L, 3);
			ptr->SetCodeBlock(codeBlock, src);
			return 0;
		}

	}; // end FunctionLuaFunctions

	static const struct luaL_Reg tableMethods[]=
	{
		{"Create", FunctionOverloadLuaFunctions::Create},
		{nullptr, nullptr}
	};
} // end of anon namespace

MemberPtr & FunctionOverloadLuaMetaData::GetInstance(lua_State * L, int index)
{
	return MemberLuaMetaData::GetInstance(L, index);
}

bool FunctionOverloadLuaMetaData::IsType(lua_State * L, int index)
{
	return MemberLuaMetaData::IsType(L, index);
}

int FunctionOverloadLuaMetaData::OpenInLua(lua_State * L) 
{
	luaL_register(L, GLOBALTABLENAME, tableMethods);		
	return 1;
}

void FunctionOverloadLuaMetaData::PutInstanceOnStack(lua_State * L, const FunctionOverloadPtr & ptr)
{
	MemberPtr memberPtr = boost::dynamic_pointer_cast<Member>(ptr);
	MemberLuaMetaData::PutInstanceOnStack(L, memberPtr);
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
	else if (index == "Const")
	{
		lua_pushboolean(L, ptr->IsConst());
		return 1;
	}
	else if (index == "Inline")
	{
		lua_pushboolean(L, ptr->IsInline());
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
	return 0;
}



END_NAMESPACE

#endif
