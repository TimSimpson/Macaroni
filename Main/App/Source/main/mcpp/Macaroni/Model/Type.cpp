/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MACARONI_MODEL_TYPE_CPP
#define MACARONI_MODEL_TYPE_CPP

#include "../ME.h"
#include "Type.h"
#include <boost/foreach.hpp>
#include "TypeArgument.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

Type::Type(NodePtr type, TypeModifiers modifiers)
:	type(type), typeArguments(new TypeArgumentList()), modifiers(modifiers)
{
}

Type::Type(NodePtr type, TypeModifiers modifiers, TypeArgumentListPtr typeArguments)
:	type(type),
	typeArguments(typeArguments),
	modifiers(modifiers)
{
}
	
Type::~Type()
{
}

bool Type::operator== (const Type & other) const
{	
	return this->modifiers == other.modifiers
		   && type == other.type
		   && TypeArgument::ListIsEqual(this->GetTypeArguments(), 
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
