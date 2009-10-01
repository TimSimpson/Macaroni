#ifndef MACARONI_MODEL_TYPEARGUMENT_CPP
#define MACARONI_MODEL_TYPEARGUMENT_CPP

#include "TypeArguments.h"

BEGIN_NAMESPACE2(Macaroni, Model)

TypeArgument::TypeArgument(NodePtr type)
:arguments(new NodeList()), type(type)
{
}

TypeArgument::TypeArgument(NodePtr type, NodeListPtr typeArguments)
:arguments(typeArguments), type(type)
{
}
	
TypeArgument::~TypeArgument()
{
}
	
NodePtr TypeArgument::GetType()
{
	return type;
}

NodeListPtr TypeArgument::GetTypeArguments()
{
	return arguments;
}

END_NAMESPACE2

#endif
