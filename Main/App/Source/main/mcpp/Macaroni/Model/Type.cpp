#ifndef MACARONI_MODEL_TYPE_CPP
#define MACARONI_MODEL_TYPE_CPP

#include "../ME.h"
#include "Type.h"
#include <boost/foreach.hpp>
#include "TypeArgument.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

Type::Type(NodePtr type, TypeModifiers modifiers)
:	isConst(modifiers.Const()), isConstPointer(modifiers.ConstPointer()),
	isPointer(modifiers.Pointer()), isReference(modifiers.Reference()),
	type(type), typeArguments(new TypeArgumentList())
{
}

Type::Type(NodePtr type, TypeModifiers modifiers, TypeArgumentListPtr typeArguments)
:	isConst(modifiers.Const()), isConstPointer(modifiers.ConstPointer()),
	isPointer(modifiers.Pointer()), isReference(modifiers.Reference()),
	type(type),
	typeArguments(typeArguments)
{
}
	
Type::~Type()
{
}

bool Type::operator== (const Type & other) const
{	
	return this->IsConst() == other.IsConst()
		&& this->IsConstPointer() == other.IsConstPointer()
		&& this->IsPointer() == other.IsPointer()
		&& this->IsReference() == other.IsReference()
		&& type == other.type
		&& TypeArgument::ListIsEqual(
			this->GetTypeArguments(), 
			other.GetTypeArguments());
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
	if (this->GetNode()->GetFullName() != other->GetNode()->GetFullName())
	{
		stream << "Previous definition was "
			<< this->GetNode()->GetFullName()
			<< " but the other is "
			<< other->GetNode()->GetFullName()
			<< ".";
	}
	stream << "(BTW, type arguments not yet implemented by this function)";
}


NodePtr Type::GetNode() const
{
	return type;
}

TypeArgumentListPtr Type::GetTypeArguments() const
{
	return typeArguments;
}
	
bool Type::ListContains(TypeListPtr list, TypePtr target) 
{
	Type & targetRef = *target.get();	
	BOOST_FOREACH(TypePtr e, *(list.get()))
	{
		Type & a = *e.get();
		if (a.operator ==(targetRef))
		{
			return true;
		}
	}
	return false;
}

bool Type::ListIsEqual(TypeListPtr list1, TypeListPtr list2)
{
	TypeList & list1Ref = *(list1.get());
	BOOST_FOREACH(TypePtr element, list1Ref)
	{
		if (!ListContains(list2, element)) 
		{
			return false;
		}
	}
	return true;
}

END_NAMESPACE2

#endif
