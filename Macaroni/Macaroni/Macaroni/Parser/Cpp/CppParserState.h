#ifndef MACARONI_PARSER_CPP_CPPPARSERSTATE_H
#define MACARONI_PARSER_CPP_CPPPARSERSTATE_H

#include "../../ME.h"
#include "../../Model/Context.h"
#include "../../Model/Source.lh"
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

/*protected:
	ContextPtr context;
	ClassPtr currentClass;
	std::string currentClassName;
	ImportPtr currentImport;
	NamespacePtr currentNamespace;
	std::string currentNamespaceName;
	std::string currentBaseClassAccessKeyword;
	std::vector<NamespacePtr> namespaceStack;*/
};

END_NAMESPACE

#endif
