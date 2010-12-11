#ifndef MACARONI_MODEL_CPP_SCOPE_CPP
#define MACARONI_MODEL_CPP_SCOPE_CPP

#include "Scope.h"
#include "../Library.h"
#include "../Node.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

void intrusive_ptr_add_ref(Scope * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Scope * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

Scope::Scope(Library * library, Node * node, const char * typeName, ReasonPtr reason)
:ScopeMember(node, typeName, reason),
 library(library)
{
}

LibraryPtr Scope::GetLibrary() const
{
	return LibraryPtr(library);
}

size_t Scope::GetMemberCount() const
{
	return getNode()->GetChildCount();
}

NodePtr Scope::GetMember(int index) const
{
	return getNode()->GetChild(index);
}

END_NAMESPACE

#endif
