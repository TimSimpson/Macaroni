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

static int dependency(lua_State * L);

static int _dependency(lua_State * L);
static int _source(lua_State * L);

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
 dependencies(),
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
:dependencies(),
 description(""),
 id(),
 luaEnv(),
 manifestFile(manifestFile),
 rootDirectory(manifestFile.branch_path())
{
	boost::filesystem::path settingsPath(Paths::GetUserPath());
	settingsPath = settingsPath / "Settings";
	luaEnv.SetPackageDirectory(settingsPath.string());

	luaEnv.ParseFile(manifestFile.string());

	lua_State * L = luaEnv.GetState();

    lua_pushlightuserdata(L, &(this->mSource));
    lua_pushcclosure(L, &_source, 1);
	lua_setglobal(L, "source");

    lua_pushlightuserdata(L, &(this->dependencies));
    lua_pushcclosure(L, &_dependency, 1);
	lua_setglobal(L, "dependency");

	luaEnv.Run();

	setManifestId(id, L);

	boost::filesystem::path manifestDir = manifestFile;
	manifestDir.remove_filename();

	luaEnv.GetFromGlobalVarOrDefault(description, "description", "");

	mSource = luaEnv.GetVectorFromGlobalTable("mSource");
	if (mSource.size() == 0)
	{
		mSource.push_back("Source");
	}
	for(unsigned int i = 0; i < mSource.size(); i ++)
	{
		mSource[i] = (manifestDir / mSource[i]).string();
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

	luaEnv.GetFromGlobalVarOrDefault(mOutput, "mOutput", "MWork/GeneratedSource");

	mOutput = (manifestDir / mOutput).string();


	configurations = createConfigurations(luaEnv);

	lua_gc(L, LUA_GCCOLLECT, NULL);
}

Manifest::~Manifest()
{
    std::cout << "Deleting " << this->id.GetGroup() << "::"
        << this->id.GetName() << "::"
        << this->id.GetVersion() << "...\n";
    for (unsigned int i = 0; i < dependencies.size(); i ++)
    {
        Manifest * m = dependencies[i];
        delete m;
    }
    this->dependencies.clear();
    lua_gc(luaEnv.GetState(), LUA_GCCOLLECT, NULL);
    luaEnv.~LuaEnvironment();
}

int _dependency(lua_State * L)
{
    ManifestId dId;
    lua_getfield(L, 1, "group");
    if (!lua_isnil(L, -1))
    {
        dId.SetGroup(std::string(lua_tostring(L, -1)));
    }
    lua_pop(L, 1);
    lua_getfield(L, 1, "name");
    if (!lua_isnil(L, -1))
    {
        dId.SetName(std::string(lua_tostring(L, -1)));
    }
    lua_pop(L, 1);
    lua_getfield(L, 1, "version");
    if (!lua_isnil(L, -1))
    {
        dId.SetVersion(std::string(lua_tostring(L, -1)));
    }
    lua_pop(L, 1);

    boost::filesystem::path manifestFilePath(dId.FindFinalManifestFile());

    void * ptr = lua_touserdata(L, lua_upvalueindex(1));
    std::vector<Manifest *> * dependencies =
        reinterpret_cast<std::vector<Manifest *> *>(ptr);

    dependencies->push_back(new Manifest(manifestFilePath));
    return 0;
}

int _source(lua_State * L)
{
    void * ptr = lua_touserdata(L, lua_upvalueindex(1));
    //Manifest * me = dynamic_cast<Manifest *>(oldThis);
    std::vector<const std::string> * sources =
        reinterpret_cast<std::vector<const std::string> *>(ptr);
    sources->push_back(std::string(lua_tolstring(L, 1, NULL)));
    return 1;
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

	std::vector<const std::string> generators = env.GetVectorFromCurrentTable("generators");
	config.SetGenerators(generators);


	return config;
}

int dependency(lua_State * L)
{
    void * ptr = lua_touserdata(L, lua_upvalueindex(1));
    //Manifest * me = dynamic_cast<Manifest *>(oldThis);
    std::vector<const std::string> * sources =
        reinterpret_cast<std::vector<const std::string> *>(ptr);
    sources->push_back(std::string(lua_tolstring(L, 1, NULL)));
    return 1;
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
