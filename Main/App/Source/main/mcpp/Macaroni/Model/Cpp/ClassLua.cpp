#ifndef MACARONI_MODEL_CPP_CLASSLUA_CPP
#define MACARONI_MODEL_CPP_CLASSLUA_CPP

#include "ClassLua.h"
#include <Macaroni/Model/LibraryLua.h>
#include "../NodeLua.h"
#include "../MemberLua.h"
#include "../NodeListLua.h"
#include <Macaroni/Model/ReasonLua.h>
#include "TypeInfoLua.h"
#include "VariableLua.h"

//extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
//}


struct lua_State;

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE(Macaroni, Model, Cpp)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE
#define LUAGLUE_CLASSNAME Class
#define LUAGLUE_CLASSREFNAME ClassPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.Cpp.Class"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::Cpp::Class
#define LUAGLUE_REGISTRATIONCLASSNAME ClassLuaMetaData
#define LUAGLUE_HELPERCLASS ClassLuaFunctions
#define LUAGLUE_OPENOTHERMODULES // Macaroni::Model::NodeLuaMetaData::OpenInLua(L);
#define LUAGLUE_CREATEMETATABLE 

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int FunctionArgumentsOpenInLua(lua_State * L);

int FunctionReturnTypeInfoOpenInLua(lua_State * L);

END_NAMESPACE

#include "../../LuaGlue.hpp"

	static int Create(lua_State * L)
	{
		using Macaroni::Model::Library;
		using Macaroni::Model::LibraryPtr;
		using Macaroni::Model::LibraryLuaMetaData;
		using Macaroni::Model::Member;
		using Macaroni::Model::MemberLuaMetaData;
		using Macaroni::Model::Node;
		using Macaroni::Model::NodeLuaMetaData;
		using Macaroni::Model::NodePtr;
		using Macaroni::Model::NodeList;
		using Macaroni::Model::NodeListLuaMetaData;
		using Macaroni::Model::NodeListPtr;
		using Macaroni::Model::Reason;
		using Macaroni::Model::ReasonLuaMetaData;
		using Macaroni::Model::ReasonPtr;

		if (!LibraryLuaMetaData::IsType(L, 1)) 
		{			
			luaL_error(L, "Expected Library for argument 1.");
		}
		LibraryPtr library = LibraryLuaMetaData::GetInstance(L, 1);

		if (!NodeLuaMetaData::IsType(L, 2))
		{
			luaL_error(L, "Expected Node for argument 2.");
		}
		NodePtr node = NodeLuaMetaData::GetInstance(L, 2);

		if (!NodeListLuaMetaData::IsType(L, 3))
		{
			luaL_error(L, "Expected NodeList for argument 3.");
		}
		NodeListPtr imports = NodeListLuaMetaData::GetInstance(L, 3);

		if (!ReasonLuaMetaData::IsType(L, 4))
		{			
			luaL_error(L, "Expected Reason for argument 4.");
		}
		ReasonPtr reason = ReasonLuaMetaData::GetInstance(L, 4);
		
		ClassPtr newInstance = Class::Create(library, node, imports, reason); 
		MemberPtr memberPtr = boost::dynamic_pointer_cast<Member>(newInstance);
		MemberLuaMetaData::PutInstanceOnStack(L, memberPtr);
		return 1;
	}

	static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{ 
		if (index == "FriendNodes")
		{
			NodeListPtr list = ptr->GetFriendNodes();
			NodeListLuaMetaData::PutInstanceOnStack(L, list);
		}
		else if (index == "GlobalNodes")
		{
			NodeListPtr list = ptr->GetGlobalNodes();
			NodeListLuaMetaData::PutInstanceOnStack(L, list);
		}
		else if (index == "ImportedNodes")
		{
			NodeListPtr list = ptr->GetImportedNodes();
			NodeListLuaMetaData::PutInstanceOnStack(L, list);
		}
		else 
		{
			lua_pushnil(L);
		}

		/*if (index == "Arguments")
		{
			createLUAGLUE_CLASSREFNAMEUserData(L, ptr);
			luaL_getmetatable(L, PROPERTIES_METATABLENAME_ARGUMENTS);
			lua_setmetatable(L, -2); 
			return 1;
		}
		else if (index == "ReturnType")
		{
			createLUAGLUE_CLASSREFNAMEUserData(L, ptr);
			luaL_getmetatable(L, PROPERTIES_METATABLENAME_RETURNTYPE);
			lua_setmetatable(L, -2); 
			return 1;
		}*/		
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		lua_pushstring(L, "Class");
		return 1;
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		/*{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, */

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"Create", LUAGLUE_HELPERCLASS::Create},		

#include "../../LuaGlue2.hpp"

#endif
