#ifndef MACARONI_PARSER_CPP_CPPPARSER_CPP
#define MACARONI_PARSER_CPP_CPPPARSER_CPP

#include "CppParser.h"
#include "../../Model/Context.h"
#include "CppParserState.h"
#include "../../Model/Source.h"
#include "CppParser.spirit"
#include <boost/algorithm/string.hpp>



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
	
	if (!info.full)
    {
		std::string remainder(info.stop);
		boost::trim(remainder);
		if (remainder.size() > 0)
		{
			std::stringstream ss;
			ss << "Parsing error:";
			ss << info.stop;
			//throw ParserException(source, ss.str());
		}
    } 

	//if (!info.hit)
	//{
	//	// Couldn't parse anything.
	//	throw ParserException(source, std::string(
	//		"Syntax or parser error."
	//		));
	//}
	///*else
	//{
	//	if (!info.full)
	//	{			
	//		throw ParserException(state.GetCurrentSource(),
	//			std::string("An unknown error occured while parsing source."));
	//	}
	//}*/
	return 1;
}

END_NAMESPACE

#endif