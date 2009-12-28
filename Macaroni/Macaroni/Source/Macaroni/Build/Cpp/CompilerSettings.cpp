#ifndef MACARONI_BUILD_CPP_COMPILERSETTINGS_CPP
#define MACARONI_BUILD_CPP_COMPILERSETTINGS_CPP

#include "CompilerSettings.h"
#include "../../Environment/LuaEnvironment.h"

using Macaroni::Environment::LuaEnvironment;

BEGIN_NAMESPACE(Macaroni, Build, Cpp)

CompilerSettings::CompilerSettings()
:additionalCompilerArgs(""),
 additionalLinkerArgs(""),
 compilerExe(""),
 environmentVariables(),
 firstLinkedObjects(),
 includePaths(),
 linkerExe(),
 linkerPaths(),
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
 linkerPaths(),
 oSwitch(),
 paths(),
 preprocessorDirectives()
{
	LuaEnvironment env;
	env.ParseFile(filePath.string());
	env.Run();
	lua_State * L = env.GetState();

	env.GetFromGlobalVarOrDefault(additionalCompilerArgs, "additionalCompilerArgs", "");
	env.GetFromGlobalVarOrDefault(additionalLinkerArgs, "additionalLinkerArgs", "");
	env.GetFromGlobalVarOrDefault(compilerExe, "compilerExe", "");
	env.GetFromGlobalVarOrDefault(firstLinkedObjects, "firstLinkedObjects", "");
	includePaths = env.GetVectorFromGlobalTable("includePaths");
	env.GetFromGlobalVarOrDefault(linkerExe, "linkerExe", "");
	linkerPaths = env.GetVectorFromGlobalTable("linkerPaths");
	env.GetFromGlobalVarOrDefault(oSwitch, "oSwitch", "");
	environmentVariables = env.GetVectorFromGlobalTable("environmentVariables");
	paths = env.GetVectorFromGlobalTable("paths");
	preprocessorDirectives = env.GetVectorFromGlobalTable("preprocessorDirectives");
}

END_NAMESPACE

#endif