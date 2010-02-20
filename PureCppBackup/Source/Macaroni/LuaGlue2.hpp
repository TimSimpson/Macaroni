/** This file is suppossed to be included after LuaGlue.h, and in addition to
 *  the arguments required by LuaGlue.h requires these macro definitions:
 * ARGUMENTS:
 *	LUAGLUE_ADDITIONALMETATABLEMETHODS - Put additional methods to be registered
 *                                       here.
 *  LUAGLUE_ADDITIONALTABLEMETHODS - Put additional global methods here.
 */

};  // The end of HELPERCLASS.

#ifdef LUAGLUE_CREATEMETATABLE
	static const struct luaL_Reg metaTableMethods[]=
	{
		{"__eq", LUAGLUE_HELPERCLASS::__eq},
		{"__gc", LUAGLUE_HELPERCLASS::luaGc},
		{"__index", LUAGLUE_HELPERCLASS::__index},
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
		return LUAGLUE_HELPERCLASS::__index(L, ptr, index);
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
	std::string metaTableName(METATABLENAME);
	std::string fullLuaName(LUAGLUE_CLASSFULLLUANAME);
	if (metaTableName == "Macaroni.Model.Cpp.Typedef" 
		||
		fullLuaName == "Macaroni.Model.Cpp.Typedef")
	{
		int five = 5;
	}

	luaL_getmetatable(L, metaTableName.c_str());
	if (lua_isnil(L, -1) != 1)
	{
		Macaroni::Environment::DebugLog::Write(LUAGLUE_CLASSFULLLUANAME, __LINE__, "Skipping!!");
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}				

	luaL_newmetatable(L, metaTableName.c_str()); // create metaTable
	Macaroni::Environment::DebugLog::Write(metaTableName.c_str(), __LINE__, " <== Metatable.");

	#ifdef LUAGLUE_CREATEMETATABLE
		luaL_register(L, nullptr, metaTableMethods);
	#endif

	Macaroni::Environment::DebugLog::Write(LUAGLUE_CLASSFULLLUANAME, __LINE__, "About to call register...");
	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	Macaroni::Environment::DebugLog::Write(LUAGLUE_CLASSFULLLUANAME, __LINE__, "...registration Completed.");

	Macaroni::Environment::DebugLog::Write(LUAGLUE_CLASSFULLLUANAME, __LINE__, "Opening other modules...");
	LUAGLUE_OPENOTHERMODULES;
	Macaroni::Environment::DebugLog::Write(LUAGLUE_CLASSFULLLUANAME, __LINE__, "...finished opening other modules.");
	return 1;
}



LUAGLUE_ENDNAMESPACE
