/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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

