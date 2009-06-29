#ifndef MACARONI_PARSER_CPP_CPPPARSERSTATE_H
#define MACARONI_PARSER_CPP_CPPPARSERSTATE_H

#include "../../ME.h"
#include "../../Model/Context.h"
#include <string>

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

/** This class contains internal variables that are needed by the Spirit parser.
 * But we don't want to add a Parser to any class definition if we can avoid it,
 * so instead we keep these instead. */
struct CppParserState
{
public:
	CppParserState(){};

	Model::ContextPtr context;
	NodePtr currentNode;

	// The parser 
	std::string currentNamespaceName;

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
