#ifndef MACARONI_MODEL_UNKNOWNSCOPE_CPP
#define MACARONI_MODEL_UNKNOWNSCOPE_CPP

#include "UnknownScope.h"
#include "Class.h"
#include "Namespace.h"

BEGIN_NAMESPACE2(Macaroni, Model)

UnknownScope::UnknownScope(Node * scope, const std::string & name)
:knownNode(nullptr), Node(scope, name)
{

}

UnknownScope::~UnknownScope()
{
}

ScopeMemberPtr UnknownScope::Find(const std::string & name)
{
	if (knownNode == false)
	{
		return this->Node::Find(name);
	}
	else
	{
		return knownNode->Find(name);
	}
}

size_t UnknownScope::GetMemberCount() const
{
	if (knownNode == false)
	{
		return this->Node::GetMemberCount();
	}
	else
	{
		return knownNode->GetMemberCount();
	}
}

ClassPtr UnknownScope::IsClass() 
{
	Class * ptr = dynamic_cast<Class *>(knownNode);
	return ptr == nullptr ? nullptr
		: ClassPtr(ptr);
}

NamespacePtr UnknownScope::IsNamespace() 
{
	Namespace * ptr = dynamic_cast<Namespace *>(knownNode);
	return ptr == nullptr ? nullptr
		: NamespacePtr(ptr);
}

bool UnknownScope::IsRoot() const
{
	if (knownNode == false)
	{
		return this->Node::IsRoot();
	}
	else
	{
		return knownNode->IsRoot();
	}
}

ScopeMemberPtr UnknownScope::GetMember(int index) const
{
	if (knownNode == false)
	{
		return this->Node::GetMember(index);
	}
	else
	{
		return knownNode->GetMember(index);
	}
}

void intrusive_ptr_add_ref(UnknownScope * p)
{
	intrusive_ptr_add_ref(static_cast<Node *>(p));
}

void intrusive_ptr_release(UnknownScope * p)
{
	intrusive_ptr_release(static_cast<Node *>(p));
}

END_NAMESPACE2

#endif
