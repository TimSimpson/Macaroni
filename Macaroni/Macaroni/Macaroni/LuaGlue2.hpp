/** This file is suppossed to be included after LuaGlue.h, and in addition to
 *  the arguments required by LuaGlue.h requires these macro definitions:
 * ARGUMENTS:
 *	LUAGLUE_ADDITIONALMETATABLEMETHODS - Put additional methods to be registered
 *                                       here.
 *  LUAGLUE_ADDITIONALTABLEMETHODS - Put additional global methods here.
 */

};  // The end of LUAGLUE_CLASSNAMELuaFunctions.

#ifdef LUAGLUE_CREATEMETATABLE
	static const struct luaL_Reg metaTableMethods[]=
	{
		{"__eq", LUAGLUE_CLASSNAMELuaFunctions::__eq},
		{"__gc", LUAGLUE_CLASSNAMELuaFunctions::luaGc},
		{"__index", LUAGLUE_CLASSNAMELuaFunctions::__index},
		LUAGLUE_ADDITIONALMETATABLEMETHODS
		{nullptr, nullptr}
	};
#endif

static const struct luaL_Reg tableMethods[]=
{
	LUAGLUE_ADDITIONALTABLEMETHODS
	{nullptr, nullptr}
};


#ifdef LUAGLUE_CREATEMETATABLE
	LUAGLUE_CLASSREFNAME & LUAGLUE_REGISTRATIONCLASSNAME::GetInstance(lua_State * L, int index)
	{
		return getInstance(L, index);	
	}

	int LUAGLUE_REGISTRATIONCLASSNAME::Index(lua_State * L, 
											 const LUAGLUE_CLASSREFNAME & ptr,
											 const std::string & index)
	{
		return LUAGLUE_CLASSNAMELuaFunctions::__index(L, ptr, index);
	}

	bool LUAGLUE_REGISTRATIONCLASSNAME::IsType(lua_State * L, int index)
	{
		// Copied this from the luaL_checkudata function
		void * p = lua_touserdata(L, index);
		bool returnValue = false;
		if (p != nullptr) // is user data
		{
			// Compares metatable from user data to one in registry.
			if (lua_getmetatable(L, index))
			{
				lua_getfield(L, LUA_REGISTRYINDEX, METATABLENAME);
				if (lua_rawequal(L, -1, -2))
				{
					returnValue = true;
				}
				lua_pop(L, 2); // remove meta tables
			}
		}
		return returnValue;
	}

	void LUAGLUE_REGISTRATIONCLASSNAME::PutInstanceOnStack(
		lua_State * L, 
		const LUAGLUE_CLASSREFNAME & ptr)
	{
		putLUAGLUE_CLASSREFNAMEInstanceOnStack(L, ptr);
	}
#endif

int LUAGLUE_REGISTRATIONCLASSNAME::OpenInLua(lua_State * L)
{	
	LUAGLUE_OPENOTHERMODULES;
	#ifdef LUAGLUE_CREATEMETATABLE
		luaL_getmetatable(L, METATABLENAME);
		if (lua_isnil(L, -1) != 1)
		{
			return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
		}
		luaL_newmetatable(L, METATABLENAME); // create metaTable
		luaL_register(L, nullptr, metaTableMethods);
	#endif

	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	return 1;
}



LUAGLUE_ENDNAMESPACE
