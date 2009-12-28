#ifndef MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_CPP
#define MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_CPP

#include "DynamicGenerator.h"
#include "../../Model/ContextLua.h"
#include "../../Exception.h"
#include "../../IO/Path.h"
#include "../../IO/PathLua.h"
#include <iostream>
#include <sstream>

using Macaroni::Model::ContextLuaMetaData;
using Macaroni::IO::Path;
using Macaroni::IO::PathLuaMetaData;
using Macaroni::IO::PathPtr;

BEGIN_NAMESPACE(Macaroni, Generator, Lua)

DynamicGenerator::DynamicGenerator
(	
	Model::ContextPtr				context, 
	const boost::filesystem::path & rootPath,
	const boost::filesystem::path & luaFile	
)
:	context(context),
	env(),
	rootPath(rootPath)
{
	//env.ParseString("FileWriter", LUACODE);
	env.ParseFile(luaFile.string());
}	 

DynamicGenerator::~DynamicGenerator()
{
}
	
bool DynamicGenerator::Run()
{
	env.Run();

	lua_State * L = env.GetState();
	
	// call Generate(context, rootPath, output);
	lua_getglobal(L, "Generate");
	ContextLuaMetaData::PutInstanceOnStack(L, context);
	
	PathPtr path(new Path(rootPath, rootPath));
	PathLuaMetaData::PutInstanceOnStack(L, path);

	bool isType = PathLuaMetaData::IsType(L, -1);

	// TO-DO: pass in next two args
	
	// Arg values: L, 1 arg, 0 return values, ?
	int eCode = lua_pcall(L, 2, 0, 0);
	if (eCode != 0)
	{	
		std::stringstream ss;
		ss << "Error running Lua:";
		ss << luaL_checkstring(L, -1);
		//std::cerr << ss.str() << std::endl;
		throw Macaroni::Exception(ss.str().c_str());
	}
	return true;
}


END_NAMESPACE

#endif

