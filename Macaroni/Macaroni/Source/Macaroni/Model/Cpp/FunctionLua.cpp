#ifndef MACARONI_MODEL_CPP_FUNCTIONLUA_CPP
#define MACARONI_MODEL_CPP_FUNCTIONLUA_CPP

#include "AccessLua.h"
#include "../NodeLua.h"
#include "../../Environment/DebugLog.h"
#include "FunctionLua.h"
#include "../MemberLua.h"
#include "../NodeLua.h"
#include "../NodeListLua.h"
#include "../ReasonLua.h"
#include "../SourceLua.h"
#include "VariableLua.h"
#include "../TypeLua.h"

//MARIO extern "C" {
	#include "../../../Lua/lua.h"
	#include "../../../Lua/lauxlib.h"
	#include "../../../Lua/lualib.h"
//MARIO }


#define GLOBALTABLENAME "Macaroni.Model.Cpp.Function"

struct lua_State;

using Macaroni::Model::Cpp::Access;
using Macaroni::Model::Cpp::AccessLuaMetaData;
using Macaroni::Model::Member;
using Macaroni::Model::MemberLuaMetaData;
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
			//static FunctionPtr Create(NodePtr home, bool isInline, 
			//	const Access access, const bool isStatic, const TypePtr rtnType, 
			//	bool constMember, Model::ReasonPtr reason);
			NodePtr home = NodeLuaMetaData::GetInstance(L, 1);
			bool isInline = (bool) lua_toboolean(L, 2);
			Access access = AccessLuaMetaData::GetInstance(L, 3);
			bool isStatic = (bool) lua_toboolean(L, 4);
			TypePtr rtnType = TypeLuaMetaData::GetInstance(L, 5);
			bool constMember = (bool) lua_toboolean(L, 6);
			ReasonPtr reason = ReasonLuaMetaData::GetInstance(L, 7);
			FunctionPtr newFunc = Function::Create(home, isInline, access, 
												   isStatic, rtnType, 
												   constMember, reason);
			MemberPtr rtnValue = boost::dynamic_pointer_cast<Member>(newFunc);
			MemberLuaMetaData::PutInstanceOnStack(L, rtnValue);
			return 1;
		}

		static int SetCodeBlock(lua_State * L)
		{
			MemberPtr memberPtr = MemberLuaMetaData::GetInstance(L, 1);
			FunctionPtr & ptr = boost::dynamic_pointer_cast<Function>(memberPtr);
			std::string codeBlock(luaL_checkstring(L, 2));
			SourcePtr src = SourceLuaMetaData::GetInstance(L, 3);
			ptr->SetCodeBlock(codeBlock, src);
			return 0;
		}

	}; // end FunctionLuaFunctions

	static const struct luaL_Reg tableMethods[]=
	{
		{"Create", FunctionLuaFunctions::Create},
		{nullptr, nullptr}
	};
} // end of anon namespace

MemberPtr & FunctionLuaMetaData::GetInstance(lua_State * L, int index)
{
	return MemberLuaMetaData::GetInstance(L, index);
}

bool FunctionLuaMetaData::IsType(lua_State * L, int index)
{
	return MemberLuaMetaData::IsType(L, index);
}

int FunctionLuaMetaData::OpenInLua(lua_State * L) 
{
	luaL_register(L, GLOBALTABLENAME, tableMethods);		
	return 1;
}

void FunctionLuaMetaData::PutInstanceOnStack(lua_State * L, const MemberPtr & ptr)
{
	MemberLuaMetaData::PutInstanceOnStack(L, ptr);
}

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
		lua_pushcfunction(L, FunctionLuaFunctions::SetCodeBlock);
		return 1;
	}		
	return 0;
}



END_NAMESPACE

#endif
