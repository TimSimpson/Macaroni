#ifndef MACARONI_BUILD_MANIFEST_CPP
#define MACARONI_BUILD_MANIFEST_CPP

#include "Manifest.h"
#include "../Exception.h"
#include "../Environment/LuaEnvironment.h"

using Macaroni::Environment::LuaEnvironment;

namespace Macaroni { namespace Build {

Configuration createConfiguration(LuaEnvironment & env, const char * name);
std::vector<const Configuration> createConfigurations(LuaEnvironment & env);
void getFromLuaVarOrDefault(std::string & rtnValue, lua_State * L, const char * name, const char * dflt);
//std::vector<const std::string> getVectorFromGlobalLuaTable(lua_State * L, const char * name);
//std::vector<const std::string> getVectorFromLocalLuaTable(lua_State * L, const char * name);
//std::vector<const std::string> getVectorFromLuaTable(lua_State * L);
void setManifestId(ManifestId & id, lua_State * L);

Manifest::Manifest(const boost::filesystem::path & manifestFile)
:description(""),
 id()
{
	LuaEnvironment env;
	env.ParseFile(manifestFile.string());
	env.Run();
	lua_State * L = env.GetState();

	setManifestId(id, L);
	
	boost::filesystem::path manifestDir = manifestFile;
	manifestDir.remove_filename();

	cInclude = env.GetVectorFromGlobalTable("cInclude");
	if (cInclude.size() == 0)
	{
		cInclude.push_back("MWork/GeneratedSource");
	}
	for(unsigned int i = 0; i < mSource.size(); i ++)
	{		
		cInclude[i] = (manifestDir / cInclude[i]).string();
	}

	getFromLuaVarOrDefault(description, L, "description", "");
	
	mSource = env.GetVectorFromGlobalTable("mSource");
	if (mSource.size() == 0)
	{
		mSource.push_back("Source");
	}
	for(unsigned int i = 0; i < mSource.size(); i ++)
	{
		mSource[i] = (manifestDir / mSource[i]).string();
	}	

	getFromLuaVarOrDefault(mOutput, L, "mOutput", "MWork/GeneratedSource");
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
	return nullptr;
}


void getFromLuaVarOrDefault(std::string & rtnValue, lua_State * L, const char * name, const char * dflt)
{
	lua_getglobal(L, name);
	if (!lua_isnil(L, -1))
	{
		rtnValue = std::string(lua_tolstring(L, -1, NULL));
	}
	else
	{
		rtnValue = std::string(dflt);
	}
	lua_pop(L, 1);
}
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
