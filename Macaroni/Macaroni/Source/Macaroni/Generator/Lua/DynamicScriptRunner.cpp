#ifndef MACARONI_GENERATOR_DYNAMICSCRIPTRUNNER_CPP
#define MACARONI_GENERATOR_DYNAMICSCRIPTRUNNER_CPP

#include "DynamicScriptRunner.h"
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

DynamicScriptRunner::DynamicScriptRunner
(	
	const boost::filesystem::path & luaFile	
)
:	luaFilePath(luaFile),
	env()
{
	//env.ParseString("FileWriter", LUACODE);
	env.SetPackageDirectory(luaFile.branch_path().string());	
}	 

DynamicScriptRunner::~DynamicScriptRunner()
{
}


END_NAMESPACE

#endif

