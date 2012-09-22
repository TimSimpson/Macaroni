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
#ifndef MACARONI_ENVIRONMENT_LUA_H
#define MACARONI_ENVIRONMENT_LUA_H

#include "../ME.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <Macaroni/Environment/Values/Any.h>
#include <Macaroni/Environment/Values/AnyPtr.h>
#include <fstream>
#include <Macaroni/InternalSource.h>
#include <string>
#include <boost/optional.hpp>
#include <sstream>
#include "StringPair.h"
#include <Macaroni/Environment/Values/Table.h>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Environment)

/** Lua environment that we can do Macaroni stuff in. */
class LuaEnvironment
{
public:
	///*class FunctionSetup
	//{
	//	virtual int operator()(lua_State * L) = 0;
	//};*/

	/** Creates a new LuaEnvironment.  If no lua_State is given, then one
	 *  is created and destroyed when this object is destroyed.
	 *  If one is given, then its used and left alone when this object is
	 *  destroyed. */
	LuaEnvironment(lua_State * L = nullptr);
	~LuaEnvironment();

	/** Adds a package directory that can be found with the "required"
	 *  keyword. */
	void AddPackageDirectory(const std::string & path);

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

	/** Grabs the given table as Lua code. Only primitives are allowed to be
	 *  there. */
	std::string GetTableAsLuaCode(const char * tableName);

	/** Assumes that a table is at the top of the stack, and finds a table within
	 * this table to read an array from. */
	std::vector<std::string> GetVectorFromCurrentTable(const char * tableName);

	/** Finds the table of the given name and reads each element into the String
	 * Vector which is returned. */
	std::vector<std::string> GetVectorFromGlobalTable(const char * tableName);

	/** Assumes that a table is on top of the stack, and reads its elements into
	 * Vector of Strings. */
	std::vector<std::string> GetVectorFromTable();

	/** "Requires" every single module defined internally. */
	void OpenAllLuaModules();

	void ParseFile(std::string filePath);

	void ParseString(const char * chunkName, const char * code);

	void Run(int results = 0);

	/** Calls Lua, but uses pcall so a Macaroni::Exception is thrown instead of
	 *  the "PANIC" message which is harder to pin down.
	 *  Call with __FILE__, __LINE__ for best results.
	 */
	static void Run(const Macaroni::InternalSource & source,
		            lua_State * L, int args, int results);

	/////* Calls a function. The functor must push all of this to the top of the
	//// * stack. */
	////static void Run(const Macaroni::InternalSource & source,
	////	            lua_State * L, FunctionSetup & funcSetup, int results);

	/** Serializes the value at the top of the stack to a string. */
	static void SerializeField(lua_State * L, std::stringstream & ss,
		                       int depth = 0, int fieldIndex = -1);

	void SerializeField(std::stringstream & ss, int depth = 0,
		                int fieldIndex = -1);

	/** Transforms a table in Lua to a String that can be executed. */
	void SerializeTable(std::stringstream & ss, const std::string & tableName);

	/** Like the above but uses whatever is at the top of the stack. */
	static void SerializeTable(lua_State * L, std::stringstream & ss);

	/** Serializes whatever value is in the stack at the position given by index
	 *  to an Any. If "key" is given it is used when throwing an exception
	 *  to specify where the error occurred. */
	static Values::AnyPtr SerializeValue(lua_State * L, int index,
		boost::optional<std::string> key=boost::none);

	/** Given a table on the Lua stack at the given index, fills in the given
	 *  table value. */
	static void SerializeTable(Values::Table & table, lua_State * L, int index,
		                    boost::optional<std::string> key=boost::none);

	/** Sets the package directories (clears previous settings). */
	void SetPackageDirectory(const std::string & path);

	/** Sets a single package directory (clears previous settings). */
	void SetPackageDirectory(const std::vector<std::string> & paths);

private:
	std::ifstream * input;
	bool iOwnLuaState;
	static const char * loadFile(lua_State * L, void * data, size_t *size);
	static const char * loadString(lua_State * L, void * data, size_t *size);
	void registerInternalLuaModules();
	std::vector<std::string> packageDirectories;
	static void serializeString(lua_State * L, std::stringstream & cereal,
								std::string str);
	static void serializeTable(lua_State * L, std::stringstream & cereal,
							   int depth);
	void setPackagePath();
	lua_State * state;
};

END_NAMESPACE2

#endif
