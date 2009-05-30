#ifndef MACARONI_MODEL_SCOPE_CPP
#define MACARONI_MODEL_SCOPE_CPP

#include "Scope.h"

BEGIN_NAMESPACE2(Macaroni, Model)

void intrusive_ptr_add_ref(Scope * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Scope * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

Scope::Scope(Node * node)
:ScopeMember(node)
{
}

size_t Scope::GetMemberCount() const
{
	return node->GetChildCount();
}

ScopeMemberPtr Scope::GetMember(int index) const
{
	return node->GetChild(index);
}

END_NAMESPACE2

#endif
