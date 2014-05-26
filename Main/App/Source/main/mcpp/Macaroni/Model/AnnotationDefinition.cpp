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
#ifndef MACARONI_MODEL_ATTRIBUTEDEFINITION_CPP
#define MACARONI_MODEL_ATTRIBUTEDEFINITION_CPP

#include "AnnotationDefinition.h"
#include <Macaroni/Exception.h>
#include "Node.h"
#include "Member.h"
#include "MemberPtr.h"
#include "ModelInconsistencyException.h"
#include <sstream>

#define TYPE_NAME "AnnotationDefinition"

BEGIN_NAMESPACE2(Macaroni, Model)

void intrusive_ptr_add_ref(AnnotationDefinition * p)
{
	intrusive_ptr_add_ref((Member *)p);
}

void intrusive_ptr_release(AnnotationDefinition * p)
{
	intrusive_ptr_release((Member *)p);
}

AnnotationDefinition::AnnotationDefinition(Node * node, TypeCode type, const ReasonPtr & reasonCreated)
:Member(node, TYPE_NAME, reasonCreated), type(type)
{
}

AnnotationDefinition::~AnnotationDefinition()
{
}

AnnotationDefinitionPtr AnnotationDefinition::Create(NodePtr home, TypeCode type, const ReasonPtr &  reason)
{
	return AnnotationDefinitionPtr(new AnnotationDefinition(home.get(), type, reason));
}

bool AnnotationDefinition::canBeChildOf(const Member * other) const
{
	return true;
}

void AnnotationDefinition::Define(NodePtr node, TypeCode type, const ReasonPtr & reason)
{
	ElementPtr member = node->GetElement();
	if (!!member)
	{
		AnnotationDefinitionPtr existingDef = boost::dynamic_pointer_cast<AnnotationDefinition>(member);
		if (!existingDef)
		{
			std::stringstream ss;
			ss << "Attempt to define previously defined Node "
				<< node->GetFullName()
				<< " as an Annotation.";
			throw ModelInconsistencyException(existingDef->GetReasonCreated(),
				reason,
				ss.str());
		}
		else
		{
			if (existingDef->GetTypeCode() == type)
			{
				// Phew! Old def is the same as this one.
				return;
			}
			std::stringstream ss;
			ss << "Attempt to redefine Annotation "
				<< node->GetFullName()
				<< " with value type of "
				<< existingDef->GetAnnotationTypeName()
				<< " as value type "
				<< getAnnotationTypeName(type);
			throw ModelInconsistencyException(existingDef->GetReasonCreated(),
				reason,
				ss.str());
		}
	} // end !!member
	Create(node, type, reason);

}

const char * AnnotationDefinition::GetAnnotationTypeName() const
{
	return getAnnotationTypeName(type);
}

const char * AnnotationDefinition::getAnnotationTypeName(TypeCode type)
{
	switch(type)
	{
	case Type_Bool:
		return "bool";
	case Type_Node:
		return "node";
	case Type_Number:
		return "number";
	case Type_String:
		return "string";
	case Type_Table:
		return "table";
	default:
		return "unknown";
	};
}

const char * AnnotationDefinition::GetTypeName() const
{
	return TYPE_NAME;
}

END_NAMESPACE2

#endif

