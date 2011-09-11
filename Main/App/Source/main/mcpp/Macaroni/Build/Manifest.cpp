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
#ifndef MACARONI_BUILD_MANIFEST_CPP
#define MACARONI_BUILD_MANIFEST_CPP

#include <boost/foreach.hpp>
#include "Manifest.h"
#include <Macaroni/Containers.h>
#include <Macaroni/Model/ContextLua.h>
#include <Macaroni/Generator/DynamicGeneratorRunner.h>
#include "../Exception.h"
#include <Macaroni/Build/GeneratorContext.h>
#include <Macaroni/Model/Library.h>
#include <Macaroni/Model/LibraryLua.h>
#include "../Environment/LuaEnvironment.h"
#include <boost/optional.hpp>
#include <iostream>
#include <fstream>
#include <Macaroni/IO/PathLua.h>
#include "../IO/Paths.h"
#include <sstream>
#include "../Environment/StringPair.h"

using boost::optional;
using Macaroni::Environment::Console;
using Macaroni::Model::ContextLuaMetaData;
using Macaroni::Model::ContextPtr;
using Macaroni::Generator::DynamicGeneratorRunner;
using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using Macaroni::Model::LibraryLuaMetaData;
using Macaroni::Environment::LuaEnvironment;
using boost::filesystem::path;
using Macaroni::IO::PathLuaMetaData;
using Macaroni::IO::PathPtr;
using Macaroni::IO::Paths;
using Macaroni::Environment::StringPair;

#define LATEST_LUA_VALUE "~LATEST~"

namespace Macaroni { namespace Build {

static int _cavatappi(lua_State * L);
static int _dependency(lua_State * L);
static int _getUpperLibrary(lua_State * L);
static int _source(lua_State * L);


Configuration createConfiguration(LuaEnvironment & env, const char * name);
std::vector<MACARONI_VE_CONST Configuration> createConfigurations(LuaEnvironment & env);
//void getFromLuaVarOrDefault(std::string & rtnValue, lua_State * L, const char * name, const char * dflt);
//std::vector<MACARONI_VE_CONST std::string> getVectorFromGlobalLuaTable(lua_State * L, const char * name);
//std::vector<MACARONI_VE_CONST std::string> getVectorFromLocalLuaTable(lua_State * L, const char * name);
//std::vector<MACARONI_VE_CONST std::string> getVectorFromLuaTable(lua_State * L);
void setLibraryId(LibraryId & id, lua_State * L);

//TODO: Does this get used?
Manifest::Manifest()
:allowCavatappi(false),
 children(),
 configurations(), 
 containsCavatappi(false),
 cppHeadersOutput(),
 cppOutput(""),
 dependencies(),
 description(""),
 fOutput(""),
 group(""),
 id(), 
 manifestFile(),
 mOutput(""),
 name(""),
 properties(),
 rootDirectory(),
 version("")
{
}

//TODO: Does anything use this?
Manifest::Manifest(LibraryId id, std::vector<LibraryId> deps)
:allowCavatappi(false),
 children(),
 configurations(),
 containsCavatappi(false),
 cppHeadersOutput(),
 cppOutput(""),
 dependencies(deps),
 description(""),
 fOutput(""),
 group(id.GetGroup()),
 id(id), 
 manifestFile(),
 mOutput(""),
 name(id.GetName()),
 properties(),
 rootDirectory(),
 version(id.GetVersion())
{
}

Manifest::Manifest(const boost::filesystem::path & manifestFile, 
				   const std::string & properties, 
				   ManifestPtr upperManifest,
				   const bool allowCavatappi)
:allowCavatappi(allowCavatappi),
 children(),
 containsCavatappi(false),
 dependencies(),
 description(""),
 id(), 
 luaEnv(),
 manifestFile(manifestFile),
 properties(properties),
 rootDirectory(manifestFile.branch_path())
{
	boost::filesystem::path settingsPath(Paths::GetUserPath());
	settingsPath = settingsPath / "Settings";
	luaEnv.SetPackageDirectory(settingsPath.string());

	// Establish the properties variable.
	std::stringstream code;	
	code << "properties = " << properties << ";" << std::endl;	
	//std::cerr << "CHUNK" << std::endl;
	//std::cerr << code.str() << std::endl;
	// This may be a frowned-upon practice, but this way the entire
	// code snippet is shown if something goes wrong.
	luaEnv.ParseString(properties.c_str(), code.str().c_str());
	luaEnv.Run();

	luaEnv.ParseFile(manifestFile.string());

	lua_State * L = luaEnv.GetState();

	lua_pushstring(L, LATEST_LUA_VALUE);	
	lua_setglobal(L, "LATEST");

	boost::filesystem::path mDirPath = 
		boost::filesystem::system_complete(manifestFile).parent_path();	
	lua_pushstring(L, mDirPath.string().c_str());
	lua_setglobal(L, "manifestDirectory");

	lua_pushlightuserdata(L, &(upperManifest));
	lua_pushcclosure(L, _getUpperLibrary, 1);
	lua_setglobal(L, "getUpperLibrary");

	optional<std::string> extraCavatappiInfo;
	lua_pushlightuserdata(L, &(extraCavatappiInfo));
	lua_pushcclosure(L, _cavatappi, 1);
	lua_setglobal(L, "cavatappi");

    lua_pushlightuserdata(L, &(this->mSource));
    lua_pushcclosure(L, &_source, 1);
	lua_setglobal(L, "source");

    lua_pushlightuserdata(L, &(this->dependencies));
	lua_pushlightuserdata(L, &(this->properties));
    lua_pushcclosure(L, &_dependency, 2);
	lua_setglobal(L, "dependency");

	luaEnv.Run();

	if (!!extraCavatappiInfo)
	{
		containsCavatappi = true;
		if (allowCavatappi) 
		{
			path cavatappiFile = manifestFile.branch_path() / extraCavatappiInfo.get();
			luaEnv.ParseFile(cavatappiFile.string());
			luaEnv.Run();
		}
	}
	


	setLibraryId(id, L);

	boost::filesystem::path manifestDir = manifestFile;
	manifestDir.remove_filename();

	luaEnv.GetFromGlobalVarOrDefault(description, "description", "");
	
	Macaroni::Containers::AssignVectorToConstElementVector(
		luaEnv.GetVectorFromGlobalTable("sources"), mSource);
///*
//	mSource = std::vector<MACARONI_VE_CONST std::string>();
//	BOOST_FOREACH(const std::string & elem, luaEnv.GetVectorFromGlobalTable("sources"))
//	{
//		mSource.push_back(elem);
//	}	*/
	if (mSource.size() == 0)
	{
		mSource.push_back("Source");
	}
	for(unsigned int i = 0; i < mSource.size(); i ++)
	{
		mSource[i] = (manifestDir / mSource[i]).string();
	}
	
	Macaroni::Containers::AssignVectorToConstElementVector(
		luaEnv.GetVectorFromGlobalTable("bugs"), bugs);
	
	Macaroni::Containers::AssignVectorToConstElementVector(
		luaEnv.GetVectorFromGlobalTable("children"), children);	

	lua_getglobal(luaEnv.GetState(), "allowChildFailure");
	if (lua_isboolean(luaEnv.GetState(), -1)) 
	{
		allowChildFailure = lua_toboolean(L, -1) == 1;
	}
	else
	{
		allowChildFailure = false;
	}
	
	lua_getglobal(luaEnv.GetState(), "id");
	if (!lua_isnil(luaEnv.GetState(), -1))
	{
		luaEnv.GetFromCurrentTableVarOrDefault(group, "group", "Unknown");
		luaEnv.GetFromCurrentTableVarOrDefault(name, "name", manifestFile.string().c_str());
		luaEnv.GetFromCurrentTableVarOrDefault(version, "version", "???");
	}
	lua_pop(luaEnv.GetState(), 1);


	lua_getglobal(luaEnv.GetState(), "cppOutput");
	if (!lua_isnil(luaEnv.GetState(), -1))
	{
		luaEnv.GetFromGlobalVarOrDefault(cppHeadersOutput, "headers", "MWork/Headers");
		luaEnv.GetFromCurrentTableVarOrDefault(cppOutput, "objects", "MWork/Objects");
		luaEnv.GetFromGlobalVarOrDefault(cppSourceOutput, "cpp", "MWork/Cpp");
	}
	lua_pop(luaEnv.GetState(), 1);

	//env.GetFromGlobalVarOrDefault(cppOutput, "cppOutput", "MWork/Objects");
	//	env.GetFromGlobalVarOrDefault(iOutput, "iOutput", "MWork/Interface");

	luaEnv.GetFromGlobalVarOrDefault(fOutput, "fOutput", "MWork/Final");

	luaEnv.GetFromGlobalVarOrDefault(mOutput, "output", "GeneratedSource");

	mOutput = (manifestDir / mOutput).string();


	configurations = createConfigurations(luaEnv);

	lua_gc(L, LUA_GCCOLLECT, NULL);
}

Manifest::~Manifest()
{
	//HACKY BUG FIX CODE...
	//						... REMOVE??!! (To be continued...)
    this->dependencies.clear();
    lua_gc(luaEnv.GetState(), LUA_GCCOLLECT, NULL);
    //luaEnv.~LuaEnvironment();
}

int _cavatappi(lua_State * L)
{
	// Up values
	void * uv1 = lua_touserdata(L, lua_upvalueindex(1));
	optional<std::string> * cavatappiFile = reinterpret_cast<optional<std::string> *>(uv1);
	// Argument
	std::string filePath(luaL_checkstring(L, 1));

	(*cavatappiFile) = filePath;
	return 0;
}


int _dependency(lua_State * L)
{
    LibraryId dId;
    lua_getfield(L, 1, "group");
    if (!lua_isnil(L, -1))
    {		
        dId.SetGroup(std::string(luaL_checkstring(L, -1)));
    }
    lua_pop(L, 1);
    lua_getfield(L, 1, "name");
    if (!lua_isnil(L, -1))
    {		
        dId.SetName(std::string(luaL_checkstring(L, -1)));
    }
    lua_pop(L, 1);
    lua_getfield(L, 1, "version");
    if (!lua_isnil(L, -1))
    {
        dId.SetVersion(std::string(luaL_checkstring(L, -1)));
    }
    lua_pop(L, 1);

	// NEW CODE, the code after this that's commented out is where I used
	// to stupidly load the entire Manifest on the spot.

	void * ptr = lua_touserdata(L, lua_upvalueindex(1));
	std::vector<LibraryId> * dependencies =
		reinterpret_cast<std::vector<LibraryId> *>(ptr);
	//void * ptr2 = lua_touserdata(L, lua_upvalueindex(2));
	//const std::string & properties = *(reinterpret_cast<std::string *>(ptr2));
	dependencies->push_back(dId);
	return 0;	
}

int _getUpperLibrary(lua_State * L)
{
	// Up values
	void * uv1 = lua_touserdata(L, lua_upvalueindex(1));
	ManifestPtr * manifestPtr = reinterpret_cast<ManifestPtr *>(uv1);
	if (!(*manifestPtr))
	{
		lua_pushnil(L);
	}
	else
	{
		LibraryId id = (*manifestPtr)->GetId();
		lua_newtable(L);
		lua_pushstring(L, "Group");
		lua_pushstring(L, id.GetGroup().c_str());
		lua_settable(L, -3);
		lua_pushstring(L, "Name");
		lua_pushstring(L, id.GetName().c_str());
		lua_settable(L, -3);
		lua_pushstring(L, "Version");
		lua_pushstring(L, id.GetVersion().c_str());
		lua_settable(L, -3);
	}	
	return 1;
}

int _runScript(lua_State * L)
{
	// Collect Up Values
	void * ptr = lua_touserdata(L, lua_upvalueindex(1));    
    const std::vector<std::string> * sources =
        reinterpret_cast<std::vector<std::string> *>(ptr);	
	std::string methodName(lua_tostring(L, lua_upvalueindex(2)));
	void * contextVP = lua_touserdata(L, lua_upvalueindex(3));
	BuildContextPtr & iCon = *(reinterpret_cast<BuildContextPtr *>(contextVP));
	void * runListVP = lua_touserdata(L, lua_upvalueindex(4));
	std::vector<Manifest::RunEntry> & runList = 
		*(reinterpret_cast<std::vector<Manifest::RunEntry> *>(runListVP));

	// Collect Arguments
	if (lua_gettop(L) < 1 || !lua_isstring(L, 1)) 
	{
		luaL_error(L, "Expected the name of a Lua script as argument.");
	}
	std::string scriptName(std::string(lua_tolstring(L, 1, NULL)));
	std::string argumentTableString;
	if (lua_gettop(L) > 1) 
	{
		if (!lua_istable(L, 2)) 
		{
			luaL_error(L, "An optional table filled only with strings is expected as the second argument.");
		}		
		std::stringstream ss;
		LuaEnvironment::SerializeTable(L, ss);
		argumentTableString = ss.str();		
	} else {
		argumentTableString = "{}";
	}
	
	DynamicGeneratorRunner runner(iCon->GetAppPaths());	
	// Find path to Lua script
	boost::filesystem::path scriptPath =
			runner.ResolveGeneratorPath(*sources, scriptName);
	// Run Lua script
	if (!scriptPath.empty())
	{
		try
		{
			//boost::filesystem::path output(iConpath->GetAbsolutePath());
			std::string returnValue = runner.RunDynamicGenerator(scriptPath,
				//iCon->GetLibrary(), 
				//						   iCon->GetOutputDir()->GetAbsolutePath(), 
				iCon,
				methodName,
				argumentTableString);
			if (returnValue.size() > 0)  // If 'nil' was returned, do nothing.
			{
				runList.push_back(Manifest::RunEntry(scriptName, returnValue));
			}
		}
		catch (const Macaroni::Exception & ex)
		{
			std::stringstream ss;
			ss << "An error occurred running the generator at " 
				<< scriptPath << ". C exception thrown from " << ex.GetSource()
				<< ". Message: " << ex.GetMessage();
			luaL_error(L, ss.str().c_str());	
		}		
		catch (const std::exception & ex)
		{
			luaL_error(L, ex.what());	
		}		
	}
	else
	{
		std::stringstream ss;
		ss << "Could not find generator " << scriptName << ".";		
		luaL_error(L, ss.str().c_str());
	}
	return 0;
}

int _source(lua_State * L)
{
    void * ptr = lua_touserdata(L, lua_upvalueindex(1));
    //Manifest * me = dynamic_cast<Manifest *>(oldThis);
    std::vector<std::string> * sources =
        reinterpret_cast<std::vector<std::string> *>(ptr);
    sources->push_back(std::string(luaL_checkstring(L, 1)));
    return 1;
}

bool Manifest::AllowChildFailure() const
{
	return allowChildFailure;
}

std::vector<MACARONI_VE_CONST Configuration> createConfigurations(LuaEnvironment & env)
{
	std::vector<MACARONI_VE_CONST Configuration> configs;

	lua_State * L = env.GetState();
	lua_getglobal(L, "configurations");
	if (!lua_isnil(L, -1))
	{
		lua_pushnil(L);
		while(lua_next(L, -2) != 0)
		{
			std::string name;
			if (lua_isstring(L, -2))
			{
				name = lua_tostring(L, -2);
			}
			else
			{
				name = "NO_NAME";
			}
			configs.push_back(createConfiguration(env, name.c_str()));
			lua_pop(L, 1);
		}
	}
	//lua_pop(L, 1);

	return configs;
}

Configuration createConfiguration(LuaEnvironment & env, const char * name)
{
	// Expects a configuration table to be on top of stack (-1)
	Configuration config;

	config.SetName(name);

	std::string compiler;
	env.GetFromCurrentTableVarOrDefault(compiler, "compiler", "default");
	/*
	lua_pushstring(env.GetState(), "compiler");
		lua_gettable(env.GetState(), -2);
		if (lua_isstring(env.GetState(), -1))
		{
			compiler = lua_tostring(env.GetState(), -1);
		}
		else
		{
			compiler = "default";
		}
		lua_pop(env.GetState(), 1);
	config.SetCompiler(compiler);
	*/
	std::vector<ConfigurationId> dependencies;
	lua_pushstring(env.GetState(), "dependencies");
	lua_gettable(env.GetState(), -2); // key "dependencies" is popped off.
	if (!lua_isnil(env.GetState(), -1))
	{
		lua_pushnil(env.GetState()); // first key
		while(lua_next(env.GetState(), -2) != 0)
		{
			if (lua_istable(env.GetState(), -1))
			{
				std::string group, name, version, configName;
				env.GetFromCurrentTableVarOrDefault(group, "group", "");
				env.GetFromCurrentTableVarOrDefault(name, "name", "");
				env.GetFromCurrentTableVarOrDefault(version, "version", "");
				env.GetFromCurrentTableVarOrDefault(configName, "configuration", "default");
				LibraryId d;
				d.SetGroup(group);
				d.SetName(name);
				d.SetVersion(version);
				ConfigurationId cId;
				cId.SetLibraryId(d);
				cId.SetName(configName);

				dependencies.push_back(cId);
			}
			lua_pop(env.GetState(), 1); // pops off value, saves key.
		}
	}
	lua_pop(env.GetState(), 1); // pop off "dependnency" table.
	config.SetDependencies(dependencies);
	// Stack should be back to normal now.

	std::vector<std::string> generators = env.GetVectorFromCurrentTable("generators");
	config.SetGenerators(generators);


	return config;
}

const Configuration * Manifest::GetConfiguration(const std::string & configName) const
{
	for(unsigned int i = 0; i < configurations.size(); i ++)
	{
		if (configurations[i].GetName() == configName)
		{
			return &configurations[i];
		}
	}
	if (configName == "default")
	{
		if (configurations.size() == 1)
		{
			return &configurations[0];
		}
	}
	return nullptr;
}

std::string Manifest::GetProperties()
{
	std::stringstream cereal;
	luaEnv.SerializeTable(cereal, "properties");
	return cereal.str();
}

Manifest::RunResultPtr Manifest::RunTarget(const Console & console, BuildContextPtr iContext, const std::string & manifestMethodName, const std::string & generatorMethodName)
{
	lua_State * L = luaEnv.GetState();

	LibraryLuaMetaData::OpenInLua(L);
	PathLuaMetaData::OpenInLua(L);
	
	RunResultPtr result(new RunResult());

	// Put the library, a table with all source directories, the output 
	// directory, and the install directory onto the stack.
	// THIS IS INSANELY DANGEROUS!
	// ... BUT I GUESS IT'LL WORK?
	// I DON'T WANT TO WRAP THIS IN LUA GLUE BECAUSE I'M LAZY.
	lua_pushlightuserdata(L, &(this->mSource));
	lua_pushstring(L, generatorMethodName.c_str());	
	lua_pushlightuserdata(L, (void *) &iContext);
	lua_pushlightuserdata(L, (void *) &(result->RunList));
/*
	LibraryLuaMetaData::PutInstanceOnStack(L, iContext->GetLibrary());
	lua_newtable(L);
	int index = 0;
	BOOST_FOREACH(const std::string & srcDirectory, this->GetMSource())
	{
		PathPtr src = iContext->GetSourceDir()->NewPathForceSlash(srcDirectory); 
		// In Lua, : newTable[index + 1] = src
		std::stringstream ss;		
		PathLuaMetaData::PutInstanceOnStack(L, src);
		ss << (index + 1);
		lua_setfield(L, -2, ss.str().c_str());
		index ++;
	}
	PathLuaMetaData::PutInstanceOnStack(L, iContext->GetOutputDir());
	PathLuaMetaData::PutInstanceOnStack(L, iContext->GetInstallDir());
	lua_pushlightuserdata(L, &(this->mSource));
	*/
	lua_pushcclosure(L, &_runScript, 4);//4);
	lua_setglobal(L, "run");	
	lua_getfield(L, LUA_GLOBALSINDEX, manifestMethodName.c_str());
	if (lua_isnil(L, -1))
	{	
		//std::stringstream ss;
		//ss << "Could not find function \"" << manifestMethodName << "\".";
		//console.WriteLine(ss.str());
		// As of 0.1.0.6, just return if the phase method isn't found.
		result->Success = true; //false;
		return result;
	}
	LuaEnvironment::Run(__FILE__, __LINE__, L, 0, 1);
	result->Success = true;
	return result;
	/*int success = lua_pcall(L, 0, 1, 0);
	if (success != 0) 
	{
		return false;
	}
	
	if (lua_isnil(L, -1)) 
	{
		return true;
	}
	int rtnValue = lua_toboolean(L, -1);
	return rtnValue;*/
}

void Manifest::SaveAs(const boost::filesystem::path & filePath, 
					  std::vector<std::pair<std::string, std::string> > & runList)
{
	using std::endl;

	this->manifestFile = filePath;
	this->rootDirectory = filePath.branch_path();

	std::ofstream file;
	file.open(filePath.string().c_str());
	try
	{
		file << "-- Final Manifest generated by Macaroni" << endl;
		file << endl;
		file << "id =" << endl
			<< "{" << endl
			<< "    group    = [[" << this->GetGroup() << "]]," << endl
			<< "    name     = [[" << this->GetName() << "]]," << endl
			<< "    version  = [[" << this->GetVersion() << "]]" << endl
			<< "}" << endl;
		file << endl;
		file << "description = [[" << this->GetDescription() << "]]" << endl;
		file << endl;
		file << "sources=" << endl
			<< "{" << endl;
		for (unsigned int i = 0; i < this->GetMSource().size(); i ++)
		{
			//file << "    [[" << this->GetMSource()[i] << "]]," << endl;
			file << "    [[Mh]]," << endl;
		}
		file << "}" << endl;
		file << endl;
		BOOST_FOREACH(const LibraryId & dep, dependencies)
		{
			file << "dependency { group   = [[" << dep.GetGroup() << "]]," << endl;
			file << "             name    = [[" << dep.GetName() << "]]," << endl;
			file << "             version = [[" << dep.GetVersion() << "]] }" << endl;
		}

		file << endl;

		file << "function prepare()" << endl;
		BOOST_FOREACH(RunEntry & runEntry, runList)
		{
			std::string & script = runEntry.first;
			std::string & args = runEntry.second;
			file << "    run(\"" << script << "\", " << args << ");" << endl;
		}
		file << "end" << endl;
		//file << "output=[[" << this->GetMOutput() << "]]" << endl;

		//

		//file << "cppOutput={" << endl;
		//file << "\tobjects=[[" << this->GetCppOutput() << "]]," << endl;
		//file << "}" << endl;

		//file << "fOutput=[[" << this->GetFinalOutput() << "]]" << endl;

		//file << "configurations = " << endl
		//	<< "{" << endl;
		//for (unsigned int i = 0; i < this->configurations.size(); i ++)
		//{
		//	const Configuration & config = this->configurations[i];
		//	file << "\t" << config.GetName() << " = " << endl
		//		<< "\t{" << endl;
		//	file << "\t\tdependencies=" << endl
		//		 << "\t\t{" << endl;
		//	for (unsigned int j = 0; j < config.GetDependencies().size(); j ++)
		//	{
		//		const ConfigurationId & id = config.GetDependencies()[j];
		//		file << "\t\t\t{" << endl;
		//		file << "\t\t\t\tgroup=[[" << id.GetLibraryId().GetGroup() << "]]," << endl;
		//		file << "\t\t\t\tname=[[" << id.GetLibraryId().GetName() << "]]," << endl;
		//		file << "\t\t\t\tversion=[[" << id.GetLibraryId().GetVersion() << "]]," << endl;
		//		file << "\t\t\t\tconfiguration=[[" << id.GetName() << "]]," << endl;
		//		file << "\t\t\t}" << endl;
		//	}
		//	file << "\t\t}," << endl;

		//	file << "\t}," << endl;
		//}
		//file << "}" << endl;
	}
	catch(std::exception & ex)
	{
		file.close();
		throw ex;
	}
	file.close();
}

void Manifest::SetBugResult(const std::string & bugName, 
							LibraryPtr libraryResult,
							boost::optional<std::string> description)
{
	//bugResults
	lua_State * L = luaEnv.GetState();
	//t
	lua_getglobal(L, "bugResults");
	if (lua_isnil(L, -1))
	{
		lua_pop(L, 1); // begone, foul null
		lua_newtable(L);
		lua_setglobal(L, "bugResults");
		lua_getglobal(L, "bugResults");
	} 
	//k
	lua_pushstring(L, bugName.c_str());
	//v
	lua_newtable(L);
	if (!!description)
	{
		lua_pushstring(L, "description");
		lua_pushstring(L, description.get().c_str());
		lua_settable(L, -3);
	}
	lua_pushstring(L, "library");
	LibraryLuaMetaData::PutInstanceOnStack(L, libraryResult);
	lua_settable(L, -3);
	lua_pushstring(L, "success");
	lua_pushboolean(L, (!!libraryResult && libraryResult->IsInstalled()));
	lua_settable(L, -3);
	
	lua_settable(L, -3); //t[k]=v
	lua_pop(L, 1); // begone, foul table!
}

void setLibraryId(LibraryId & id, lua_State * L)
{
	lua_getglobal(L, "id");
	if (!lua_istable(L, -1))
	{
		throw Macaroni::Exception("Table \"id\" not found in Manifest file.");
	}

	lua_pushstring(L, "group");
	lua_gettable(L, -2);
	id.SetGroup(std::string(luaL_checkstring(L, -1)));
	lua_pop(L, 1);

	lua_pushstring(L, "name");
	lua_gettable(L, -2);
	id.SetName(std::string(luaL_checkstring(L, -1)));
	lua_pop(L, 1);


	lua_pushstring(L, "version");
	lua_gettable(L, -2);
	id.SetVersion(std::string(luaL_checkstring(L, -1)));
	lua_pop(L, 1);

	lua_pop(L, 1);
}



} }

#endif
