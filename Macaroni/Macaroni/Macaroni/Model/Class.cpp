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

Class::Class(Node * scope, const std::string & simpleName)
:Node(scope, simpleName)
{
	MACARONI_ASSERT(Node::IsSimpleName(simpleName), 
				    "Class constructor cannot take complex name as argument.");
}

void Class::AddBaseClass(BaseClassPtr ptr)
{
	baseClasses.push_back(ptr);
}

ClassPtr Class::FindOrCreate(NamespacePtr startingNs, std::string & complexName)
{
	//TO-DO: Call Node create node method, set Data to class
	MACARONI_ASSERT(false, "TO-DO");
	return ClassPtr(); //return startingNs->FindOrCreateClass(complexName);	
}

void intrusive_ptr_add_ref(Class * p)
{
	intrusive_ptr_add_ref(static_cast<Node *>(p));
}

void intrusive_ptr_release(Class * p)
{
	intrusive_ptr_release(static_cast<Node *>(p));
}

void Class::SetBody(std::string & value)
{
	this->body = value;
}

END_NAMESPACE2


#endif
