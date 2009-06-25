#ifndef MACARONI_PARSER_DOCUMENT_H
#define MACARONI_PARSER_DOCUMENT_H

#include "../ME.h"
#include "../Model/Context.lh"
#include "DocumentPtr.h"
#include "Parser.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

/** Represents any Macaroni document which can be parsed. 
 * Parses nothing except for directives indicating the type of parser to use,
 * which start with #~.
 */
class Document
{
public:
	Document(ContextPtr context);
	
	~Document();
	
	ContextPtr GetContext();

	/** Reads a file. */
	void ReadFile(const std::string & fileName);

	///** Reads in the stream. */
	//void Read(SourcePtr source, istream stream);

	/** Reads from the given source, returning the lines read. */
	int Read(SourcePtr source, const std::string & text);

	void SetParser(Parser * value);

private:	

	Parser * parser;
};

END_NAMESPACE2

#endif
