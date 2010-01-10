#ifndef MACARONI_PARSER_PARSER_H
#define MACARONI_PARSER_PARSER_H

#include "../ME.h"
#include "../Model/Context.lh"
#include "../Model/Library.lh"
#include "Parser.lh"
#include "../Model/Source.lh"

BEGIN_NAMESPACE2(Macaroni, Parser)

/** Interface for parsing text into Model objects. */
class Parser
{
public:
	Parser(){}
	virtual ~Parser(){}

	/** Returns lines read.  Throws Model::ModelInconsistencyException, or 
	 * Parser::ParserException. */
	virtual int Read(Model::LibraryPtr l, // Library to read into.
					  Model::SourcePtr source, // Where the text begins. 
					  const std::string & text // Text.
					  ) = 0;
};

END_NAMESPACE2

#endif