#ifndef MACARONI_PARSERSTATE_CPP
#define MACARONI_PARSERSTATE_CPP

#include "ParserState.h"

BEGIN_NAMESPACE2(Macaroni, Model)

ParserState::ParserState()
{
	context = new Context(std::string("{ROOT}"));
	currentNode = context->GetRoot();//Namespace::CreateRoot(std::string("{ROOT}"));
	namespaceStack.push_back(currentNode);
}

END_NAMESPACE2

#endif
