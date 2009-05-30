#ifndef MACARONI_MODEL_SCOPEMEMBER_CPP
#define MACARONI_MODEL_SCOPEMEMBER_CPP

#include "ScopeMember.h"
#include "Context.h"
#include "Node.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

ScopeMember::ScopeMember(Node * node)
:node(node)
{
}

ScopeMember::~ScopeMember()
{
}

const std::string & ScopeMember::GetName() const
{
	return node->GetName();
}

std::string ScopeMember::GetFullName() const
{
	return node->GetFullName();
}

NodePtr ScopeMember::GetNode() const
{
	return NodePtr(node);
}

Node * ScopeMember::getNode() const
{
	return scope;
}

/**TO-DO: In retrospect, a cleaner solution would give ScopeMember a reference
 * to the Context, so casting and checking like this would be unnecessary. */
void intrusive_ptr_add_ref(ScopeMember * p)
{
	intrusive_ptr_add_red(node);/*
	if (p->scope == nullptr)
	{
		Node * pAsNode = dynamic_cast<Node *>(p);
		MACARONI_ASSERT(pAsNode != nullptr, 
			"Error adding smart pointer- ScopeMember has nullptr for Node.");
		intrusive_ptr_add_ref(pAsNode);
	}
	else
	{
		intrusive_ptr_add_ref(p->scope);
	}*/
}

void intrusive_ptr_release(ScopeMember * p)
{
	intrusive_ptr_release(node);

	//if (p->scope == nullptr)
	//{
	//	Node * pAsNode = dynamic_cast<Node *>(p);
	//	MACARONI_ASSERT(pAsNode != nullptr,
	//		"Error releasing smart pointer- ScopeMember has nullptr for Node.");		
	//	intrusive_ptr_release(pAsNode);		
	//}
	//else
	//{
	//	intrusive_ptr_release(p->scope);
	//}
}


END_NAMESPACE2

#endif
