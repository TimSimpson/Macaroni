#ifndef MACARONI_MODEL_BLOCKLUA_CPP
#define MACARONI_MODEL_BLOCKLUA_CPP

#include "BlockLua.h"
#include "NodeLua.h"
#include "../Environment/DebugLog.h"
#include "MemberLua.h"
#include "NodeLua.h"
#include "NodeListLua.h"
#include "ReasonLua.h"
#include "SourceLua.h"
#include "TypeLua.h"

//MARIO extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
//MARIO }

#define GLOBALTABLENAME "Macaroni.Model.Block"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

namespace 
{
	struct globalFunctions
	{
		static int Create(lua_State * L) 
		{
			//static BlockPtr Create(NodePtr host, const std::string & id, 
			//		const std::string & block,	
			//		const ReasonPtr reasonCreated);
			NodePtr home = NodeLuaMetaData::GetInstance(L, 1);
			std::string id(luaL_checkstring(L, 2));
			std::string block(luaL_checkstring(L, 3));
			ReasonPtr reason = ReasonLuaMetaData::GetInstance(L, 4);
			BlockPtr newBlock = Block::Create(home, id, block, reason);
			MemberPtr rtnValue = boost::dynamic_pointer_cast<Member>(newBlock);
			MemberLuaMetaData::PutInstanceOnStack(L, rtnValue);
			return 1;
		}
	}; // end struct

	static const struct luaL_Reg tableMethods[]=
	{
		{"Create", globalFunctions::Create},
		{nullptr, nullptr}
	};
}

MemberPtr & BlockLuaMetaData::GetInstance(lua_State * L, int index)
{
	return MemberLuaMetaData::GetInstance(L, index);
}

bool BlockLuaMetaData::IsType(lua_State * L, int index)
{
	return MemberLuaMetaData::IsType(L, index);
}

int BlockLuaMetaData::Index(lua_State * L, const BlockPtr & ptr, 
									  const std::string & index)
{		
	if (index == "Code")
	{
		lua_pushstring(L, ptr->GetCode().c_str());
		return 1;
	}
	else if (index == "Id")
	{
		lua_pushstring(L, ptr->GetId().c_str());
		return 1;
	}		
	return 0;
}

int BlockLuaMetaData::OpenInLua(lua_State * L) 
{
	luaL_register(L, GLOBALTABLENAME, tableMethods);		
	return 1;
}

void BlockLuaMetaData::PutInstanceOnStack(lua_State * L, const MemberPtr & ptr)
{
	MemberPtr memberPtr = boost::dynamic_pointer_cast<Member>(ptr);
	MemberLuaMetaData::PutInstanceOnStack(L, memberPtr);
}

END_NAMESPACE2

#endif
