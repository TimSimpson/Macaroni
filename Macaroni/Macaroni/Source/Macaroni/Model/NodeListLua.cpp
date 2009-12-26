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
#define LUAGLUE_OPENOTHERMODULES		/*Macaroni::Model::NodeLuaMetaData::OpenInLua(L); */
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

	static int __new(lua_State * L)
	{
		// First arg is an array.
		luaL_checktype(L, 1, LUA_TTABLE);

		NodeListPtr nodeList(new NodeList());

		bool stop = false;
		int index = 0;		
		while(!stop)
		{
			index ++;			
			lua_pushnumber(L, index);
			lua_gettable(L, 1);
			if (!lua_isnil(L, -1))
			{
				if (NodeLuaMetaData::IsType(L, -1))
				{
					NodePtr & node = NodeLuaMetaData::GetInstance(L, -1);
					nodeList->push_back(node);
				}
				else
				{
					std::stringstream ss;
					// Reusing variables is evil, but SO MUCH FUN!  YEAA!!
					ss << "When constructing new NodeList, array argument "
					   << "had unexpected type at index " << index 
					   << ". All types in array are expected to be Nodes.";
					lua_pushstring(L, ss.str().c_str());
					lua_error(L);
				}
			}
			else
			{
				stop = true;
			}
		}

		NodeListLuaMetaData::PutInstanceOnStack(L, nodeList);
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
		{"New", LUAGLUE_HELPERCLASS::__new},

#include "../LuaGlue2.hpp"

#endif