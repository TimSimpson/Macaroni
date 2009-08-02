#ifndef MACARONI_FILEREGISTER_CPP
#define MACARONI_FILEREGISTER_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "File.h"
#include "NamespaceLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define GLOBALTABLENAME "Macaroni.Model.File"
#define METATABLENAME "Macaroni.Model.File"
#define PNS_METATABLENAME "Macaroni.Model.File.Properties.Namespaces"


namespace {
	/** Creates a new FilePtr whose memory is managed by Lua (in other words,
	 * it lives in Lua) and puts it at the top of the Lua stack. */
	static inline void createFilePtrUserData(lua_State * L, const FilePtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(FilePtr));
		FilePtr * ptr = new (memory) FilePtr();
		(*ptr).operator=(source);
	}

	/** In all Object-oriented style access of the object from within Lua, the
	 * first argument will be the object itself. */
	static inline FilePtr & getInstance(lua_State * L)
	{
		FilePtr * ptrToPtr = (FilePtr *) luaL_checkudata(L, 1, METATABLENAME);
		FilePtr & ptr = dynamic_cast<FilePtr &>(*ptrToPtr);
		return ptr;
	}

	static inline void putFilePtrInstanceOnStack(lua_State * L, const FilePtr & source)
	{
		createFilePtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}
}; // end anon namespace

struct FileRegister
{
	static int Ctor(lua_State * L)
	{
		const char * name = luaL_checkstring(L, 1);
		FilePtr newInstance(new File(std::string(name)));

		putFilePtrInstanceOnStack(L, newInstance);
		return 1;
	}

	static int luaGc(lua_State * L)
	{
		FilePtr & instance = getInstance(L);
		instance.~FilePtr();
		return 0;
	}

	static int __index(lua_State * L)
	{
		FilePtr & f = getInstance(L);	
		std::string index(luaL_checkstring(L, 2));
		
		// Really, another table is needed for the __index metamethod
		// and this table could itself have an __index metamethod which could
		// call a function to return property values.
		if (index == "GetName")
		{
			lua_pushcfunction(L, GetName);
		}
		else if (index == "Name")
		{
			lua_pushlstring(L, f->GetName().c_str(), f->GetName().length());
		}
		else if (index == "Namespaces")
		{
			// Put new PTR for this file into Lua...
			createFilePtrUserData(L, f);
			// with meta table that knows how to access Namespaces property...
			luaL_getmetatable(L, PNS_METATABLENAME);
			lua_setmetatable(L, -2); 
			return 1;
		}
		else
		{
			lua_pushnil(L);
		}
		//lua_pushcfunction(L, GetName);
		//lua_pushlstring(L, f->GetName().c_str(), f->GetName().length());
		return 1;
	}

	static int __tostring(lua_State * L)
	{
		FilePtr & instance = getInstance(L);
		std::stringstream ss;
		ss << "File\"" << instance->GetName() << "\"";
		lua_pushlstring(L, ss.str().c_str(), ss.str().length());
		return 1;
	}

	static int GetName(lua_State * L)
	{
		FilePtr & f = getInstance(L);
		lua_pushlstring(L, f->GetName().c_str(), f->GetName().length());
		return 1;
	}

	static int Namespaces(lua_State * L)
	{
		const char * name = luaL_checkstring(L, 1);
		return 0;
	}

	static inline FilePtr & PNS_getInstance(lua_State * L)
	{
		FilePtr * ptrToPtr = (FilePtr *) luaL_checkudata(L, 1, PNS_METATABLENAME);
		FilePtr & ptr = dynamic_cast<FilePtr &>(*ptrToPtr);
		return ptr;
	}

	static int PNS__gc(lua_State * L)
	{
		FilePtr & instance = PNS_getInstance(L);
		instance.~FilePtr();
		return 0;
	}

	static int PNS__index(lua_State * L)
	{
		FilePtr & f = PNS_getInstance(L);	
		int num = luaL_checkinteger(L, 2);
		NamespaceLuaMetaData::PutInstanceOnStack(L, f->namespaces[num]);
		return 1;
	}

	static int PNS__len(lua_State * L)
	{
		FilePtr & f = PNS_getInstance(L);	
		lua_pushinteger(L, f->namespaces.size());
		return 1;
	}

	static int PNS__tostring(lua_State * L)
	{
		FilePtr & instance = PNS_getInstance(L);
		std::stringstream ss;
		ss << "File\"" << instance->GetName() << "\" Namespace Count =";
		ss << instance->namespaces.size();
		lua_pushlstring(L, ss.str().c_str(), ss.str().length());
		return 1;
	}
};

static const struct luaL_Reg tableMethods[]=
{
	{"New", FileRegister::Ctor},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__gc", FileRegister::luaGc},
	{"__index", FileRegister::__index},
	{"__tostring", FileRegister::__tostring},
	//{"GetName", FileRegister::GetName},
	{nullptr, nullptr}
};

static const struct luaL_Reg PNS_MetaTable[]=
{
	{"__gc", FileRegister::PNS__gc},
	{"__index", FileRegister::PNS__index},
	{"__len", FileRegister::PNS__len},
	{"__tostring", FileRegister::PNS__tostring},
	//{"GetName", FileRegister::GetName},
	{nullptr, nullptr}
};

//static const struct luaL_Reg __indexTableMethods[]=
//{
//	{"__index", FileRegister::__index},
//	{"GetName", FileRegister::GetName},
//	{nullptr, nullptr}
//};

/* 
 * META TABLE 1:
 * The proper way to register an object like user datatum in Lua is to create
 * a unique metatable for it that will exist in the Lua Registry.  This step
 * ensures safe access- in other words, we can check the Lua stack to ensure
 * that we are being given the type of user data we originally created.
 * Thus all user data we create is assigned this meta table.
 * META TABLE 2:
 * If we want object-style access, we must also another metatable, and set the
 * __index key of the first metatable to reference this second meta table.
 */
int File::OpenInLua(lua_State * L)
{
	luaL_newmetatable(L, METATABLENAME); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);

	luaL_newmetatable(L, PNS_METATABLENAME); // create metaTable
	luaL_register(L, nullptr, PNS_MetaTable);

	//lua_newtable(L);	// create __indexTable
	//luaL_register(L, nullptr, __indexTableMethods);
	//lua_setfield(L, -2, "__index"); // metaTable.__index = __indexTable;
	//lua_pushvalue(L, -1); // we duplicate __index table...
	//lua_setfield(L, -2, "__index"); // __indexTable.__index = __indexTable;
	// The second reference to __indexTable is off the stack now.

	/*
	// Duplicate the meta table to create the indexing metatable, assign it
	// to the __index of the first metatable, then register the instance 
	// methods.  Pg 266 of the Lua book.	
	lua_pushvalue(L, -1); // duplicates the metatable
	lua_setfield(L, -2, "__index"); // set stack[-2].__index = stack[-1]
	luaL_register(L, nullptr, metaTableMethods);
	*/
	
	
	// Creates or reuses a table called "Macaroni_File" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	return 1;
}

END_NAMESPACE2

#endif