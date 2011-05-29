#ifndef MACARONI_ENVIRONMENT_LUA_CPP
#define MACARONI_ENVIRONMENT_LUA_CPP

#include "LuaEnvironment.h"
#include <boost/foreach.hpp>
#include "DebugLog.h"
#include "../Exception.h"
#include <iostream>
#include <sstream>
#include <string>


BEGIN_NAMESPACE2(Macaroni, Environment)


LuaEnvironment::LuaEnvironment(lua_State * L)
:input(nullptr), iOwnLuaState(L == nullptr), state(nullptr)
{
	if (L == nullptr)
	{
		state = luaL_newstate();
		luaL_openlibs(state);
		registerInternalLuaModules();
	}
	else 
	{
		state = L;
	}
}

LuaEnvironment::~LuaEnvironment()
{
	if (iOwnLuaState)
	{
		lua_close(state);		
	}
	if (input != nullptr)
	{
		input->close();
		delete input;
	}
}

void LuaEnvironment::BLARGOS()
{
	//luaL_getmetatable(L, "Macaroni.Parser.Parser");
	//luaL_getmetatable(L, "Macaroni.Parser.ParserException");
	//if (lua_isnil(L, -1) != 1)
	//{
	//	return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	//}			
}

void LuaEnvironment::CreateNewTableFromStringPairs(lua_State * L, std::vector<StringPair> & pairs)
{
	lua_newtable(L);
	BOOST_FOREACH(StringPair & pair, pairs)
	{
		lua_pushstring(L, pair.Name.c_str());
		lua_pushstring(L, pair.Value.c_str());
		lua_rawset(L, -3);
	}
}

void LuaEnvironment::GetFromCurrentTableVarOrDefault(std::string & rtnValue, const char * name, const char * dflt)
{
	lua_pushstring(state, name);
	lua_gettable(state, -2);
	if (lua_isstring(state, -1))
	{
		rtnValue = lua_tostring(state, -1);
	}
	else
	{
		rtnValue = dflt;
	}
	lua_pop(state, 1);
}

void LuaEnvironment::GetFromGlobalVarOrDefault(std::string & rtnValue, const char * name, const char * dflt)
{
	lua_getglobal(state, name);
	if (!lua_isnil(state, -1))
	{
		if (lua_isstring(state, -1))
		{
			rtnValue = std::string(lua_tolstring(state, -1, NULL));
		}
		else
		{
			std::stringstream ss;
			ss << "The value with name " 
				<< name << " was expected to be nil or a string, but it was "
				"something else.";
			MACARONI_FAIL(ss.str().c_str());
		}
	}
	else
	{
		rtnValue = std::string(dflt);
	}
	lua_pop(state, 1);	
}

std::vector<StringPair> LuaEnvironment::GetStringPairsFromGlobalTable(const char * tableName)
{
	lua_getglobal(state, tableName);
	if (lua_isnil(state, -1))
	{
		std::vector<StringPair> bag;
		return bag;
	}
	else
	{
		return GetStringPairsFromTable();
	}
	lua_pop(state, 1);
}

std::vector<StringPair> LuaEnvironment::GetStringPairsFromTable()
{
	return GetStringPairsFromTable(state, false);
}

std::vector<StringPair> LuaEnvironment::GetStringPairsFromTable(lua_State * L, bool errorIfNotStrings)
{
	std::vector<StringPair> bag;
	
	lua_pushnil(L); // first key
	const int tableIndex = -2;
	while(lua_next(L, tableIndex)  != 0)
	{
		StringPair entry;		
		if (lua_isstring(L, -2))
		{
			entry.Name = lua_tolstring(L, -2, NULL);
			if (lua_isstring(L, -1))
			{
				entry.Value = lua_tolstring(L, -1, NULL);				
				bag.push_back(entry);
			}
			else if (errorIfNotStrings)
			{
				std::stringstream ss;
				ss << "Non-string entry found for table value '"
					<< entry.Name << "'.";
				luaL_error(L, ss.str().c_str());
			}
		}
		lua_pop(L, 1); // pops off value, saves key
	}
	
	return bag;
}

std::vector<std::string> LuaEnvironment::GetVectorFromCurrentTable(const char * tableName)
{
	if (!(lua_istable(state, -1)))
	{
		throw Macaroni::Exception("Lua Table expected to be on top of stack before call to get local table.");
	}

	std::vector<std::string> vec;

	lua_pushstring(state, tableName); // push key to get table
	lua_gettable(state, -2); // get table
	if (lua_istable(state, -1))
	{
		vec = GetVectorFromTable();		
	}

	lua_pop(state, 1);
	return vec;
}

std::vector<std::string> LuaEnvironment::GetVectorFromGlobalTable(const char * tableName)
{
	lua_getglobal(state, tableName);
	if (lua_isnil(state, -1))
	{
		std::vector<std::string> vec;
		return vec;
	}
	else
	{
		return GetVectorFromTable();
	}
	lua_pop(state, 1);
}

std::vector<std::string> LuaEnvironment::GetVectorFromTable()
{
	std::vector<std::string> vec;	
	
	lua_pushnil(state); // first key
	const int tableIndex = -2;
	while(lua_next(state, tableIndex)  != 0)
	{
		if (lua_isstring(state, -1))
		{
			std::string newStr(lua_tolstring(state, -1, NULL));
			vec.push_back(newStr);
		}
		lua_pop(state, 1); // pops off value, saves key
	}
	
	return vec;
}

lua_State * LuaEnvironment::GetState()
{
	return state;
}

const char * LuaEnvironment::loadFile(lua_State * L, void * data, size_t * size)
{
	static char block[512];
	LuaEnvironment * object = (LuaEnvironment *) data;
	std::ifstream * input = object->input;		
	if (input->eof())
	{
		*size =0;
		return nullptr;
	}			
	input->read(block, 512);	
	*size = input->gcount();
		
	return block;		
}

const char * LuaEnvironment::loadString(lua_State * L, void * data, size_t * size)
{	
	const char * src = (const char *) data;
	*size = strnlen(src, 2^16);
	return src;
}

void LuaEnvironment::ParseFile(std::string filePath)
{	
	input = new std::ifstream(filePath.c_str(), std::ios::binary);
	if (!input->is_open())
	{
		std::stringstream ss;
		ss << "Could not open Lua file \"" << filePath << "\".\n";
		throw Macaroni::Exception(ss.str().c_str());
	}
	else
	{
		std::string line;
		lua_Reader reader = loadFile;
		int eCode = lua_load(this->state,reader, ((void *)this), filePath.c_str());

		if (eCode != 0)
		{	
			std::stringstream ss;
			ss << "Error while loading Lua file:";
			ss << luaL_checkstring(this->state, -1);
			//std::cerr << ss.str() <<  std::endl;
			Macaroni::ThrowMacaroniException(filePath.c_str(), 0, ss.str().c_str());
		}
		input->close();			
	}
}

void LuaEnvironment::ParseString(const char * chunkName, const char * code)
{
	//lua_Reader reader = loadString;
	//int eCode = lua_load(this->state, reader, (void *) code, chunkName);
	int eCode = luaL_loadbuffer(this->state, code, strnlen(code, 80 * 10000), chunkName);
		//|| lua_pcall(L, 0, 0, 0);
	if (eCode != 0)
	{	
		std::stringstream ss;
		ss << "Error while loading Lua string:";
		ss << luaL_checkstring(this->state, -1);
		//std::cerr << ss.str() <<  std::endl;
		throw Macaroni::Exception(ss.str().c_str());
	}
}

void LuaEnvironment::Run(int results)
{
	int eCode = lua_pcall(state, 0, results, 0);
	if (eCode != 0)
	{	
		std::stringstream ss;
		ss << "Error running Lua:";
		ss << luaL_checkstring(this->state, -1);
		//std::cerr << ss.str() << std::endl;
		throw Macaroni::Exception(ss.str().c_str());
	}	
}

void LuaEnvironment::SerializeField(std::stringstream & cereal, int depth) 
{
	if (lua_isstring(state, -1))
	{
		std::string value(lua_tostring(state, -1));
		cereal << "\"";
		serializeString(cereal, value);
		cereal << "\"";
	}
	else if (lua_isnumber(state, -1))
	{
		double d = lua_tonumber(state, -1);
		cereal << d;
	}
	else if (lua_isboolean(state, -1))
	{
		int b = lua_toboolean(state, -1);
		cereal << ((b == 1) ? "true" : "false");
	}
	else if (lua_istable(state, -1))
	{
		serializeTable(cereal, depth + 1);
	}
	else if (lua_isnil(state, -1))
	{
		cereal << "nil";
	}
	else 
	{
		std::stringstream msg;
		msg << "Element within table had a value whose type cannot be "
			<< "handled by the serialize function. " 
			<< "Current data is as follows: " << cereal;
		throw Macaroni::Exception(msg.str().c_str());	
	}
}

void LuaEnvironment::serializeString(std::stringstream & cereal, std::string str)
{	
	BOOST_FOREACH(char ch, str)
	{
		if (ch == '\"')
		{
			cereal << "\\\"";
		}
		else if (ch == '\\')
		{
			cereal << "\\\\";
		}
		else 
		{
			cereal << ch;
		}
	}
}

void LuaEnvironment::SerializeTable(std::stringstream & ss, 
										   const std::string & tableName)
{
	lua_getglobal(state, tableName.c_str());
	if (!lua_istable(state, -1))
	{
		std::stringstream msg;
		msg << "Can't find table value " << tableName << ".";
		throw Macaroni::Exception(msg.str().c_str());
	}
	serializeTable(ss, 0);
}

/** Expects table to be at -1 on the stack. */
void LuaEnvironment::serializeTable(std::stringstream & cereal, int depth) 
{
	if (!lua_istable(state, -1))
	{		
		throw Macaroni::Exception(
			"Expected table to be at top of stack for invocation of "
			"serializeTable function.");
	}	
	cereal << "{ " << std::endl;
	lua_pushnil(state); // first key
	const int tableIndex = -2;
	while(lua_next(state, tableIndex)  != 0)
	{
		for (int i = 0; i < depth; i ++)
		{
			cereal << "\t";
		}
		if (!lua_isstring(state, -2))
		{
			std::stringstream msg;
			msg << "Element within table had a key which was not a string. "
				<< "Serialize function cannot handle this.  The current " 
				<< "serialized data is as follows: " << cereal;
			throw Macaroni::Exception(msg.str().c_str());
		}
		cereal << "[\"";
		std::string keyName(lua_tostring(state, -2));
		serializeString(cereal, keyName);		
		cereal << "\"]";
		cereal << " = ";
		SerializeField(cereal, depth);	
		cereal << ", ";
		cereal << std::endl;
		lua_pop(state, 1); // pops off value, saves key		
	}
	for (int i = 0; i < depth - 1; i ++)
	{
		cereal << "\t";
	}
	cereal << "}";	
}







void LuaEnvironment::SetPackageDirectory(const std::vector<std::string> & paths)
{
	lua_getglobal(GetState(), "package");
	lua_pushstring(GetState(), "path");
	std::stringstream ss;
	BOOST_FOREACH(const std::string & path, paths)
	{
		ss << path << "/?.lua;";
	}	
	lua_pushstring(GetState(), ss.str().c_str());
	lua_settable(GetState(), -3);
	lua_pop(GetState(), 1);
}

void LuaEnvironment::SetPackageDirectory(const std::string & path)
{
	std::vector<std::string> vec;
	vec.push_back(path);
	SetPackageDirectory(vec);
}

END_NAMESPACE2

#endif
