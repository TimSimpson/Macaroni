#ifndef MACARONI_MODEL_CPP_SCOPEMEMBER_CPP
#define MACARONI_MODEL_CPP_SCOPEMEMBER_CPP

#include "ScopeMember.h"
#include "../Node.h"
#include <sstream>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

ScopeMember::ScopeMember(Node * node, ReasonPtr reason)
:Member(node, reason)
{
}

ScopeMember::~ScopeMember()
{
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
