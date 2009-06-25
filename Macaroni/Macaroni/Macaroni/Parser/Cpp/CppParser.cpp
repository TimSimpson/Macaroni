#ifndef MACARONI_PARSER_CPP_CPPPARSER_CPP
#define MACARONI_PARSER_CPP_CPPPARSER_CPP

#include "CppParser.h"
#include "../../Model/Context.h"
#include "../../Model/Source.h"

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


int CppParser::Read(ContextPtr c, SourcePtr source, const std::string & text)
{
	return 1;
}

END_NAMESPACE

#endif