#ifndef MACARONI_MODEL_TYPE_CPP
#define MACARONI_MODEL_TYPE_CPP

#include "../ME.h"
#include "Type.h"
#include <sstream>

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
	
void Type::DescribeDifferences(const TypePtr other, std::stringstream & stream) const
{
	if (this->IsConst() && !other->IsConst())
	{
		stream << "Previous definition was const.";
	}
	if (this->IsConstPointer() && !other->IsConstPointer())
	{
		stream << "Previous definition was const pointer.";
	}
	if (this->IsPointer() && !other->IsPointer())
	{
		stream << "Previous definition was pointer.";
	}
	if (this->IsReference() && !other->IsReference())
	{
		stream << "Previous definition was reference.";
	}
}


NodePtr Type::GetNode() const
{
	return type;
}

TypeArgumentListPtr Type::GetTypeArguments()
{
	return typeArguments;
}
	
END_NAMESPACE2

#endif
