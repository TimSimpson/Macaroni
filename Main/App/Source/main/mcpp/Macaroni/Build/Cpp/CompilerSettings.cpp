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
#ifndef MACARONI_BUILD_CPP_COMPILERSETTINGS_CPP
#define MACARONI_BUILD_CPP_COMPILERSETTINGS_CPP

#include <Macaroni/Containers.h>
#include "CompilerSettings.h"
#include "../../Environment/LuaEnvironment.h"

using Macaroni::Containers::AssignVectorToConstElementVector;
using Macaroni::Environment::LuaEnvironment;
using Macaroni::Environment::StringPair;

BEGIN_NAMESPACE(Macaroni, Build, Cpp)

CompilerSettings::CompilerSettings()
:additionalCompilerArgs(""),
 additionalLinkerArgs(""),
 compilerExe(""),
 environmentVariables(),
 firstLinkedObjects(),
 includePaths(),
 linkerExe(),
 linkerLibraryPaths(),
 oSwitch(),
 paths(),
 preprocessorDirectives()
{
}

CompilerSettings::CompilerSettings(const boost::filesystem::path & filePath)
:additionalCompilerArgs(""),
 additionalLinkerArgs(""),
 compilerExe(""),
 environmentVariables(),
 firstLinkedObjects(),
 includePaths(),
 linkerExe(),
 linkerLibraryPaths(),
 oSwitch(),
 paths(),
 preprocessorDirectives()
{
	LuaEnvironment env;
	env.SetPackageDirectory(filePath.branch_path().string());
	env.ParseFile(filePath.string());
	env.Run();
	lua_State * L = env.GetState();

	env.GetFromGlobalVarOrDefault(additionalCompilerArgs, "additionalCompilerArgs", "");
	env.GetFromGlobalVarOrDefault(additionalLinkerArgs, "additionalLinkerArgs", "");
	env.GetFromGlobalVarOrDefault(compilerExe, "compilerExe", "");
	env.GetFromGlobalVarOrDefault(firstLinkedObjects, "firstLinkedObjects", "");
	AssignVectorToConstElementVector(
		env.GetVectorFromGlobalTable("includePaths"), includePaths);
	env.GetFromGlobalVarOrDefault(linkerExe, "linkerExe", "");
	AssignVectorToConstElementVector(
		env.GetVectorFromGlobalTable("linkerLibraryPaths"), linkerLibraryPaths);
	env.GetFromGlobalVarOrDefault(oSwitch, "oSwitch", "");
	environmentVariables = env.GetStringPairsFromGlobalTable("environmentVariables");
	AssignVectorToConstElementVector(
		env.GetVectorFromGlobalTable("paths"), paths);
	preprocessorDirectives = env.GetStringPairsFromGlobalTable("preprocessorDirectives");
}

END_NAMESPACE

#endif
