#ifndef MACARONI_DOCUMENTLUA_CPP
#define MACARONI_DOCUMENTLUA_CPP

extern "C" {
	#include "../../Lua/lua.h"
	#include "../../Lua/lauxlib.h"
	#include "../../Lua/lualib.h"
}
#include "Document.h"
#include "NamespaceLua.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

#define GLOBALTABLENAME "Macaroni.Model.Document"
#define METATABLENAME "Macaroni.Model.Document"
#define PNS_METATABLENAME "Macaroni.Model.Document.Properties.Namespaces"


namespace {
	/** Creates a new DocumentPtr whose memory is managed by Lua (in other words,
	 * it lives in Lua) and puts it at the top of the Lua stack. */
	static inline void createDocumentPtrUserData(lua_State * L, const DocumentPtr & source)
	{
		void * memory = lua_newuserdata(L, sizeof(DocumentPtr));
		DocumentPtr * ptr = new (memory) DocumentPtr();
		(*ptr).operator=(source);
	}

	/** In all Object-oriented style access of the object from within Lua, the
	 * first argument will be the object itself. */
	static inline DocumentPtr & getInstance(lua_State * L)
	{
		DocumentPtr * ptrToPtr = (DocumentPtr *) luaL_checkudata(L, 1, METATABLENAME);
		DocumentPtr & ptr = dynamic_cast<DocumentPtr &>(*ptrToPtr);
		return ptr;
	}

	static inline void putDocumentPtrInstanceOnStack(lua_State * L, const DocumentPtr & source)
	{
		createDocumentPtrUserData(L, source);
		luaL_getmetatable(L, METATABLENAME);
		lua_setmetatable(L, -2); 
	}
}; // end anon namespace

struct DocumentRegister
{
	static int Ctor(lua_State * L)
	{
		const char * name = luaL_checkstring(L, 1);
		DocumentPtr newInstance(new Document(std::string(name)));

		putDocumentPtrInstanceOnStack(L, newInstance);
		return 1;
	}

	static int luaGc(lua_State * L)
	{
		DocumentPtr & instance = getInstance(L);
		instance.~DocumentPtr();
		return 0;
	}

	static int __index(lua_State * L)
	{
		DocumentPtr & f = getInstance(L);	
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
			createDocumentPtrUserData(L, f);
			// with meta table that knows how to access Namespaces property...
			luaL_getmetatable(L, PNS_METATABLENAME);
			lua_setmetatable(L, -2); 
			return 1;
		}
		else if (index == "Read")
		{
			lua_pushcfunction(L, Read);
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
		DocumentPtr & instance = getInstance(L);
		std::stringstream ss;
		ss << "Document\"" << instance->GetName() << "\"";
		lua_pushlstring(L, ss.str().c_str(), ss.str().length());
		return 1;
	}

	static int GetName(lua_State * L)
	{
		DocumentPtr & f = getInstance(L);
		lua_pushlstring(L, f->GetName().c_str(), f->GetName().length());
		return 1;
	}

	static int Namespaces(lua_State * L)
	{
		const char * name = luaL_checkstring(L, 1);
		return 0;
	}

	static inline DocumentPtr & PNS_getInstance(lua_State * L)
	{
		DocumentPtr * ptrToPtr = (DocumentPtr *) luaL_checkudata(L, 1, PNS_METATABLENAME);
		DocumentPtr & ptr = dynamic_cast<DocumentPtr &>(*ptrToPtr);
		return ptr;
	}

	static int Read(lua_State * L)
	{
		DocumentPtr & instance = getInstance(L);
		const char * name = luaL_checkstring(L, 2);
		
		instance->Read(std::string(name));
		return 1;
	}

	static int PNS__gc(lua_State * L)
	{
		DocumentPtr & instance = PNS_getInstance(L);
		instance.~DocumentPtr();
		return 0;
	}

	static int PNS__index(lua_State * L)
	{
		DocumentPtr & f = PNS_getInstance(L);	
		int num = luaL_checkinteger(L, 2);
		if (num > 0 && num <= f->namespaces.size())
		{
			NamespaceLuaMetaData::PutInstanceOnStack(L, f->namespaces[num - 1]);
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	static int PNS__len(lua_State * L)
	{
		DocumentPtr & f = PNS_getInstance(L);	
		lua_pushinteger(L, f->namespaces.size());
		return 1;
	}

	static int PNS__tostring(lua_State * L)
	{
		DocumentPtr & instance = PNS_getInstance(L);
		std::stringstream ss;
		ss << "Document\"" << instance->GetName() << "\" Namespace Count =";
		ss << instance->namespaces.size();
		lua_pushlstring(L, ss.str().c_str(), ss.str().length());
		return 1;
	}
};

static const struct luaL_Reg tableMethods[]=
{
	{"New", DocumentRegister::Ctor},
	{nullptr, nullptr}
};

static const struct luaL_Reg metaTableMethods[]=
{
	{"__gc", DocumentRegister::luaGc},
	{"__index", DocumentRegister::__index},
	{"__tostring", DocumentRegister::__tostring},
	//{"GetName", DocumentRegister::GetName},
	{nullptr, nullptr}
};

static const struct luaL_Reg PNS_MetaTable[]=
{
	{"__gc", DocumentRegister::PNS__gc},
	{"__index", DocumentRegister::PNS__index},
	{"__len", DocumentRegister::PNS__len},
	{"__tostring", DocumentRegister::PNS__tostring},
	//{"GetName", DocumentRegister::GetName},
	{nullptr, nullptr}
};

//static const struct luaL_Reg __indexTableMethods[]=
//{
//	{"__index", DocumentRegister::__index},
//	{"GetName", DocumentRegister::GetName},
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
int Document::OpenInLua(lua_State * L)
{
	// Necessary because we may pass back namespaces on the Lua stack...
	NamespaceLuaMetaData::OpenInLua(L);

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
	
	
	// Creates or reuses a table called "Macaroni_Document" and puts it in global 
	// scope.
	luaL_register(L, GLOBALTABLENAME, tableMethods);
	return 1;
}

END_NAMESPACE2

#endif