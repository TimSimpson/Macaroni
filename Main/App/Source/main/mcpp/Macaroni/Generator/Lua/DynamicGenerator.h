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
#ifndef MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_H
#define MACARONI_GENERATOR_LUA_DYNAMICGENERATOR_H

#include "../../ME.h"
#include <Macaroni/Model/Context.h>
#include "DynamicScriptRunner.h"
#include <Macaroni/Build/BuildContextPtr.h>
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
					 const std::string & arguments /* Represents a table. */);
	
	~DynamicGenerator();
	
	std::string Run(const std::string & methodName);

private:
	std::string arguments;

	Macaroni::Build::BuildContextPtr buildContext;

	Model::LibraryPtr library;

	boost::filesystem::path rootPath;

};

END_NAMESPACE

#endif

