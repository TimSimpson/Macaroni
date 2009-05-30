#ifndef MACARONI_MODEL_UNKNOWNSCOPE_CPP
#define MACARONI_MODEL_UNKNOWNSCOPE_CPP

#include "UnknownScope.h"
#include "Class.h"
#include "Namespace.h"

BEGIN_NAMESPACE2(Macaroni, Model)

UnknownScope::UnknownScope(Scope * scope, const std::string & name)
:knownScope(nullptr), Scope(scope, name)
{

}

UnknownScope::~UnknownScope()
{
}

ScopeMemberPtr UnknownScope::Find(const std::string & name)
{
	if (knownScope == false)
	{
		return this->Scope::Find(name);
	}
	else
	{
		return knownScope->Find(name);
	}
}

size_t UnknownScope::GetMemberCount() const
{
	if (knownScope == false)
	{
		return this->Scope::GetMemberCount();
	}
	else
	{
		return knownScope->GetMemberCount();
	}
}

ClassPtr UnknownScope::IsClass() 
{
	Class * ptr = dynamic_cast<Class *>(knownScope);
	return ptr == nullptr ? nullptr
		: ClassPtr(ptr);
}

NamespacePtr UnknownScope::IsNamespace() 
{
	Namespace * ptr = dynamic_cast<Namespace *>(knownScope);
	return ptr == nullptr ? nullptr
		: NamespacePtr(ptr);
}

bool UnknownScope::IsRoot() const
{
	if (knownScope == false)
	{
		return this->Scope::IsRoot();
	}
	else
	{
		return knownScope->IsRoot();
	}
}

ScopeMemberPtr UnknownScope::GetMember(int index) const
{
	if (knownScope == false)
	{
		return this->Scope::GetMember(index);
	}
	else
	{
		return knownScope->GetMember(index);
	}
}

void intrusive_ptr_add_ref(UnknownScope * p)
{
	intrusive_ptr_add_ref(static_cast<Scope *>(p));
}

void intrusive_ptr_release(UnknownScope * p)
{
	intrusive_ptr_release(static_cast<Scope *>(p));
}

END_NAMESPACE2

#endif
