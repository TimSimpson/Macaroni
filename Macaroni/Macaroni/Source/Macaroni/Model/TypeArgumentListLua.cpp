#ifndef MACARONI_MODEL_TYPEARGUMENTLISTLUA_CPP
#define MACARONI_MODEL_TYPEARGUMENTLISTLUA_CPP

#include "Node.h"
#include "NodeLua.h"
#include "NodeListLua.h"
#include "TypeArgumentLua.h"
#include "TypeArgument.h"
#include "TypeArgumentListLua.h"
#include <sstream>

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME TypeArgumentList
#define LUAGLUE_CLASSREFNAME TypeArgumentListPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.TypeArgumentList"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::TypeArgumentList
#define LUAGLUE_REGISTRATIONCLASSNAME TypeArgumentListLuaMetaData
#define LUAGLUE_OPENOTHERMODULES Macaroni::Model::TypeArgumentLuaMetaData::OpenInLua(L), Macaroni::Model::NodeLuaMetaData::OpenInLua(L); Macaroni::Model::NodeListLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE YESPLEASE
#define LUAGLUE_HELPERCLASS				TypeArgumentListLuaGlueHelperClass

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & indexString)
	{	
		unsigned int index = luaL_checkinteger(L, 2);
		if (index > 0 && index <= ptr->size())
		{
			index --;
			TypeArgumentPtr node = (*ptr)[index];
			TypeArgumentLuaMetaData::PutInstanceOnStack(L, node);
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
		TypeArgumentListPtr & ptr = getInstance(L);
		std::stringstream ss;
	
		ss << " [TODO-stuff])";			
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__len", LUAGLUE_HELPERCLASS::__len},	\
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif