#ifndef MACARONI_ENVIRONMENT_LUA_H
#define MACARONI_ENVIRONMENT_LUA_H

#include "../ME.h"
extern "C" {
#include "../../Lua/lua.h"
#include "../../Lua/lauxlib.h"
#include "../../Lua/lualib.h"
}

#include <fstream>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Environment)

/** Lua environment that we can do Macaroni stuff in. */
class LuaEnvironment
{
public:
	LuaEnvironment();
	~LuaEnvironment();

	void ParseFile(std::string filePath);
	void Run();
private:
	static const char * loadFile(lua_State * L, void * data, size_t *size);
	std::ifstream * input;
	void registerInternalLuaModules();
	lua_State * state;
};

END_NAMESPACE2

#endif
