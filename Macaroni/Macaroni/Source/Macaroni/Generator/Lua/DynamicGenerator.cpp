#ifndef MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_CPP
#define MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_CPP

#include "DynamicGenerator.h"
#include "../../Model/ContextLua.h"
#include "../../Exception.h"
#include "../../Model/LibraryLua.h"
#include "../../IO/Path.h"
#include "../../IO/PathLua.h"
#include <iostream>
#include <sstream>

using Macaroni::Model::ContextLuaMetaData;
using Macaroni::Environment::LuaEnvironment;
using Macaroni::Model::Library;
using Macaroni::Model::LibraryLuaMetaData;
using Macaroni::Model::LibraryPtr;
using Macaroni::IO::Path;
using Macaroni::IO::PathLuaMetaData;
using Macaroni::IO::PathPtr;
using Macaroni::Environment::StringPair;

BEGIN_NAMESPACE(Macaroni, Generator, Lua)

DynamicGenerator::DynamicGenerator
(	
	Model::LibraryPtr				library, 
	const boost::filesystem::path & rootPath,
	const boost::filesystem::path & luaFile,
	const std::vector<StringPair> & arguments
)
:	arguments(arguments),
	DynamicScriptRunner(luaFile),
    library(library),
	rootPath(rootPath)
{
	
}	 

DynamicGenerator::~DynamicGenerator()
{
}
	
bool DynamicGenerator::Run()
{
	env.Run();

	lua_State * L = env.GetState();
	
	lua_pushliteral(L, "debug");
	lua_gettable(L, LUA_GLOBALSINDEX);
	lua_pushliteral(L, "traceback");
    lua_gettable(L, -2);

	// call Generate(context, rootPath, output);
	lua_getglobal(L, "Generate");
	LibraryLuaMetaData::PutInstanceOnStack(L, library);
	
	PathPtr path(new Path(rootPath, rootPath));
	PathLuaMetaData::PutInstanceOnStack(L, path);
	
	bool isType = PathLuaMetaData::IsType(L, -1);

	LuaEnvironment::CreateNewTableFromStringPairs(L, arguments);

	// TO-DO: pass in next two args
	
	//http://lua-users.org/lists/lua-l/2005-04/msg00070.html
	const int n = 3;
	// Cause Lua to give us a stack trace. */
	
    //lua_insert(L, -n - 2);*/
	// stack layout:  _TRACEBACK func arg1 ... argn 

	
	//lua_call(L, n, 0);
	//goto SkipIt;
	// Arg values: L, 1 arg, 0 return values, ?
	int eCode = lua_pcall(L, n, 0, -n - 2);	
	if (eCode != 0)
	{	
		std::stringstream ss;
		ss << "Error running Generator \"" << luaFilePath.string() << "\":";
		if (lua_isstring(L, -1) == 1) 
		{
			ss << luaL_checkstring(L, -1);	
		} /*
		else if (lua_isfunction(L, -1) == 1)
		{
			ss << "A FUNCTION!?";
			lua_call(L, 0, 0);
			ss << "Called it...?!";
		}*/
		else
		{
			lua_getglobal(L, "tostring");
			lua_pushvalue(L, -2);
			lua_call(L, 1, 1);
			ss << luaL_checkstring(L, -1);
			ss << "?!!?!?x!!!";
		}
		
		//std::cerr << ss.str() << std::endl;
		throw Macaroni::Exception(ss.str().c_str());
	}
SkipIt:
	return true;
}


END_NAMESPACE

#endif

