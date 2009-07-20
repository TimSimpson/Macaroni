#ifndef MACARONI_MODEL_CPP_CLASS_CPP
#define MACARONI_MODEL_CPP_CLASS_CPP

#include "Class.h"
#include "../../Exception.h"
#include "../Node.h"
#include <memory>
#include <sstream>

using class Macaroni::Model::Node;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Class::Class(Node * parent, ReasonPtr reason)
:Scope(parent, "Namespace", reason)
{
}

Class::~Class()
{
	
}

bool Class::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Scope *>(other) != nullptr;
}

ClassPtr Class::Create(NodePtr parent, ReasonPtr reason)
{
	return ClassPtr(new Class(parent.get(), reason));
}

const char * Class::GetTypeName() const
{
	return "Class";
}

void intrusive_ptr_add_ref(Class * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Class * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}


END_NAMESPACE

#endif
