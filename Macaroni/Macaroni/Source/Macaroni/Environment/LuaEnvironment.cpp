#ifndef MACARONI_ENVIRONMENT_LUA_CPP
#define MACARONI_ENVIRONMENT_LUA_CPP

#include "LuaEnvironment.h"
#include "DebugLog.h"
#include "../Exception.h"
#include <iostream>
#include <sstream>
#include <string>

using Macaroni::Environment::DebugLog;

BEGIN_NAMESPACE2(Macaroni, Environment)


LuaEnvironment::LuaEnvironment()
:input(nullptr)
{
	DEBUGLOG_WRITE("Creating a new Lua state.");
	state = luaL_newstate();
	DEBUGLOG_WRITE("Opening standard Lua libs.");
	luaL_openlibs(state);
	DEBUGLOG_WRITE("Opening Macaroni Internal Lua Modules.");
	registerInternalLuaModules();
}

LuaEnvironment::~LuaEnvironment()
{
	DEBUGLOG_WRITE("Destroying Lua Environment; Calling close.");
	lua_close(state);
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
		rtnValue = std::string(lua_tolstring(state, -1, NULL));
	}
	else
	{
		rtnValue = std::string(dflt);
	}
	lua_pop(state, 1);
}

std::vector<const std::string> LuaEnvironment::GetVectorFromCurrentTable(const char * tableName)
{
	if (!(lua_istable(state, -1)))
	{
		throw Macaroni::Exception("Lua Table expected to be on top of stack before call to get local table.");
	}

	std::vector<const std::string> vec;

	lua_pushstring(state, tableName); // push key to get table
	lua_gettable(state, -2); // get table
	if (lua_istable(state, -1))
	{
		vec = GetVectorFromTable();		
	}

	lua_pop(state, 1);
	return vec;
}

std::vector<const std::string> LuaEnvironment::GetVectorFromGlobalTable(const char * tableName)
{
	lua_getglobal(state, tableName);
	if (lua_isnil(state, -1))
	{
		std::vector<const std::string> vec;
		return vec;
	}
	else
	{
		return GetVectorFromTable();
	}
	lua_pop(state, 1);
}

std::vector<const std::string> LuaEnvironment::GetVectorFromTable()
{
	std::vector<const std::string> vec;	
	
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
	DEBUGLOG_WRITE("Opening file-\\ ");
	DEBUGLOG_WRITE(filePath);

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
			throw Macaroni::Exception(ss.str().c_str());
		}
		input->close();			
	}

	DEBUGLOG_WRITE("\\- Finished reading file.");
}

void LuaEnvironment::ParseString(const char * chunkName, const char * code)
{
	DEBUGLOG_WRITE("Parsing code:");
	DEBUGLOG_WRITE(code);

	lua_Reader reader = loadString;
	int eCode = lua_load(this->state, reader, (void *) code, chunkName);
	if (eCode != 0)
	{	
		std::stringstream ss;
		ss << "Error while loading Lua string:";
		ss << luaL_checkstring(this->state, -1);
		//std::cerr << ss.str() <<  std::endl;
		throw Macaroni::Exception(ss.str().c_str());
	}
}

void LuaEnvironment::Run()
{
	DEBUGLOG_WRITE("Run...");
	int eCode = lua_pcall(state, 0, 0, 0);
	if (eCode != 0)
	{	
		std::stringstream ss;
		ss << "Error running Lua:";
		ss << luaL_checkstring(this->state, -1);
		//std::cerr << ss.str() << std::endl;
		throw Macaroni::Exception(ss.str().c_str());
	}
	DEBUGLOG_WRITE("... run complete.");
}

END_NAMESPACE2

#endif
