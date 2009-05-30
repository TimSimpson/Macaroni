#ifndef MACARONI_MODEL_SCOPEMEMBER_CPP
#define MACARONI_MODEL_SCOPEMEMBER_CPP

#include "ScopeMember.h"
#include "Context.h"
#include "Scope.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

ScopeMember::ScopeMember(Scope * scope, const std::string & name)
:name(name), scope(scope)
{
}

ScopeMember::~ScopeMember()
{
}

const std::string & ScopeMember::GetName() const
{
	return name;
}

std::string ScopeMember::GetFullName() const
{
	std::stringstream ss;
	if (this->scope != nullptr && !(this->scope->IsRoot()))
	{
		ss << this->scope->GetFullName();
		ss << "::";		
	}	// do not include root node.
	ss << this->name;
	return ss.str();
}

ScopePtr ScopeMember::GetScope() const
{
	return ScopePtr(scope);
}

Scope * ScopeMember::getScope() const
{
	return scope;
}

/**TO-DO: In retrospect, a cleaner solution would give ScopeMember a reference
 * to the Context, so casting and checking like this would be unnecessary. */
void intrusive_ptr_add_ref(ScopeMember * p)
{
	if (p->scope == nullptr)
	{
		Scope * pAsScope = dynamic_cast<Scope *>(p);
		MACARONI_ASSERT(pAsScope != nullptr, 
			"Error adding smart pointer- ScopeMember has nullptr for Scope.");
		intrusive_ptr_add_ref(pAsScope);
	}
	else
	{
		intrusive_ptr_add_ref(p->scope);
	}
}

void intrusive_ptr_release(ScopeMember * p)
{
	if (p->scope == nullptr)
	{
		Scope * pAsScope = dynamic_cast<Scope *>(p);
		MACARONI_ASSERT(pAsScope != nullptr,
			"Error releasing smart pointer- ScopeMember has nullptr for Scope.");		
		intrusive_ptr_release(pAsScope);		
	}
	else
	{
		intrusive_ptr_release(p->scope);
	}
}


END_NAMESPACE2

#endif
