#ifndef MACARONI_GENERATOR_LUA_DYNAMICINSTALLER_H
#define MACARONI_GENERATOR_LUA_DYNAMICINSTALLER_H

#include "../../ME.h"
#include "../../Model/Context.h"
#include "DynamicScriptRunner.h"
#include "../../Model/Library.h"
#include "../../Environment/LuaEnvironment.h"
#include <Macaroni/Build/Manifest.h>
#include "../../Model/MemberVisitor.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include "../../Environment/StringPair.h"
#include "../../IO/Path.h"

BEGIN_NAMESPACE(Macaroni, Generator, Lua)

//TO-DO: Consider removing this ugly piece of crap

/** 
 * Represents a Lua file which gets run in its own environment.
 * Lua files must define the following function:
 *     function Install(library, sourcePaths, localOutput, installPath)
 * paths.Sources
 * paths.Output
 * paths.Install 
 * 
 */
class DynamicInstaller : public DynamicScriptRunner
{
public:
	DynamicInstaller(Model::LibraryPtr context, 					 
					 const boost::filesystem::path & luaFile,
					 Macaroni::Build::ManifestPtr manifest,
					 Macaroni::IO::Path manifestDir,
					 Macaroni::IO::Path installDir,
					 const std::vector<Macaroni::Environment::StringPair> & arguments);
	
	~DynamicInstaller();
	
	bool Run(const std::string & methodName);

private:
	std::vector<Macaroni::Environment::StringPair> arguments;

	Macaroni::IO::Path installDir;

	Model::LibraryPtr library;

	Macaroni::Build::ManifestPtr manifest;

	Macaroni::IO::Path manifestDir;

};

END_NAMESPACE

#endif

