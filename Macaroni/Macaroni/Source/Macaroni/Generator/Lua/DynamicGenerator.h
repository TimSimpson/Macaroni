#ifndef MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_H
#define MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_H

#include "../../ME.h"
#include "../../Model/Context.h"
#include "../../Environment/LuaEnvironment.h"
#include "../../Model/MemberVisitor.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>

BEGIN_NAMESPACE(Macaroni, Generator, Lua)

/** 
 * Represents a Lua file which gets run in its own environment.
 * Lua files must define the following function:
 *     function Generate(context, rootPath, log)
 * Context and rootPath are passed in as expected, while log is a simple
 * output apparatus that for the time being simply echoes everything to the
 * screen.
 * 
 */
class DynamicGenerator
{
public:
	DynamicGenerator(Model::ContextPtr context, 
					 const boost::filesystem::path & rootPath,
					 const boost::filesystem::path & luaFile);
	
	~DynamicGenerator();
	
	bool Run();

private:
	Model::ContextPtr context;

	Macaroni::Environment::LuaEnvironment env;

	boost::filesystem::path rootPath;

};

END_NAMESPACE

#endif

