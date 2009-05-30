#ifndef MACARONI_MODEL_PRIMITIVE_CPP
#define MACARONI_MODEL_PRIMITIVE_CPP

#include "Primitive.h"
#include "Scope.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Primitive::Primitive(Scope * scope, std::string & name)
:ScopeMember(scope, name)
{
}
	
Primitive::~Primitive()
{
}

const std::string & Primitive::GetTypeName() const 
{ 
	return GetName(); 
}

ScopePtr Primitive::GetTypeScope() const 
{ 
	return GetScope(); 
}	


void intrusive_ptr_add_ref(Primitive * p)
{
	return intrusive_ptr_add_ref(p->getScope());
}

void intrusive_ptr_release(Primitive * p)
{
	return intrusive_ptr_release(p->getScope());
}

END_NAMESPACE2

#endif

