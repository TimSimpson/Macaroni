#ifndef MACARONI_MODEL_TYPEARGUMENTLUA_CPP
#define MACARONI_MODEL_TYPEARGUMENTLUA_CPP

#include "Node.h"
#include "NodeLua.h"
#include "NodeListLua.h"
#include "TypeArgumentLua.h"
#include "TypeArgument.h"
#include <sstream>

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME TypeArgument
#define LUAGLUE_CLASSREFNAME TypeArgumentPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.TypeArgument"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::TypeArgument
#define LUAGLUE_REGISTRATIONCLASSNAME TypeArgumentLuaMetaData
#define LUAGLUE_OPENOTHERMODULES Macaroni::Model::NodeLuaMetaData::OpenInLua(L); Macaroni::Model::NodeListLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE YESPLEASE
#define LUAGLUE_HELPERCLASS				TypeArgumentLuaGlueHelperClass

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{		
		if (index == "Arguments")
		{
			NodeListPtr nodeList = ptr->GetArguments();
			NodeListLuaMetaData::PutInstanceOnStack(L, nodeList);
			return 1;
		}
		else if (index == "Node")
		{
			NodeLuaMetaData::PutInstanceOnStack(L, ptr->GetNode());
			return 1;
		}
		lua_pushnil(L);			
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		TypeArgumentPtr & ptr = getInstance(L);
		std::stringstream ss;
		ss << "(";
		ss << ptr->GetNode()->GetFullName();
		ss << " [TODO-list arguments here])";			
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif