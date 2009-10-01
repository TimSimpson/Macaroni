#ifndef MACARONI_ENVIRONMENT_REGISTERINTERNALMODULES_CPP
#define MACARONI_ENVIRONMENT_REGISTERINTERNALMODULES_CPP

#include "LuaEnvironment.h"
#include "../Model/AccessLua.h"
#include "../Model/AxiomLua.h"
//#include "../Model/ClassLua.h"
#include "../Model/ContextLua.h"
//#include "../Model/Document.h"
#include "../Model/FileNameLua.h"
#include "../Model/MemberLua.h"
#include "MessagesLua.h"
//#include "../Model/NamespaceLua.h"
#include "../Model/NodeLua.h"
#include "../Model/ReasonLua.h"
#include "../Parser/ParserLua.h"
#include "../Parser/ParserExceptionLua.h"
#include "../Generator/Output/PathLua.h"
#include "../Parser/Cpp/CppParserLua.h"
#include "../Parser/Pippy/PippyParserLua.h"
#include "../Model/SourceLua.h"
#include <sstream>
#include "../Model/TypeLua.h"
#include "../Model/TypeArgumentListLua.h"
#include "../Model/TypeArgumentLua.h"
#include "../Model/Cpp/VariableAssignmentLua.h"
#include <windows.h>

BEGIN_NAMESPACE2(Macaroni, Environment)

static const struct luaL_Reg libs[] = {
	{"Macaroni.Environment.Messages", Environment::MessagesLuaMetaData::OpenInLua},
	{"Macaroni.Model.Access", Model::AccessLuaMetaData::OpenInLua},
	{"Macaroni.Model.Axiom", Model::AxiomLuaMetaData::OpenInLua},
	//MARIO {"Macaroni.Model.Class", Model::ClassLuaMetaData::OpenInLua},
	{"Macaroni.Model.Context", Model::ContextLuaMetaData::OpenInLua},
	//MARIO {"Macaroni.Model.Document", Model::Document::OpenInLua},
	{"Macaroni.Model.FileName", Model::FileNameLuaMetaData::OpenInLua},
	{"Macaroni.Model.Member", Model::MemberLuaMetaData::OpenInLua},
	//MARIO {"Macaroni.Model.Namespace", Model::NamespaceLuaMetaData::OpenInLua},
	{"Macaroni.Model.Node", Model::NodeLuaMetaData::OpenInLua},
	{"Macaroni.Model.Reason", Model::ReasonLuaMetaData::OpenInLua},	
	{"Macaroni.Model.Source", Model::SourceLuaMetaData::OpenInLua},
	{"Macaroni.Parser.Parser", Parser::ParserLuaMetaData::OpenInLua},
	{"Macaroni.Parser.ParserException", Parser::ParserExceptionLuaMetaData::OpenInLua},
	{"Macaroni.Generator.Output.Path", Generator::Output::PathLuaMetaData::OpenInLua},
	{"Macaroni.Parser.Cpp.CppParser", Parser::Cpp::CppParserLuaMetaData::OpenInLua},
	{"Macaroni.Parser.Pippy.PippyParser", Parser::Pippy::PippyParserLuaMetaData::OpenInLua},
	{"Macaroni.Model.Type", Macaroni::Model::TypeLuaMetaData::OpenInLua},
	{"Macaroni.Model.TypeArgument", Macaroni::Model::TypeArgumentLuaMetaData::OpenInLua},
	{"Macaroni.Model.TypeArgumentList", Macaroni::Model::TypeArgumentListLuaMetaData::OpenInLua},
	{"Macaroni.Model.Cpp.VariableAssignment", Macaroni::Model::Cpp::VariableAssignmentLuaMetaData::OpenInLua},
	{nullptr, nullptr} /* sentinel */
	};

void LuaEnvironment::registerInternalLuaModules()
{
	// How this works:
	// To make something available via "require" in lua, you must stick
	// it into the "preload" table of the "package" table.

	// Put package table on the stack.
	lua_getglobal(state, "package");
	
	// Put the preload table of the package table on the stack.
	lua_pushstring(state, "preload");
	lua_gettable(state, -2); // pops key "preload", puts table on stack
    
	// Registers everything into "package.preload."
	// This way they are not loaded until lua calls
	// "require."
	luaL_register(state, nullptr, libs);

	// Override how the default "require" functionality works.
	// Try to keep true to the spirit of require, just fudge it a little so we
	// can *actually use it*.
	// First, force require to use the current working path.
	#define SZDIRECTORYSIZE 256
	TCHAR szDirectory[SZDIRECTORYSIZE];
	MACARONI_ASSERT(GetCurrentDirectory(SZDIRECTORYSIZE - 1, szDirectory) != 0,
					"Failure getting working directory!");
	char directory[SZDIRECTORYSIZE];
	int ret = WideCharToMultiByte(CP_ACP, 0, szDirectory, -1, 
								  directory, 256, NULL, NULL);
	MACARONI_ASSERT(ret != 0, "Couldn't convert the dumb directory string.");

	lua_pushstring(state, directory);
	lua_setglobal(state, "LUA_PATH");

	lua_pushstring(state, directory);
	lua_setglobal(state, "package.path");

	std::stringstream ss;	
	ss << std::string(directory);
	ss << "\\?.lua";
	lua_getglobal(state, "package");
	lua_pushstring(state, ss.str().c_str());
	lua_setfield(state, -2, "path");
	
	// blue pill, 73
	// SET environment variable LUA_PATH to current working directory.

	// Take required function, override it with a function which 
	// given 
};

END_NAMESPACE2

#endif
