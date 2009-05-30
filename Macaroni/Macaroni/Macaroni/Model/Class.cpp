#ifndef MACARONI_CLASS_CPP
#define MACARONI_CLASS_CPP

#include "../ME.h"
#include "Class.h"
#include "Namespace.h"
#include <vector>
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

BaseClass::BaseClass(Access access, ClassPtr baseClass)
:access(access), baseClass(baseClass)
{
}

Class::Class(Scope * scope, const std::string & simpleName)
:Scope(scope, simpleName)
{
	MACARONI_ASSERT(Scope::IsSimpleName(simpleName), 
				    "Class constructor cannot take complex name as argument.");
}

void Class::AddBaseClass(BaseClassPtr ptr)
{
	baseClasses.push_back(ptr);
}

ClassPtr Class::FindOrCreate(NamespacePtr startingNs, std::string & complexName)
{
	return startingNs->FindOrCreateClass(complexName);
	//ScopePtr finalScope;
	//std::string simpleName;
	//Scope::ParseComplexName(startingNs, complexName, finalScope, simpleName);
	////Namespace::FindOrCreate(startingNs, complexName, finalNs, simpleName);
	//ClassPtr newInstance(new Class(finalScope.get(), simpleName));
	//return finalScope->InsertClass(newInstance);
}

void intrusive_ptr_add_ref(Class * p)
{
	intrusive_ptr_add_ref(static_cast<Scope *>(p));
}

void intrusive_ptr_release(Class * p)
{
	intrusive_ptr_release(static_cast<Scope *>(p));
}

void Class::SetBody(std::string & value)
{
	this->body = value;
}

END_NAMESPACE2


#endif
