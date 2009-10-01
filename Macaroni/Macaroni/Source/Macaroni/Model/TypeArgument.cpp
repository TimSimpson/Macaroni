#ifndef MACARONI_MODEL_TYPEARGUMENT_CPP
#define MACARONI_MODEL_TYPEARGUMENT_CPP

#include "TypeArgument.h"

BEGIN_NAMESPACE2(Macaroni, Model)

TypeArgument::TypeArgument(NodePtr type)
:arguments(new NodeList()), node(type)
{
}

TypeArgument::TypeArgument(NodePtr type, NodeListPtr typeArguments)
:arguments(typeArguments), node(type)
{
}
	
TypeArgument::~TypeArgument()
{
}

NodeListPtr TypeArgument::GetArguments()
{
	return arguments;
}

NodePtr TypeArgument::GetNode()
{
	return node;
}

END_NAMESPACE2

#endif
