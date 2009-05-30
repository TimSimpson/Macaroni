#ifndef MACARONI_ENVIRONMENT_LUA_CPP
#define MACARONI_ENVIRONMENT_LUA_CPP

#include "LuaEnvironment.h"
#include "../Exception.h"
#include <iostream>
#include <sstream>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Environment)


LuaEnvironment::LuaEnvironment()
:input(nullptr)
{
	state = luaL_newstate();
	luaL_openlibs(state);
	registerInternalLuaModules();
}

LuaEnvironment::~LuaEnvironment()
{
	lua_close(state);
	if (input != nullptr)
	{
		input->close();
		delete input;
	}
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

void LuaEnvironment::ParseFile(std::string filePath)
{
	input = new std::ifstream(filePath.c_str(), std::ios::binary);
	if (!input->is_open())
	{
		std::cerr << "Could not open file!";
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
			std::cerr << ss.str() <<  std::endl;
			//throw new Macaroni::Exception(ss);
		}
		input->close();			
	}
}

void LuaEnvironment::Run()
{
	int eCode = lua_pcall(state, 0, 0, 0);
	if (eCode != 0)
	{	
		std::stringstream ss;
		ss << "Error running Lua:";
		ss << luaL_checkstring(this->state, -1);
		std::cerr << ss.str() << std::endl;
		//throw new Macaroni::Exception(ss);
	}
}

END_NAMESPACE2

#endif
