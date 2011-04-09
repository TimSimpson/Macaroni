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
	/** Creates a new LuaEnvironment.  If no lua_State is given, then one
	 *  is created and destroyed when this object is destroyed.
	 *  If one is given, then its used and left alone when this object is
	 *  destroyed. */
	LuaEnvironment(lua_State * L = nullptr);
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
	std::vector<std::string> GetVectorFromCurrentTable(const char * tableName);

	/** Finds the table of the given name and reads each element into the String
	 * Vector which is returned. */
	std::vector<std::string> GetVectorFromGlobalTable(const char * tableName);	

	/** Assumes that a table is on top of the stack, and reads its elements into
	 * Vector of Strings. */
	std::vector<std::string> GetVectorFromTable();	

	void ParseFile(std::string filePath);

	void ParseString(const char * chunkName, const char * code);

	void Run(int results = 0);

	/** Serializes the value at the top of the stack to a string. */
	void SerializeField(std::stringstream & ss, int depth = 0);

	/** Transforms a table in Lua to a String that can be executed. */
	void SerializeTable(std::stringstream & ss, const std::string & tableName);

	void SetPackageDirectory(const std::string & path);

	void SetPackageDirectory(const std::vector<std::string> & paths);

private:
	std::ifstream * input;
	bool iOwnLuaState;	
	static const char * loadFile(lua_State * L, void * data, size_t *size);
	static const char * loadString(lua_State * L, void * data, size_t *size);	
	void registerInternalLuaModules();	
	void serializeString(std::stringstream & cereal, std::string str);
	void serializeTable(std::stringstream & cereal, int depth);
	lua_State * state;	
};

END_NAMESPACE2

#endif
