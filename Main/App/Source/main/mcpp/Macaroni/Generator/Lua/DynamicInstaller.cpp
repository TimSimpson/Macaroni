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
#ifndef MACARONI_GENERATOR_LUA_DYNAMICINSTALLER_CPP
#define MACARONI_GENERATOR_LUA_DYNAMICINSTALLER_CPP

#include "DynamicInstaller.h"
#include <Macaroni/Model/ContextLua.h>
#include <Macaroni/Exception.h>
#include <Macaroni/Model/LibraryLua.h>
#include <Macaroni/IO/Path.h>
#include <Macaroni/IO/PathLua.h>
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

