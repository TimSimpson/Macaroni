#ifndef MACARONI_BUILD_MANIFEST_CPP
#define MACARONI_BUILD_MANIFEST_CPP

#include "Manifest.h"
#include "../Environment/LuaEnvironment.h"

using Macaroni::Environment::LuaEnvironment;

namespace Macaroni { namespace Build {

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
}

void setManifestId(ManifestId & id, lua_State * L)
{
	lua_getglobal(L, "id");
	
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
