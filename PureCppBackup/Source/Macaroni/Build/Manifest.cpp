#ifndef MACARONI_BUILD_MANIFEST_CPP
#define MACARONI_BUILD_MANIFEST_CPP

#include "Manifest.h"
#include "../Exception.h"
#include "../Environment/LuaEnvironment.h"
#include <iostream>
#include <fstream>
#include "../IO/Paths.h"

using Macaroni::Environment::LuaEnvironment;
using Macaroni::IO::Paths;

namespace Macaroni { namespace Build {

Configuration createConfiguration(LuaEnvironment & env, const char * name);
std::vector<const Configuration> createConfigurations(LuaEnvironment & env);
//void getFromLuaVarOrDefault(std::string & rtnValue, lua_State * L, const char * name, const char * dflt);
//std::vector<const std::string> getVectorFromGlobalLuaTable(lua_State * L, const char * name);
//std::vector<const std::string> getVectorFromLocalLuaTable(lua_State * L, const char * name);
//std::vector<const std::string> getVectorFromLuaTable(lua_State * L);
void setManifestId(ManifestId & id, lua_State * L);

Manifest::Manifest()
:configurations(),
 cppHeadersOutput(),
 cppOutput(""),
 description(""),
 fOutput(""),
 group(""),
 id(),
 manifestFile(),
 mOutput(""),
 name(""),
 rootDirectory(),
 version("")
{
}

Manifest::Manifest(const boost::filesystem::path & manifestFile)
:description(""),
 id(),
 manifestFile(manifestFile),
 rootDirectory(manifestFile.branch_path())
{
	LuaEnvironment env;
	
	boost::filesystem::path settingsPath(Paths::GetUserPath());
	settingsPath = settingsPath / "Settings";
	env.SetPackageDirectory(settingsPath.string());

	env.ParseFile(manifestFile.string());
	env.Run();
	lua_State * L = env.GetState();

	setManifestId(id, L);
	
	boost::filesystem::path manifestDir = manifestFile;
	manifestDir.remove_filename();	

	env.GetFromGlobalVarOrDefault(description, "description", "");
	
	mSource = env.GetVectorFromGlobalTable("mSource");
	if (mSource.size() == 0)
	{
		mSource.push_back("Source");
	}
	for(unsigned int i = 0; i < mSource.size(); i ++)
	{
		mSource[i] = (manifestDir / mSource[i]).string();
	}	

	lua_getglobal(env.GetState(), "id");
	if (!lua_isnil(env.GetState(), -1))
	{
		env.GetFromCurrentTableVarOrDefault(group, "group", "Unknown");
		env.GetFromCurrentTableVarOrDefault(name, "name", manifestFile.string().c_str());
		env.GetFromCurrentTableVarOrDefault(version, "version", "???");			
	}
	lua_pop(env.GetState(), 1);


	lua_getglobal(env.GetState(), "cppOutput");
	if (!lua_isnil(env.GetState(), -1))
	{
		env.GetFromGlobalVarOrDefault(cppHeadersOutput, "headers", "MWork/Headers");
		env.GetFromCurrentTableVarOrDefault(cppOutput, "objects", "MWork/Objects");	
		env.GetFromGlobalVarOrDefault(cppSourceOutput, "cpp", "MWork/Cpp");
	}
	lua_pop(env.GetState(), 1);

	//env.GetFromGlobalVarOrDefault(cppOutput, "cppOutput", "MWork/Objects");
	//	env.GetFromGlobalVarOrDefault(iOutput, "iOutput", "MWork/Interface");

	env.GetFromGlobalVarOrDefault(fOutput, "fOutput", "MWork/Final");

	env.GetFromGlobalVarOrDefault(mOutput, "mOutput", "MWork/GeneratedSource");

	mOutput = (manifestDir / mOutput).string();
	

	configurations = createConfigurations(env);
}

std::vector<const Configuration> createConfigurations(LuaEnvironment & env)
{
	std::vector<const Configuration> configs;

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

	std::string additionalLinkerArgs;
	env.GetFromCurrentTableVarOrDefault(additionalLinkerArgs, "additionalLinkerArgs", "");
	config.SetAdditionalLinkerArgs(additionalLinkerArgs);


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
	config.SetCompiler(compiler);
	
	std::vector<const std::string> cInclude = 
		env.GetVectorFromCurrentTable("cInclude");
	config.SetCInclude(cInclude);

	std::vector<const ConfigurationId> dependencies;
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
				ManifestId d;
				d.SetGroup(group);
				d.SetName(name);
				d.SetVersion(version);
				ConfigurationId cId;
				cId.SetManifestId(d);
				cId.SetName(configName);

				dependencies.push_back(cId);
			}
			lua_pop(env.GetState(), 1); // pops off value, saves key.
		}
	}
	lua_pop(env.GetState(), 1); // pop off "dependnency" table.
	config.SetDependencies(dependencies);
	// Stack should be back to normal now.

	std::vector<const std::string> libs = 
		env.GetVectorFromCurrentTable("linkerLibraries");	
	config.SetLinkerLibraries(libs);	

	std::string final;
	env.GetFromCurrentTableVarOrDefault(final, "final", "final");
	config.SetFinal(final);

	std::vector<const std::string> generators = env.GetVectorFromCurrentTable("generators");
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

//
//void getFromLuaVarOrDefault(std::string & rtnValue, lua_State * L, const char * name, const char * dflt)
//{
//	lua_getglobal(L, name);
//	if (!lua_isnil(L, -1))
//	{
//		rtnValue = std::string(lua_tolstring(L, -1, NULL));
//	}
//	else
//	{
//		rtnValue = std::string(dflt);
//	}
//	lua_pop(L, 1);
//}
//
//std::vector<const std::string> getVectorFromLocalLuaTable(lua_State * L, const char * name)
//{
//	std::vector<const std::string> vec;
//
//	lua_pushstring(L, name); // push key to get table
//	lua_gettable(L, -2); // get table
//	if (lua_istable(L, -1))
//	{
//		vec = getVectorFromLuaTable(L);		
//	}
//
//	lua_pop(L, 1);
//	return vec;
//}
//
//std::vector<const std::string> getVectorFromGlobalLuaTable(lua_State * L, const char * name)
//{
//	lua_getglobal(L, name);
//	if (lua_isnil(L, -1))
//	{
//		std::vector<const std::string> vec;
//		return vec;
//	}
//	else
//	{
//		return getVectorFromLuaTable(L);
//	}
//	lua_pop(L, 1);
//}
//
//std::vector<const std::string> getVectorFromLuaTable(lua_State * L)
//{
//	std::vector<const std::string> vec;	
//	
//	lua_pushnil(L); // first key
//	const int tableIndex = -2;
//	while(lua_next(L, tableIndex)  != 0)
//	{
//		if (lua_isstring(L, -1))
//		{
//			std::string newStr(lua_tolstring(L, -1, NULL));
//			vec.push_back(newStr);
//		}
//		lua_pop(L, 1); // pops off value, saves key
//	}
//	
//	return vec;
//}

void Manifest::SaveAs(boost::filesystem::path & filePath)
{
	using std::endl;

	this->manifestFile = filePath;
	this->rootDirectory = filePath.branch_path();

	std::ofstream file;
	file.open(filePath.string().c_str());
	try
	{
		file << "id =" << endl
			<< "{" << endl
			<< "\tgroup=[[" << this->GetGroup() << "]]," << endl
			<< "\tname=[[" << this->GetName() << "]]," << endl
			<< "\tversion=[[" << this->GetVersion() << "]]" << endl
			<< "}" << endl;
		file << "description=[[" << this->GetDescription() << "]]" << endl;
		
		file << "mSource=" << endl
			<< "{" << endl;			
		for (unsigned int i = 0; i < this->GetMSource().size(); i ++)
		{
			file << "\t[[" << this->GetMSource()[i] << "]]," << endl;
		}		
		file << "}" << endl;

		file << "mOutput=[[" << this->GetMOutput() << "]]" << endl;

		////file << "cppInput=" << endl
		////	<< "{" << endl;
		////for (unsigned int i = 0; i < this->GetCpp
		////file << "}" << endl;
		

		file << "cppOutput={" << endl;
		file << "\tobjects=[[" << this->GetCppOutput() << "]]," << endl;		
		file << "}" << endl;

		file << "fOutput=[[" << this->GetFinalOutput() << "]]" << endl;
		
		file << "configurations = " << endl
			<< "{" << endl;
		for (unsigned int i = 0; i < this->configurations.size(); i ++)
		{
			const Configuration & config = this->configurations[i];
			file << "\t" << config.GetName() << " = " << endl
				<< "\t{" << endl;
			file << "\t\tcompiler=[[" << config.GetCompiler() << "]]," << endl;
			file << "\t\tcInclude=" << endl
				<< "\t\t{" << endl;			
			for (unsigned int i = 0; i < config.GetCInclude().size(); i ++)
			{
				file << "\t\t\t[[" << config.GetCInclude()[i] << "]]," << endl;
			}		
			file << "\t\t}," << endl;
			file << "\t\tdependencies=" << endl
				 << "\t\t{" << endl;
			for (unsigned int j = 0; j < config.GetDependencies().size(); j ++)
			{
				const ConfigurationId & id = config.GetDependencies()[j];
				file << "\t\t\t{" << endl;
				file << "\t\t\t\tgroup=[[" << id.GetManifestId().GetGroup() << "]]," << endl;
				file << "\t\t\t\tname=[[" << id.GetManifestId().GetName() << "]]," << endl;
				file << "\t\t\t\tversion=[[" << id.GetManifestId().GetVersion() << "]]," << endl;
				file << "\t\t\t\tconfiguration=[[" << id.GetName() << "]]," << endl;
				file << "\t\t\t}" << endl;
			}
			file << "\t\t}," << endl;

			file << "\t\tlinkerLibraries=" << endl
				<<  "\t\t{" << endl;
			for (unsigned int j = 0; j < config.GetLinkerLibraries().size(); j ++)
			{
				file << "\t\t\t[[" << config.GetLinkerLibraries()[j] << "]]," << endl;
			}
			file << "\t\t}," << endl;
			file << "\t\tfinal=[[" << config.GetFinal() << "]]," << endl;
			file << "\t}," << endl;
		}
		file << "}" << endl;
	}
	catch(std::exception & ex)
	{
		file.close();
		throw ex;
	}
	file.close();
}

void setManifestId(ManifestId & id, lua_State * L)
{
	lua_getglobal(L, "id");
	if (!lua_istable(L, -1))
	{
		throw Macaroni::Exception("Table not found in Manifest file.");
	}

	lua_pushstring(L, "group");
	lua_gettable(L, -2);
	id.SetGroup(std::string(lua_tolstring(L, -1, NULL)));
	lua_pop(L, 1);

	lua_pushstring(L, "name");
	lua_gettable(L, -2);
	id.SetName(std::string(lua_tolstring(L, -1, NULL)));
	lua_pop(L, 1);


	lua_pushstring(L, "version");
	lua_gettable(L, -2);
	id.SetVersion(std::string(lua_tolstring(L, -1, NULL)));
	lua_pop(L, 1);

	lua_pop(L, 1);
}




} }

#endif
