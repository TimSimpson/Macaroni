#ifndef MACARONI_PARSER_CPP_CPPPARSERSTATE_H
#define MACARONI_PARSER_CPP_CPPPARSERSTATE_H

#include "../../ME.h"
#include <Macaroni/Model/Context.h>
#include <Macaroni/Model/ContextPtr.h>
#include "../../Model/SourcePtr.h"
#include <string>

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

/** This class contains internal variables that are needed by the Spirit parser.
 * But we don't want to add a Parser to any class definition if we can avoid it,
 * so instead we keep these instead. */
class CppParserState
{
public:
	CppParserState();

	Model::ContextPtr context;
	Model::NodePtr currentNode;
	Model::NodePtr currentScope;

	// The parser
	std::string currentNamespaceName;

	Model::SourcePtr GetCurrentSource();

	void IncrementLineNumber();

	int lineNumber;

	void SetContext(Model::ContextPtr context);

	void SetCurrentSource(Model::SourcePtr source);

private:

	Model::SourcePtr currentSource;
};

END_NAMESPACE

#endif
