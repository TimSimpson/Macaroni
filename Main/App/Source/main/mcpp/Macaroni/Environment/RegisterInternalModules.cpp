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
#ifndef MACARONI_ENVIRONMENT_REGISTERINTERNALMODULES_CPP
#define MACARONI_ENVIRONMENT_REGISTERINTERNALMODULES_CPP

#include "LuaEnvironment.h"
#include <Macaroni/Model/AnnotationTableLua.h>
#include <Macaroni/Model/AnnotationValueLua.h>
#include <Macaroni/Model/AxiomLua.h>
#include <Macaroni/Model/BlockLua.h>
#include <Macaroni/Model/Cpp/ClassLua.h>
#include <Macaroni/Model/Cpp/ClassParentListLuaMetaData.h>
#include <Macaroni/Model/ContextLua.h>
//#include <Macaroni/Model/Document.h>
#include <Macaroni/Model/ElementLua.h>
#include <Macaroni/Model/Cpp/FunctionLua.h>
#include <Macaroni/Model/Cpp/FunctionOverloadLua.h>
#include <Macaroni/Model/LibraryLua.h>
#include <Macaroni/Doc/MDocParser.h>
//#include <Macaroni/Model/MemberLua.h>
#include <Macaroni/Environment/MessagesLua.h>
//#include <Macaroni/Model/NamespaceLua.h>
#include <Macaroni/Model/NodeListLua.h>
#include <Macaroni/Parser/ParserLua.h>
#include <Macaroni/Parser/ParserExceptionLua.h>
#include <Macaroni/IO/PathLua.h>
#include <Macaroni/IO/Paths.h>
#include <Macaroni/Parser/Pippy/PippyParserLua.h>
#include <sstream>
#include <Macaroni/Model/TypeLua.h>
#include <Macaroni/Model/TypeArgumentListLua.h>
#include <Macaroni/Model/TypeArgumentLua.h>
#include <Macaroni/Model/Cpp/TypedefLua.h>
#include <Macaroni/Model/TypeListLua.h>
#include <Macaroni/Model/Cpp/VariableLua.h>
#include <Macaroni/Model/Cpp/VariableAssignmentLua.h>
#include <Macaroni/VersionNo.h>
#include <LuaModulesInclude_Macaroni___Macaroni_46_App___lib.h>

BEGIN_NAMESPACE2(Macaroni, Environment)

static const struct luaL_Reg libs[] = {
	{"Macaroni.Environment.Messages", Environment::MessagesLuaMetaData::OpenInLua},
	{"Macaroni.Model.AnnotationTable", Model::AnnotationTableLuaMetaData::OpenInLua},
	{"Macaroni.Model.AnnotationValue", Model::AnnotationValueLuaMetaData::OpenInLua},
	{"Macaroni.Model.Axiom", Model::AxiomLuaMetaData::OpenInLua},
	{"Macaroni.Model.Block", Model::BlockLuaMetaData::OpenInLua},
	{"Macaroni.Model.Cpp.Class", Model::Cpp::ClassLuaMetaData::OpenInLua},
	{"Macaroni.Model.Cpp.ClassParentList", Model::Cpp::ClassParentListLuaMetaData::OpenInLua},
	{"Macaroni.Model.Context", Model::ContextLuaMetaData::OpenInLua},
	{"Macaroni.Model.Element", Model::ElementLuaMetaData::OpenInLua},
	{"Macaroni.Model.Cpp.Function", Model::Cpp::FunctionLuaMetaData::OpenInLua},
	{"Macaroni.Model.Cpp.FunctionOverload", Model::Cpp::FunctionOverloadLuaMetaData::OpenInLua},
	{"Macaroni.Model.Library", Model::LibraryLuaMetaData::OpenInLua},
	{"Macaroni.Doc.MDocParser", Macaroni::Doc::MDocParser::OpenInLua},
	//{"Macaroni.Model.Member", Model::MemberLuaMetaData::OpenInLua},
	{"Macaroni.Model.NodeList", Model::NodeListLuaMetaData::OpenInLua},
	{"Macaroni.Parser.Parser", Parser::ParserLuaMetaData::OpenInLua},
	{"Macaroni.Parser.ParserException", Parser::ParserExceptionLuaMetaData::OpenInLua},
	{"Macaroni.IO.Path", IO::PathLuaMetaData::OpenInLua},
	{"Macaroni.IO.PathList", IO::PathListLuaMetaData::OpenInLua},
//	{"Macaroni.Parser.Cpp.CppParser", Parser::Cpp::CppParserLuaMetaData::OpenInLua},
	{"Macaroni.Parser.Pippy.PippyParser", Parser::Pippy::PippyParserLuaMetaData::OpenInLua},
	{"Macaroni.Model.Type", Macaroni::Model::TypeLuaMetaData::OpenInLua},
	{"Macaroni.Model.TypeArgument", Macaroni::Model::TypeArgumentLuaMetaData::OpenInLua},
	{"Macaroni.Model.TypeArgumentList", Macaroni::Model::TypeArgumentListLuaMetaData::OpenInLua},
	{"Macaroni.Model.Cpp.TypeDef", Macaroni::Model::Cpp::TypedefLuaMetaData::OpenInLua},
	{"Macaroni.Model.TypeList", Macaroni::Model::TypeListLuaMetaData::OpenInLua},
	{"Macaroni.Model.Cpp.Variable", Macaroni::Model::Cpp::VariableLuaMetaData::OpenInLua},
	{"Macaroni.Model.Cpp.VariableAssignment", Macaroni::Model::Cpp::VariableAssignmentLuaMetaData::OpenInLua},
	#include <LuaModulesRegister_Macaroni___Macaroni_46_App___lib.h>
	{nullptr, nullptr} /* sentinel */
	};

void LuaEnvironment::OpenAllLuaModules()
{
	std::stringstream ss;
	for (const luaL_Reg * ptr = libs; ptr->name != nullptr; ptr ++)
	{
		ss << "require \"" << ptr->name << "\";" << std::endl;
	}
	ParseString("Require-All-Internal", ss.str().c_str());
	Run();
}

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
	// can use it.
	// First, force require to use the current working path.
	std::string directory = Macaroni::IO::Paths::GetExeDirectoryPath().string();

	lua_pushstring(state, MACARONI_VERSION_STRING);
	lua_setglobal(state, "MACARONI_VERSION");

	lua_pushstring(state, directory.c_str());
	lua_setglobal(state, "LUA_PATH");

	lua_pushstring(state, directory.c_str());
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
