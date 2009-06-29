#ifndef MACARONI_PARSER_CPP_CPPPARSER_H
#define MACARONI_PARSER_CPP_CPPPARSER_H

#include <boost/spirit/core.hpp>
#include "../../ME.h"
#include "CppParser.lh"
#include "../Parser.h"

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

class CppParser : public Macaroni::Parser::Parser
{
public:
	CppParser();

	~CppParser();

	static CppParserPtr Create();

	virtual int Read(Model::ContextPtr c, Model::SourcePtr source, const std::string & text);
private:
	//boost::spirit::parser p;

	//void addNamespace(NamespacePtr ns);

private:
	/*
	void beginParse(const std::string & fileContents);
	
	std::vector<ClassPtr> classes;

	std::vector<std::string> comments;

	std::vector<ImportPtr> imports;

	std::vector<NamespacePtr> namespaces;

	void endParse();
	
	std::string name;

	ParserState parserState;*/
};

END_NAMESPACE

#endif