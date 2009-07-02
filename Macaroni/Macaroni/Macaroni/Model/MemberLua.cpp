#ifndef MACARONI_MODEL_MEMBERLUA_CPP
#define MACARONI_MODEL_MEMBERLUA_CPP

#include "MemberLua.h"
#include "Member.h"
#include "Node.h"
#include "NodeLua.h"
#include "ReasonLua.h"
#include <sstream>

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME Member
#define LUAGLUE_CLASSREFNAME MemberPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.Member"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::Member
#define LUAGLUE_REGISTRATIONCLASSNAME MemberLuaMetaData
#define LUAGLUE_OPENOTHERMODULES Macaroni::Model::NodeLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE YESPLEASE

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "Node")
		{
			NodeLuaMetaData::PutInstanceOnStack(L, ptr->GetNode());			
		}
		else if (index == "ReasonCreated")
		{
			ReasonLuaMetaData::PutInstanceOnStack(L, ptr->GetReasonCreated());
		}
		else if (index == "TypeName")
		{
			lua_pushstring(L, ptr->GetTypeName());
		}
		else 
		{
			lua_pushnil(L);			
		}
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		std::stringstream ss;
		ss << "[";
		ss << ptr->GetTypeName();
		ss << "]";
		ss << ptr->GetNode()->GetFullName();			
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_CLASSNAMELuaFunctions::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_CLASSNAMELuaFunctions::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif