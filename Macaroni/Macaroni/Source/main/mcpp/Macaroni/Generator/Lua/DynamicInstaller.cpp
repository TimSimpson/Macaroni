#ifndef MACARONI_GENERATOR_LUA_DYNAMICINSTALLER_CPP
#define MACARONI_GENERATOR_LUA_DYNAMICINSTALLER_CPP

#include "DynamicInstaller.h"
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

DynamicInstaller::DynamicInstaller
(	
	Model::LibraryPtr				library, 
	const boost::filesystem::path & luaFile,
	Macaroni::Build::ManifestPtr manifest,
	Macaroni::IO::Path manifestDir,
	Macaroni::IO::Path installDir,
	const std::vector<StringPair> & arguments
)
:	arguments(arguments),
	DynamicScriptRunner(luaFile),
	installDir(installDir),
    library(library),
	manifest(manifest),
	manifestDir(manifestDir)	
{	
}	 

DynamicInstaller::~DynamicInstaller()
{
}
	
std::string DynamicInstaller::Run(const std::string & methodName)
{
	return "nil";
}


END_NAMESPACE

#endif

