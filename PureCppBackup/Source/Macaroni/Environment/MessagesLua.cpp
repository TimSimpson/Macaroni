#ifndef MACARONI_ENVIRONMENT_MESSAGESLUA_CPP
#define MACARONI_ENVIRONMENT_MESSAGESLUA_CPP

#include "MessagesLua.h"
#include "Messages.h"
#include <sstream>

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Environment)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME Messages
#define LUAGLUE_CLASSREFNAME MessagesPtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Environment.Messages"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Environment::Messages
#define LUAGLUE_REGISTRATIONCLASSNAME MessagesLuaMetaData
#define LUAGLUE_OPENOTHERMODULES /* */

#include "../LuaGlue.hpp"

	/*static int __index(lua_State * L, const LUAGLUE_CLASSREFNAME & ptr, 
									  const std::string & index)
	{
		ParserPtr p = boost::dynamic_pointer_cast<Parser>(ptr);
		return ParserLuaMetaData::Index(L, p, index);		
	}

	static int __tostring(lua_State * L)
	{
		LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
		lua_pushstring(L, "CppParser");
		return 1;
	}*/

	static int Get(lua_State * L)
	{
		const char * key = luaL_checkstring(L, 1);
		if (!lua_isnumber(L, 2))
		{
			lua_pushstring(L, Messages::Get(key).c_str());	
		}
		else 
		{
			int number = lua_tointeger(L, 2);
			lua_pushstring(L, Messages::Get(key, number).c_str());
		}							
		return 1;
	}

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		/*{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, */

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		{"Get", LUAGLUE_HELPERCLASS::Get},	

#include "../LuaGlue2.hpp"

#endif