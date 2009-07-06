#ifndef MACARONI_PARSER_PIPPY_PIPPYPARSER_CPP
#define MACARONI_PARSER_PIPPY_PIPPYPARSER_CPP

#include "../../ME.h"
#include "../../Model/Context.h"
#include "../../Model/FileName.h"
#include "PippyParser.h"
#include "../Parser.h"

#include "PippyParser.spirit"


using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;

BEGIN_NAMESPACE(Macaroni, Parser, Pippy)

PippyParser::PippyParser()
{
}

PippyParser::~PippyParser()
{
}

PippyParserPtr PippyParser::Create()
{
	return PippyParserPtr(new PippyParser());
}

int PippyParser::Read(Model::ContextPtr c, Model::SourcePtr source, const std::string & text)
{
	DocumentGrammar cppGrammar;

	const char * c_str = text.c_str();
	typedef position_iterator<char const*> iterator_t;
	iterator_t begin(c_str, 
					 c_str + text.length(), 
					 source->GetFileName()->GetName().c_str());
	iterator_t end;

	parse_info<iterator_t> info = parse(
		//text.c_str(),
		begin, end,
		cppGrammar,
		space_p);

	return 1;
}

END_NAMESPACE

#endif