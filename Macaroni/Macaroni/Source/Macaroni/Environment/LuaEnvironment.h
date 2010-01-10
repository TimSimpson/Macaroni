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
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Environment)

/** Lua environment that we can do Macaroni stuff in. */
class LuaEnvironment
{
public:
	LuaEnvironment();
	~LuaEnvironment();
	
	void GetFromCurrentTableVarOrDefault(std::string & rtnValue, const char * name, const char * dflt);

	void GetFromGlobalVarOrDefault(std::string & rtnValue, const char * name, const char * dflt);

	/** It would be pointless to wrap every call to Lua, and that isn't the
	 * point of this class- the point is it creates a common place to put code
	 * for loading files and registering modules to enforce consistency. */
	lua_State * GetState();

	void BLARGOS();

	/** Assumes that a table is at the top of the stack, and finds a table within
	 * this table to read an array from. */
	std::vector<const std::string> GetVectorFromCurrentTable(const char * tableName);

	/** Finds the table of the given name and reads each element into the String
	 * Vector which is returned. */
	std::vector<const std::string> GetVectorFromGlobalTable(const char * tableName);	

	/** Assumes that a table is on top of the stack, and reads its elements into
	 * Vector of Strings. */
	std::vector<const std::string> GetVectorFromTable();	

	void ParseFile(std::string filePath);

	void ParseString(const char * chunkName, const char * code);

	void Run();

	void SetPackageDirectory(const std::string & path);
private:
	static const char * loadFile(lua_State * L, void * data, size_t *size);
	static const char * loadString(lua_State * L, void * data, size_t *size);
	std::ifstream * input;
	void registerInternalLuaModules();
	lua_State * state;
};

END_NAMESPACE2

#endif
