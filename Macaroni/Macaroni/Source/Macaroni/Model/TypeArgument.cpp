#ifndef MACARONI_MODEL_TYPEARGUMENT_CPP
#define MACARONI_MODEL_TYPEARGUMENT_CPP

#include "TypeArgument.h"
#include <boost/foreach.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

TypeArgument::TypeArgument(NodePtr type)
:arguments(new TypeList()), node(type)
{
}

TypeArgument::TypeArgument(NodePtr type, TypeListPtr typeArguments)
:arguments(typeArguments), node(type)
{
}
	
TypeArgument::~TypeArgument()
{
}

bool TypeArgument::operator==(const TypeArgument & other) const
{
	if (this->node->GetFullName() != other.GetNode()->GetFullName())
	{
		return false;
	}
	return Type::ListIsEqual(this->GetArguments(), other.GetArguments());
}

TypeListPtr TypeArgument::GetArguments() const
{
	return arguments;
}

NodePtr TypeArgument::GetNode() const
{
	return node;
}

bool TypeArgument::ListContains(TypeArgumentListPtr list, TypeArgumentPtr target)
{
	TypeArgumentList & listRef = *(list.get());
	TypeArgument & targetRef = *(target.get());
	BOOST_FOREACH(TypeArgumentPtr element, listRef)
	{
		TypeArgument & elementRef = *(element.get());
		if (elementRef.operator ==(targetRef))
		{
			return true;
		}
	}
	return false;
}

bool TypeArgument::ListIsEqual(TypeArgumentListPtr list1, TypeArgumentListPtr list2)
{
	TypeArgumentList & list1Ref = *(list1.get());
	BOOST_FOREACH(TypeArgumentPtr element, list1Ref)
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
