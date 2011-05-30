#ifndef MACARONI_MODEL_CPP_FUNCTIONLUA_CPP
#define MACARONI_MODEL_CPP_FUNCTIONLUA_CPP

#include "AccessLua.h"
#include "../NodeLua.h"
#include "../../Environment/DebugLog.h"
#include "FunctionLua.h"
#include "../MemberLua.h"
#include "../ModelInconsistencyException.h"
#include "../NodeLua.h"
#include "../NodeListLua.h"
#include "../ReasonLua.h"
#include "../SourceLua.h"
#include "VariableLua.h"
#include "../TypeLua.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


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
			try
			{
				//static FunctionPtr Create(NodePtr home, bool isInline, 
				//	const Access access, const bool isStatic, const TypePtr rtnType, 
				//	bool constMember, Model::ReasonPtr reason);
				NodePtr home = NodeLuaMetaData::GetInstance(L, 1);
				ReasonPtr reason = ReasonLuaMetaData::GetInstance(L, 2);
				FunctionPtr newFunc = Function::Create(home, reason);
				MemberPtr rtnValue = boost::dynamic_pointer_cast<Member>(newFunc);
				MemberLuaMetaData::PutInstanceOnStack(L, rtnValue);
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

void FunctionLuaMetaData::PutInstanceOnStack(lua_State * L, const FunctionPtr & ptr)
{
	MemberPtr memberPtr = boost::dynamic_pointer_cast<Member>(ptr);
	MemberLuaMetaData::PutInstanceOnStack(L, memberPtr);
}

int FunctionLuaMetaData::Index(lua_State * L, const FunctionPtr & ptr, 
									  const std::string & index)
{		
	return 0;
}



END_NAMESPACE

#endif
