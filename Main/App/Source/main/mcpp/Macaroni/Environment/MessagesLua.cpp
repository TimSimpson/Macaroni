/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MACARONI_ENVIRONMENT_MESSAGESLUA_CPP
#define MACARONI_ENVIRONMENT_MESSAGESLUA_CPP

#include "MessagesLua.h"
#include <Macaroni/Environment/Messages.h>
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
