#ifndef MACARONI_PARSERSTATE_CPP
#define MACARONI_PARSERSTATE_CPP

#include "ParserState.h"

BEGIN_NAMESPACE2(Macaroni, Model)

ParserState::ParserState()
{
	context = Context::Create(std::string("{ROOT}"), std::string("*"));
	currentNamespace = context->GetRootNamespace();//Namespace::CreateRoot(std::string("{ROOT}"));
	namespaceStack.push_back(currentNamespace);
}

END_NAMESPACE2

#endif
