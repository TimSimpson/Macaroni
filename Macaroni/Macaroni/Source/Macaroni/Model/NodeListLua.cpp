#ifndef MACARONI_MODEL_NODELISTLUA_CPP
#define MACARONI_MODEL_NODELISTLUA_CPP

#include "Cpp/ClassLua.h"
#include "Cpp/FunctionLua.h"
#include "MemberLua.h"
#include "Member.h"
#include "Node.h"
#include "NodeLua.h"
#include "NodeListLua.h"
#include "ReasonLua.h"
#include <sstream>
#include "Cpp/VariableLua.h"

#define LUAGLUE_STARTNAMESPACE			BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE			END_NAMESPACE2
#define LUAGLUE_CLASSNAME				NodeList
#define LUAGLUE_CLASSREFNAME			NodeListPtr
#define LUAGLUE_CLASSFULLLUANAME		"Macaroni.Model.NodeList"
#define LUAGLUE_CLASSFULLCPPNAME		Macaroni::Model::NodeList
#define LUAGLUE_REGISTRATIONCLASSNAME	NodeListLuaMetaData
#define LUAGLUE_OPENOTHERMODULES		Macaroni::Model::NodeLuaMetaData::OpenInLua(L); 
#define LUAGLUE_CREATEMETATABLE YESPLEASE
#define LUAGLUE_HELPERCLASS				NodeListLuaGlueHelperClass

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & indexAsString)
	{		
		unsigned int index = luaL_checkinteger(L, 2);
		if (index > 0 && index <= ptr->size())
		{
			index --;
			NodePtr node = (*ptr)[index];
			NodeLuaMetaData::PutInstanceOnStack(L, (*ptr)[index]);
			return 1;
		}
		lua_pushnil(L);			
		return 1;
	}

	static int __len(lua_State * L)
	{
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		int size = (*ptr).size();
		lua_pushnumber(L, size);
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		std::stringstream ss;
		ss << "NodeList";
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__len", LUAGLUE_HELPERCLASS::__len}, \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif