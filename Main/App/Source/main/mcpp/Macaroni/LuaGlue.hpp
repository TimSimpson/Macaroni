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
/**  This file is intended to be included exclusively from CPP files.
 *   It must have the following defines added before being included.
 *   First, include all needed files...
 * ARGUMENTS:
 *   LUAGLUE_STARTNAMESPACE - this line begins the namespace surrounding the glue.
 *   LUAGLUE_ENDNAMESPACE - This is the counterpart to start namespace.
 *   LUAGLUE_CLASSNAME - The name of the class, such as "Parser".
 *   LUAGLUE_CLASSREFNAME - The name of the smart pointer, such as "ParserPtr".
 *   LUAGLUE_CLASSFULLLUANAME - Something like "Macaroni.Parser.Parser".
 *	                            Name of a class with dots. MUST BE SURROUNDED
 *                              WITH DOUBLE QUOTES.
 *   LUAGLUE_CLASSFULLCPPNAME - Something like "Macaroni::Parser::Parser".
 *   LUAGLUE_REGISTRATIONCLASSNAME - The class name for the struct used to
 *                                   register this LuaGlue which is in a .h
 *                                   file.
 *   LUAGLUE_OPENOTHERMODULES - Put calls here to open other Modules in Lua
 *                              which will be required by this module.
 *   LUAGLUE_CREATEMETATABLE - If set, a metatable and methods such as __index
 *                             are created.  Essentially, this creates instance
 *                             type info. If that is not desired, do not set
 *                             this.
 */

#include <Macaroni/Lua.h>
#include <Macaroni/LuaCompat.h>
#include <Macaroni/Exception.h>

LUAGLUE_STARTNAMESPACE

#define METATABLENAME LUAGLUE_CLASSFULLLUANAME
#define GLOBALTABLENAME LUAGLUE_CLASSFULLLUANAME
#define LUA_GLUE_TRY try {
#define LUA_GLUE_CATCH } catch(const Macaroni::Exception & ex){ \
		return luaL_error(L, ex.message().c_str()); \
	} catch(const std::exception & ex){ \
		return luaL_error(L, ex.what()); \
	}


#ifdef LUAGLUE_CREATEMETATABLE
	namespace {

		static inline void createLUAGLUE_CLASSREFNAMEUserData(lua_State * L, const LUAGLUE_CLASSREFNAME & source)
		{
			void * memory = lua_newuserdata(L, sizeof(LUAGLUE_CLASSREFNAME));
			LUAGLUE_CLASSREFNAME * instance = new (memory) LUAGLUE_CLASSREFNAME();
			(*instance).operator=(source);
		}

		static inline LUAGLUE_CLASSREFNAME & getInstance(lua_State * L, int index, const char * metaTableName)
		{
			LUAGLUE_CLASSREFNAME * ptrToPtr =
				(LUAGLUE_CLASSREFNAME *) luaL_checkudata(L, index, metaTableName);
			LUAGLUE_CLASSREFNAME & ptr =
				dynamic_cast<LUAGLUE_CLASSREFNAME &>(*ptrToPtr);
			return ptr;
		}

		static inline LUAGLUE_CLASSREFNAME & getInstance(lua_State * L, int index)
		{
			return getInstance(L, index, METATABLENAME);
		}


		static inline LUAGLUE_CLASSREFNAME & getInstance(lua_State * L)
		{
			return getInstance(L, 1);
		}

		static inline void putLUAGLUE_CLASSREFNAMEInstanceOnStack(
				lua_State * L,
				const LUAGLUE_CLASSREFNAME & source
			)
		{
			if (!source)
			{
				lua_pushnil(L);
			}
			else
			{
				createLUAGLUE_CLASSREFNAMEUserData(L, source);
				luaL_getmetatable(L, METATABLENAME);
				lua_setmetatable(L, -2);
			}
		}

	} // End anonymous namespace

#endif

struct LUAGLUE_HELPERCLASS
{
	// Some of these glue functions never change; others will be added by the
	// user...

	#ifdef LUAGLUE_CREATEMETATABLE
		static int luaGc(lua_State * L)
		{
			LUAGLUE_CLASSREFNAME * ptr =
				(LUAGLUE_CLASSREFNAME *) luaL_checkudata(L, 1, METATABLENAME);
			ptr->~LUAGLUE_CLASSREFNAME();
			return 0;
		}

		static int __eq(lua_State * L)
		{
			LUAGLUE_CLASSREFNAME & a = getInstance(L, 1);
			LUAGLUE_CLASSREFNAME & b = getInstance(L, 2);
			lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
			return 1;
		}

		static int __index(lua_State * L)
		{
			LUAGLUE_CLASSREFNAME & ptr = getInstance(L);
			std::string index(luaL_checkstring(L, 2));
			return __index(L, ptr, index);
		}
	#endif

	// From this point, define multiple functions, then include LuaGlue2.cpp.
