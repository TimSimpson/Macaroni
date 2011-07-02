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
