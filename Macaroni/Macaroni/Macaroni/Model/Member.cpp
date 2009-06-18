#ifndef MACARONI_MODEL_MEMBER_CPP
#define MACARONI_MODEL_MEMBER_CPP

#include "Member.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Member::Member()
:node(nullptr)
{
	construct();
	MACARONI_ASSERT(node != nullptr, "Node must be initialized by base class.");
	MACARONI_ASSERT(reasonCreated != false, "Reason created must be set by base class.");
	//TO-DO: Consider moving this to it's own method.
	for(int i = 0; i < node->GetChildCount(); i ++)
	{
		NodePtr child = node->GetChild(i);
		Member * childMember->getMember();
		if (childMember != nullptr)
		{
			if (!childMember->canBeChildOf(this))
			{
				std::stringstream ss;
				ss << "Error creating member " << GetTypeName() << " at "
					<< node->GetFullName() << ": child "
					<< child->GetFullName() << " cannot be nested under this type.";
				throw new Macaroni::Exception(ss);
			}
		}
	}
}

bool Member::CanBeChildOf(const Member * otherMember) const
{
	if (otherMember == nullptr)
	{
		return false;
	}
	return canBeChildOf(otherMember);
}

void Member::setReasonCreation(const ReasonPtr reason)
{
	this->reasonCreated = reason;
}

void Member::setNode(Node * node)
{
	node->setMember(this); // Node will set Member's node pointer to itself.
}

END_NAMESPACE2

#endif

