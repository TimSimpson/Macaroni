#ifndef MACARONI_MODEL_CPP_TYPEDEF_CPP
#define MACARONI_MODEL_CPP_TYPEDEF_CPP

#include "Typedef.h"
#include "../../Environment/DebugLog.h"
#include "../Member.h"
#include "../Node.h"
#include "ScopeMember.h"
#include "../Type.h"
#include "Typedef.h"

using Macaroni::Model::Member;
using Macaroni::Model::Node;
using Macaroni::Model::Reason;
using Macaroni::Model::ReasonPtr;
using Macaroni::Model::Type;
using Macaroni::Model::TypePtr;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Typedef::Typedef(Node * node, ReasonPtr reason, TypePtr type)
:ScopeMember(node, "Typedef", reason), type(type)
{
}

Typedef::~Typedef()
{
}

bool Typedef::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Typedef *>(other) != nullptr;
}


TypedefPtr Typedef::Create(NodePtr home, ReasonPtr reason, TypePtr type)
{
	return TypedefPtr(new Typedef(home.get(), reason, type));
}

TypePtr Typedef::GetType() const
{
	return type;
}

const char * Typedef::GetTypeName() const
{
	return "Typedef";
}

void intrusive_ptr_add_ref(Typedef * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Typedef * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

void Typedef::Visit(MemberVisitor * visitor) const
{
	// Ignore! I HATE THIS IDEA! 
}

END_NAMESPACE

#endif
