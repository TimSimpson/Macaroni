#ifndef MACARONI_GENERATOR_LUA_DYNAMICSCRIPTRUNNER_H
#define MACARONI_GENERATOR_LUA_DYNAMICSCRIPTRUNNER_H

#include "../../ME.h"
#include "../../Model/Library.h"
#include "../../Environment/LuaEnvironment.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include "../../Environment/StringPair.h"

BEGIN_NAMESPACE(Macaroni, Generator, Lua)

/** 
 * Represents a Lua file which gets run in its own environment.
 * 
 */
class DynamicScriptRunner
{
public:
	DynamicScriptRunner(const boost::filesystem::path & luaFile);
	
	~DynamicScriptRunner();
	
	virtual std::string Run(const std::string & methodName) = 0;

protected:	
	Macaroni::Environment::LuaEnvironment env;

	boost::filesystem::path luaFilePath;

};

END_NAMESPACE

#endif

