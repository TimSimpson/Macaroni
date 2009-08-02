#ifndef MACARONI_PARSER_SOURCE_H
#define MACARONI_PARSER_SOURCE_H

#include "../ME.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

class Source
{
public:
	Source(const std::string & file, int lineNumber)
		: file(file), lineNumber(lineNumber)
	{
	}
	
	~Source();
	
	std::string GetFile() { return file; }

	int GetLineNumber() { return lineNumber; }

private:
	std::string file;

	int lineNumber;
};

END_NAMESPACE2

#endif

