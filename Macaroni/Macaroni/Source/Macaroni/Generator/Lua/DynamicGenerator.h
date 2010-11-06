#ifndef MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_H
#define MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_H

#include "../../ME.h"
#include "../../Model/Context.h"
#include "DynamicScriptRunner.h"
#include <Macaroni/Build/_.h>
#include <Macaroni/Build/BuildContext.h>
#include "../../Model/Library.h"
#include "../../Environment/LuaEnvironment.h"
#include "../../Model/MemberVisitor.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include "../../Environment/StringPair.h"

BEGIN_NAMESPACE(Macaroni, Generator, Lua)

/** 
 * Represents a Lua file which gets run in its own environment.
 * Lua files must define the following function:
 *     function Generate(library, path)
 * Context and rootPath are passed in as expected, while log is a simple
 * output apparatus that for the time being simply echoes everything to the
 * screen.
 * 
 */
class DynamicGenerator : public DynamicScriptRunner
{
public:
	DynamicGenerator(const boost::filesystem::path & luaFile,
					 Macaroni::Build::BuildContextPtr buildContext,
					//Model::LibraryPtr context, 
					 //const boost::filesystem::path & rootPath,					 
					 const std::vector<Macaroni::Environment::StringPair> & arguments);
	
	~DynamicGenerator();
	
	bool Run(const std::string & methodName);

private:
	std::vector<Macaroni::Environment::StringPair> arguments;

	Macaroni::Build::BuildContextPtr buildContext;

	Model::LibraryPtr library;

	boost::filesystem::path rootPath;

};

END_NAMESPACE

#endif

