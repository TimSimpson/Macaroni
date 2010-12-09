#ifndef MACARONI_MODEL_ATTRIBUTEVALUELUA_CPP
#define MACARONI_MODEL_ATTRIBUTEVALUELUA_CPP

#include "AttributeTable.h"
#include "AttributeTableLua.h"
#include "AttributeValue.h"
#include "AttributeValueLua.h"
#include "Node.h"
#include "NodeLua.h"
#include "ReasonLua.h"
#include <sstream>

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME AttributeValue
#define LUAGLUE_CLASSREFNAME AttributeValuePtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.AttributeValue"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::AttributeValue
#define LUAGLUE_REGISTRATIONCLASSNAME AttributeValueLuaMetaData
#define LUAGLUE_OPENOTHERMODULES \
	//Macaroni::Model::Cpp::ConstructorLuaMetaData::AssignmentListOpenInLua(L);
	//Macaroni::Model::NodeLuaMetaData::OpenInLua(L); \
	//Macaroni::Model::Cpp::FunctionLuaMetaData::OpenInLua(L); \
	//Macaroni::Model::Cpp::ClassLuaMetaData::OpenInLua(L);  \
	//Macaroni::Model::Cpp::TypedefLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE YESPLEASE
#define LUAGLUE_HELPERCLASS				AttributeValueLuaGlueHelperClass

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const AttributeValuePtr & ptr, 
									  const std::string & index)
	{		
		if (index == "ValueAsBool")
		{
			lua_pushboolean(L, ptr->GetValueAsBool());
		}
		else if (index == "ValueAsNode")
		{
			NodeLuaMetaData::PutInstanceOnStack(L, ptr->GetValueAsNode());
		}
		else if (index == "ValueAsNumber")
		{
			lua_pushnumber(L, ptr->GetValueAsNumber());
		}
		else if (index == "ValueAsString")
		{
			lua_pushstring(L, ptr->GetValueAsString().c_str());
		}
		else if (index == "ValueAsTable")
		{
			AttributeTableLuaMetaData::PutInstanceOnStack(L, ptr->GetValueAsTable());
		}
		else if (index == "IsBool")
		{
			lua_pushboolean(L, ptr->IsBool());
		}
		else if (index == "IsNode")
		{
			lua_pushboolean(L, ptr->IsNode());
		}
		else if (index == "IsNumber")
		{
			lua_pushboolean(L, ptr->IsNumber());
		}
		else if (index == "IsString")
		{
			lua_pushboolean(L, ptr->IsString());
		}
		else if (index == "IsTable")
		{
			lua_pushboolean(L, ptr->IsTable());
		}
		else if (index == "Name")
		{
			NodeLuaMetaData::PutInstanceOnStack(L, ptr->GetName());	
		}		
		else if (index == "TypeString")
		{
			lua_pushstring(L, ptr->GetTypeString().c_str());
		}
		else
		{
			lua_pushnil(L);			
		}
		
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		AttributeValuePtr & ptr = getInstance(L);
		std::stringstream ss;
		ss << "[AttributeValue Name=";
		ss << ptr->GetName()->GetFullName();
		ss << ", Type=";
		ss << ptr->GetTypeString();
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