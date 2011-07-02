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

