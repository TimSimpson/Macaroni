#ifndef MACARONI_PARSER_PARSERFACTORY_H
#define MACARONI_PARSER_PARSERFACTORY_H

#include "../ME.h"
#include "Parser.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

/** Grabs a Parser based on hard-coded conventions such as file name 
  * extensions. */
class ParserFactory
{
public:
	ParserPtr GetParserForFileType(const std::string & fileType);

};

END_NAMESPACE2

#endif