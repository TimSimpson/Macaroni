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

Member::Member(Node * node, const char * typeName, const ReasonPtr reasonCreated)
:node(nullptr), reasonCreated(reasonCreated)
{
	node->setMember(this, typeName, reasonCreated);

	MACARONI_ASSERT(node != nullptr, "Node must be initialized by base class.");
	MACARONI_ASSERT(reasonCreated != false, "Reason created must be set by base class.");
}

void Member::assertChildrenAreLegal() 
{	
	for(unsigned int i = 0; i < node->GetChildCount(); i ++)
	{
		NodePtr child = node->GetChild(i);
		Member * childMember = child->getMember();
		if (childMember != nullptr)
		{
			if (!childMember->canBeChildOf(this))
			{
				std::stringstream ss;
				ss << "Error creating member " << GetTypeName() << " at "
					<< node->GetFullName() << ": child "
					<< child->GetFullName() << " cannot be nested under this type.";
				throw Macaroni::Exception(ss.str().c_str());
			}
		}
	}
}

void Member::assertValidConstruction()
{
	// The Constructor of a subclass in C++ can't chain to a virtual method 
	// because the vtable isn't ready to roll at that point, so unfortunately
	// there's no good way I can think of to force check that subclasses adhere
	// to these behaviors.	
	assertChildrenAreLegal();
}

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
		if (!!kid->GetMember() && kid->GetMember()->DoesDefinitionReference(node))
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
		MemberPtr m = itr->GetMember();
		NamespacePtr ns = boost::dynamic_pointer_cast<Cpp::Namespace>(m);
		if (!!ns)
		{
			return ns;
		}
		itr = itr->GetNode();
	}
	throw Macaroni::Exception("Could not find Namespace in this Member.");
}

NodePtr Member::GetNode() const
{
	return NodePtr(node);
}

Node * Member::getNode() const
{
	return node;
}

ReasonPtr Member::GetReasonCreated() const
{
	return reasonCreated;
}

void intrusive_ptr_add_ref(Member * p)
{
	intrusive_ptr_add_ref(p->node);
}

void intrusive_ptr_release(Member * p)
{
	intrusive_ptr_release(p->node);
}

void Member::setReasonCreation(const ReasonPtr reason)
{
	this->reasonCreated = reason;
}

END_NAMESPACE2

#endif

