#ifndef MACARONI_MODEL_CPP_PRIMITIVE_CPP
#define MACARONI_MODEL_CPP_PRIMITIVE_CPP

#include "Primitive.h"
#include "../../Exception.h"
#include "Namespace.h"
#include "../Node.h"
#include "Scope.h"
#include "ScopeMember.h"
#include <memory>
#include <sstream>

using class Macaroni::Model::Node;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Primitive::Primitive(Node * parent, ReasonPtr reason)
:ScopeMember(parent, "Primitive", reason)
{
}

Primitive::~Primitive()
{
	
}

/*void Primitive::AddCppPrimitives(ContextPtr context)
{
	NodePtr root = context->GetRoot();
	NodePtr integer = root->FindNode("integer");
	if (!integer)
	{
		integer = root->FindOrCreateNode("int");
		Primitive::Create(integer, Reason::Create(
	}
}*/

bool Primitive::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Scope *>(other) != nullptr;
}

PrimitivePtr Primitive::Create(NodePtr parent, ReasonPtr reason)
{
	return PrimitivePtr(new Primitive(parent.get(), reason));
}

const char * Primitive::GetTypeName() const
{
	return "Primitive";
}


void intrusive_ptr_add_ref(Primitive * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Primitive * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

void Primitive::Visit(MemberVisitor * visitor) const
{
	// Do nothing.
}

END_NAMESPACE

#endif
