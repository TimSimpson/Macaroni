#ifndef MACARONI_PARSER_PIPPY_PIPPYPARSER_H
#define MACARONI_PARSER_PIPPY_PIPPYPARSER_H

#include "../../ME.h"
#include "../../Model/Library.lh"
#include "PippyParser.lh"
#include "../Parser.h"

BEGIN_NAMESPACE(Macaroni, Parser, Pippy)

class PippyParser : public Macaroni::Parser::Parser
{
public:
	PippyParser();

	~PippyParser();

	static PippyParserPtr Create();

	virtual int Read(Model::LibraryPtr c, Model::SourcePtr source, const std::string & text);

	static void RunTests(); 

private:

private:
	
};

END_NAMESPACE

#endif