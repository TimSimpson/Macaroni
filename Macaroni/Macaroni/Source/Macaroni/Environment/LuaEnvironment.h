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
	
	/** It would be pointless to wrap every call to Lua, and that isn't the
	 * point of this class- the point is it creates a common place to put code
	 * for loading files and registering modules to enforce consistency. */
	lua_State * GetState();

	void BLARGOS();

	void ParseFile(std::string filePath);

	void ParseString(const char * chunkName, const char * code);

	void Run();
private:
	static const char * loadFile(lua_State * L, void * data, size_t *size);
	static const char * loadString(lua_State * L, void * data, size_t *size);
	std::ifstream * input;
	void registerInternalLuaModules();
	lua_State * state;
};

END_NAMESPACE2

#endif
