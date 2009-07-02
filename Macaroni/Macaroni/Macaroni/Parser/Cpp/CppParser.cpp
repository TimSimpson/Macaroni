#ifndef MACARONI_PARSER_CPP_CPPPARSER_CPP
#define MACARONI_PARSER_CPP_CPPPARSER_CPP

#include "CppParser.h"
#include "../../Model/Context.h"
#include "CppParserState.h"
#include "../../Model/Source.h"
#include "CppParser.spirit"


using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)


CppParser::CppParser()
{
	//p = nullptr;
}

CppParser::~CppParser()
{
}

CppParserPtr CppParser::Create()
{
	return CppParserPtr(new CppParser());
}

int CppParser::Read(ContextPtr c, SourcePtr source, const std::string & text)
{
	CppParserState state;
	state.SetContext(c);
	state.SetCurrentSource(source);
	DocumentGrammar cppGrammar;
	ParserActions actors(state);
	cppGrammar.actors = &actors;

	parse_info<> info = parse(
		text.c_str(),
		cppGrammar,
		space_p);

	return 1;
}

END_NAMESPACE

#endif