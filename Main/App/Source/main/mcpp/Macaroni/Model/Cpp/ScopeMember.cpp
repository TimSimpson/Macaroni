#ifndef MACARONI_MODEL_CPP_SCOPEMEMBER_CPP
#define MACARONI_MODEL_CPP_SCOPEMEMBER_CPP

#include "ScopeMember.h"
#include "../Node.h"
#include <sstream>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

ScopeMember::ScopeMember(Node * node, const char * typeName, ReasonPtr reason)
:Member(node, typeName, reason), access(Access::Private()), isStatic(false)
{
}

ScopeMember::ScopeMember(Node * node, const char * typeName, ReasonPtr reason, Access access, bool isStatic)
:Member(node, typeName, reason), access(access), isStatic(isStatic)
{
}

ScopeMember::~ScopeMember()
{
}

Access ScopeMember::GetAccess() const
{
	return access;
}

const std::string & ScopeMember::GetName() const
{
	return getNode()->GetName();
}

std::string ScopeMember::GetFullName() const
{
	return getNode()->GetFullName();
}

void intrusive_ptr_add_ref(ScopeMember * p)
{
	intrusive_ptr_add_ref(p->getNode());
}

void intrusive_ptr_release(ScopeMember * p)
{
	intrusive_ptr_release(p->getNode());
}


END_NAMESPACE

#endif
