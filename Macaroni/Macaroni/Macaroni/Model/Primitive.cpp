#ifndef MACARONI_MODEL_PRIMITIVE_CPP
#define MACARONI_MODEL_PRIMITIVE_CPP

#include "Primitive.h"
#include "Node.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Primitive::Primitive(Node * scope, std::string & name)
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

NodePtr Primitive::GetTypeNode() const 
{ 
	return GetNode(); 
}	


void intrusive_ptr_add_ref(Primitive * p)
{
	return intrusive_ptr_add_ref(p->getNode());
}

void intrusive_ptr_release(Primitive * p)
{
	return intrusive_ptr_release(p->getNode());
}

END_NAMESPACE2

#endif

