#ifndef MACARONI_MODEL_TYPELUA_CPP
#define MACARONI_MODEL_TYPELUA_CPP

#include "Node.h"
#include "NodeLua.h"
#include "NodeListLua.h"
#include "Type.h"
#include "TypeLua.h"
#include "TypeArgumentListLua.h"
#include <sstream>

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME Type
#define LUAGLUE_CLASSREFNAME TypePtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.Type"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::Type
#define LUAGLUE_REGISTRATIONCLASSNAME TypeLuaMetaData
#define LUAGLUE_OPENOTHERMODULES // Macaroni::Model::NodeLuaMetaData::OpenInLua(L); Macaroni::Model::NodeListLuaMetaData::OpenInLua(L); Macaroni::Model::TypeArgumentListLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE YESPLEASE
#define LUAGLUE_HELPERCLASS				TypeLuaGlueHelperClass

#include "../LuaGlue.hpp"

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{				
		if (index == "Const")
		{
			lua_pushboolean(L, ptr->IsConst());
			return 1;
		}
		else if (index == "ConstPointer")
		{
			lua_pushboolean(L, ptr->IsConstPointer());
			return 1;
		}
		else if (index == "Pointer")
		{
			lua_pushboolean(L, ptr->IsPointer());
			return 1;
		}
		else if (index == "Reference")
		{
			lua_pushboolean(L, ptr->IsReference());
			return 1;
		}
		else if (index == "Node")
		{
			NodeLuaMetaData::PutInstanceOnStack(L, ptr->GetNode());
			return 1;
		}
		else if (index == "TypeArguments")
		{
			TypeArgumentListPtr list = ptr->GetTypeArguments();
			TypeArgumentListLuaMetaData::PutInstanceOnStack(L, list);
			return 1;
		}
		lua_pushnil(L);			
		return 1;
	}

	static int __new(lua_State * L)
	{
		if (!NodeLuaMetaData::IsType(L, 1))
		{
			lua_pushstring(L, "Expected a Node for argument #1 in Type creator.");
			lua_error(L);
		}		

		NodePtr node = NodeLuaMetaData::GetInstance(L, 1);				

		TypePtr type;
		
		TypeModifiers modifiers;
		const int tableIndex = 2;
		
		if (lua_istable(L, tableIndex))
		{
			luaL_checktype(L, tableIndex, LUA_TTABLE);

			lua_pushnil(L); // p39, start of table iteratin'
			while(lua_next(L, tableIndex) != 0)
			{
				std::string key(lua_tostring(L, -2)); // key
				bool value = lua_toboolean(L, -1); // value
				if (key == "Const")
				{
					modifiers.Const = value;
				}
				else if (key == "ConstPointer")
				{
					modifiers.ConstPointer = value;
				}
				else if (key == "Pointer")
				{
					modifiers.Pointer = value;
				}
				else if (key == "Reference")
				{
					modifiers.Reference = value;
				}
				lua_pop(L, 1);
			}
		}

		if (lua_checkstack(L, 3) && TypeArgumentListLuaMetaData::IsType(L, 3))
		{
			TypeArgumentListPtr list = 
				TypeArgumentListLuaMetaData::GetInstance(L, 3);
			type.reset(new Type(node, modifiers, list));
		}
		else
		{
			type.reset(new Type(node, modifiers));
		}
		
		TypeLuaMetaData::PutInstanceOnStack(L, type);
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		TypePtr & ptr = getInstance(L);
		std::stringstream ss;
		ss << "(";
		ss << " [TODO-list arguments here])";			
		lua_pushstring(L, ss.str().c_str());
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"New", LUAGLUE_HELPERCLASS::__new},

#include "../LuaGlue2.hpp"

#endif