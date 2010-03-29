#ifndef MACARONI_MODEL_ATTRIBUTETABLELUA_CPP
#define MACARONI_MODEL_ATTRIBUTETABLELUA_CPP

#include "AttributeTable.h"
#include "AttributeValue.h"
#include "AttributeTableLua.h"
#include "AttributeValueLua.h"
#include "Node.h"
#include "NodeLua.h"
#include "ReasonLua.h"
#include <sstream>

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME AttributeTable
#define LUAGLUE_CLASSREFNAME AttributeTablePtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.AttributeTable"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::AttributeTable
#define LUAGLUE_REGISTRATIONCLASSNAME AttributeTableLuaMetaData
#define LUAGLUE_OPENOTHERMODULES /** F'no */
#define LUAGLUE_CREATEMETATABLE YESPLEASE
#define LUAGLUE_HELPERCLASS				AttributeTableLuaGlueHelperClass

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const AttributeTablePtr & ptr, 
									  const std::string & index)
	{		
		AttributeValuePtr value = ptr->GetByName(index);
		if (!!value)
		{
			AttributeValueLuaMetaData::PutInstanceOnStack(L, value);
			return 1;
		}		
		
		// Currently only Strings allowed here due to how I this Macro
		// madness works. I'll change that later...

		lua_pushnil(L);			
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		AttributeTablePtr & ptr = getInstance(L);
		std::stringstream ss;
		ss << "[AttributeTable, Home Node=";
		ss << ptr->GetHomeNode()->GetFullName();
		ss << "]";
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif