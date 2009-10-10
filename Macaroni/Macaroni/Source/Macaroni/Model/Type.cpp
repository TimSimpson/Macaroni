#ifndef MACARONI_MODEL_TYPE_CPP
#define MACARONI_MODEL_TYPE_CPP

#include "../ME.h"
#include "Type.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Type::Type(NodePtr type, TypeModifiers modifiers)
:	isConst(modifiers.Const), isConstPointer(modifiers.ConstPointer),
	isPointer(modifiers.Pointer), isReference(modifiers.Reference),
	type(type), typeArguments(new TypeArgumentList())
{
}

Type::Type(NodePtr type, TypeModifiers modifiers, TypeArgumentListPtr typeArguments)
:	isConst(modifiers.Const), isConstPointer(modifiers.ConstPointer),
	isPointer(modifiers.Pointer), isReference(modifiers.Reference),
	type(type),
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
