#ifndef MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_CPP
#define MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_CPP

#include "DynamicGenerator.h"
#include "../../Model/ContextLua.h"
#include "../../Exception.h"
#include <boost/foreach.hpp>
#include <Macaroni/Build/_.h>
#include <Macaroni/Build/BuildContext.h>
#include "../../Model/LibraryLua.h"
#include "../../IO/Path.h"
#include "../../IO/PathLua.h"
#include <iostream>
#include <sstream>

using Macaroni::Model::ContextLuaMetaData;
using Macaroni::Build::BuildContext;
using Macaroni::Build::BuildContextPtr;
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
	const boost::filesystem::path & luaFile,
	BuildContextPtr buildContext,
	///*Model::LibraryPtr				library, 
	//const std::vector<boost::filesystem
	//*/const boost::filesystem::path & rootPath,	
	const std::vector<StringPair> & arguments
)
:	arguments(arguments),
	buildContext(buildContext),
	DynamicScriptRunner(luaFile)	
    //library(library),
	//rootPath(rootPath)
{
	if (buildContext->GetProperties().size() > 0) 
	{
		std::stringstream code;	
		code << "properties = " << buildContext->GetProperties() << ";" << std::endl;	
		env.ParseString(buildContext->GetProperties().c_str(), code.str().c_str());
		env.Run();
	}
	env.ParseFile(luaFile.string());
}	 

DynamicGenerator::~DynamicGenerator()
{
}
	
std::string DynamicGenerator::Run(const std::string & methodName)
{
	env.Run();

	lua_State * L = env.GetState();
	
	lua_pushliteral(L, "debug");
	lua_gettable(L, LUA_GLOBALSINDEX);
	lua_pushliteral(L, "traceback");
    lua_gettable(L, -2);

	// call Generate(context, rootPath, output);
	lua_getglobal(L, methodName.c_str());//"Generate");

	// Argument 1 - Library
	LibraryLuaMetaData::PutInstanceOnStack(L, buildContext->GetLibrary());
	if (methodName != "Generate")
	{
		// Argument 2 - sources array
		lua_newtable(L);
		int count = 1;
		BOOST_FOREACH(PathPtr source, buildContext->GetSourceDirs())
		{
			lua_pushnumber(L, count);
			PathLuaMetaData::PutInstanceOnStack(L, source);
			lua_settable(L, -3);			
		}		
	}
	// Argument 3 - OutputPath (2 for "Generate")
	//PathPtr path(new Path(rootPath, rootPath));
	PathLuaMetaData::PutInstanceOnStack(L, buildContext->GetOutputDir());//path);

	// Argument 4 - InstallPath 
	if (methodName != "Generate")
	{
		PathLuaMetaData::PutInstanceOnStack(L, buildContext->GetInstallDir());
	}
	// Argument 5 - Extra args (3 for "Generate")
	bool isType = PathLuaMetaData::IsType(L, -1);
	LuaEnvironment::CreateNewTableFromStringPairs(L, arguments);

	// TO-DO: pass in next two args
	
	//http://lua-users.org/lists/lua-l/2005-04/msg00070.html
	const int n = methodName != "Generate" ? 5 : 3;
	// Cause Lua to give us a stack trace. */
	
    //lua_insert(L, -n - 2);*/
	// stack layout:  _TRACEBACK func arg1 ... argn 

	
	//lua_call(L, n, 0);
	//goto SkipIt;
	// Arg values: L, 1 arg, 0 return values, ?
	const int resultCount = 1;
	int eCode = lua_pcall(L, n, resultCount, -n - 2);	
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

	if (lua_isnil(L, -1))
	{
		return "";
	}
	else if (lua_istable(L, -1))
	{
		std::stringstream ss;
		env.SerializeField(ss);
		return ss.str();
	}
	else
	{
		std::stringstream msg;
		msg << "Return value of generators must be either a table or nil (" 
			<< luaFilePath.string() << ", method "
			<< methodName 
			<< ").";
		throw Macaroni::Exception(msg.str().c_str());
	}
}


END_NAMESPACE

#endif

