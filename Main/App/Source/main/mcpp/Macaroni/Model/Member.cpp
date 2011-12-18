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
#ifndef MACARONI_MODEL_MEMBER_CPP
#define MACARONI_MODEL_MEMBER_CPP

#include "Member.h"
#include "../Exception.h"
#include "Cpp/Namespace.h"
#include "Node.h"
#include "Reason.h"
#include <sstream>

using Macaroni::Model::Cpp::Namespace;
using Macaroni::Model::Cpp::NamespacePtr;

BEGIN_NAMESPACE2(Macaroni, Model)

//TODO: Get rid of typeName, it isn't used.
Member::Member(Node * node, const char * typeName, const ReasonPtr reasonCreated)
:Element(node, reasonCreated)
{
}

//void Member::assertChildrenAreLegal()
//{
//	for(unsigned int i = 0; i < getNode()->GetChildCount(); i ++)
//	{
//		NodePtr child = getNode()->GetChild(i);
//		MemberPtr childMember = child->getElement<MemberPtr>();
//		if (!!childMember)
//		{
//			if (!childMember->canBeChildOf(this))
//			{
//				std::stringstream ss;
//				ss << "Error creating member " << GetTypeName() << " at "
//					<< node->GetFullName() << ": child "
//					<< child->GetFullName() << " cannot be nested under this type.";
//				throw Macaroni::Exception(ss.str().c_str());
//			}
//		}
//	}
//}
//
//void Member::assertValidConstruction()
//{
//	// The Constructor of a subclass in C++ can't chain to a virtual method
//	// because the vtable isn't ready to roll at that point, so unfortunately
//	// there's no good way I can think of to force check that subclasses adhere
//	// to these behaviors.
//	assertChildrenAreLegal();
//}

bool Member::CanBeChildOf(const Member * otherMember) const
{
	if (otherMember == nullptr)
	{
		return false;
	}
	return canBeChildOf(otherMember);
}

bool Member::DoesDefinitionReference(NodePtr node) const
{
	NodePtr me = this->GetNode();
	for(unsigned int i = 0; i < me->GetChildCount(); i ++)
	{
		NodePtr kid = me->GetChild(i);
		MemberPtr member = kid->GetElement<MemberPtr>();
		if (!!member && member->DoesDefinitionReference(node))
		{
			return true;
		}
	}
	return false;
}

NamespacePtr Member::FindClosestParentNamespace() const
{
	NodePtr itr = this->GetNode();
	while(!!itr)
	{
		NamespacePtr ns = itr->GetElement<Cpp::NamespacePtr>();
		if (!!ns)
		{
			return ns;
		}
		itr = itr->GetNode();
	}
	throw Macaroni::Exception("Could not find Namespace in this Member.");
}

//NodePtr Member::GetNode() const
//{
//	return NodePtr(node);
//}
//
//Node * Member::getNode() const
//{
//	return node;
//}
//
//ReasonPtr Member::GetReasonCreated() const
//{
//	return reasonCreated;
//}

void intrusive_ptr_add_ref(Member * p)
{
	intrusive_ptr_add_ref((Element *)p);
}

void intrusive_ptr_release(Member * p)
{
	intrusive_ptr_release((Element *)p);
}

/*
void Member::setReasonCreation(const ReasonPtr reason)
{
	this->reasonCreated = reason;
}*/

END_NAMESPACE2

#endif

