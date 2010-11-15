#ifndef MACARONI_ENVIRONMENT_LUA_H
#define MACARONI_ENVIRONMENT_LUA_H

#include "../ME.h"
//extern "C" {
#include "../../Lua/lua.h"
#include "../../Lua/lauxlib.h"
#include "../../Lua/lualib.h"
//}

#include <fstream>
#include <string>
#include "StringPair.h"
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Environment)

/** Lua environment that we can do Macaroni stuff in. */
class LuaEnvironment
{
public:
	LuaEnvironment();
	~LuaEnvironment();
	
	/** Creates a new table in otherL at the top of the stack and populates
	 * it with the pairs of strings. */
	static void CreateNewTableFromStringPairs(lua_State * otherL, std::vector<StringPair> & pairs);

	void GetFromCurrentTableVarOrDefault(std::string & rtnValue, const char * name, const char * dflt);

	void GetFromGlobalVarOrDefault(std::string & rtnValue, const char * name, const char * dflt);

	/** It would be pointless to wrap every call to Lua, and that isn't the
	 * point of this class- the point is it creates a common place to put code
	 * for loading files and registering modules to enforce consistency. */
	lua_State * GetState();

	void BLARGOS();

	std::vector<StringPair> GetStringPairsFromGlobalTable(const char * tableName);

	std::vector<StringPair> GetStringPairsFromTable();

	static std::vector<StringPair> GetStringPairsFromTable(lua_State * L, bool errorIfNotStrings);

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
		
	/** Transforms a table in Lua to a String that can be executed. */
	void SerializeTable(std::stringstream & ss, const std::string & tableName);

	void SetPackageDirectory(const std::string & path);

	void SetPackageDirectory(const std::vector<std::string> & paths);

private:
	static const char * loadFile(lua_State * L, void * data, size_t *size);
	static const char * loadString(lua_State * L, void * data, size_t *size);
	std::ifstream * input;
	void registerInternalLuaModules();
	lua_State * state;

	void serializeField(std::stringstream & ss, int depth);

	void serializeString(std::stringstream & cereal, std::string str);

	void serializeTable(std::stringstream & cereal, int depth);

};

END_NAMESPACE2

#endif
