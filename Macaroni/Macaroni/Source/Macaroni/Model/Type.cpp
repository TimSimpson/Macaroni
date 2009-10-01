#ifndef MACARONI_MODEL_TYPE_CPP
#define MACARONI_MODEL_TYPE_CPP

#include "../ME.h"
#include "Type.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Type::Type(NodePtr type)
: type(type), typeArguments(new TypeArgumentList())
{
}

Type::Type(NodePtr type, TypeArgumentListPtr typeArguments)
:type(type),
 typeArguments(typeArguments)
{
}
	
Type::~Type()
{
}
	
NodePtr Type::GetNode()
{
	return type;
}

TypeArgumentListPtr Type::GetTypeArguments()
{
	return typeArguments;
}
	
END_NAMESPACE2

#endif
