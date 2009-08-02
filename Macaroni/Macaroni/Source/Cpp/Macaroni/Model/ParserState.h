#ifndef MACARONI_PARSERSTATE_H
#define MACARONI_PARSERSTATE_H

#include "../ME.h"
#include "Class.h"
#include "Context.h"
#include "Import.h"
#include "Namespace.h"
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

/** This class contains internal variables that are needed by the Spirit parser.
 * But we don't want to add a Parser to any class definition if we can avoid it,
 * so instead we keep these instead. */
class ParserState
{
friend ParserActions;
public:
	ParserState();
protected:
	ContextPtr context;
	ClassPtr currentClass;
	std::string currentClassName;
	ImportPtr currentImport;
	NamespacePtr currentNamespace;
	std::string currentNamespaceName;
	std::string currentBaseClassAccessKeyword;
	std::vector<NamespacePtr> namespaceStack;
};

END_NAMESPACE2

#endif
