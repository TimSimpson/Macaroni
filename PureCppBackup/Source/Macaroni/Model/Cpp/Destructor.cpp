#ifndef MACARONI_MODEL_CPP_DESTRUCTOR_CPP
#define MACARONI_MODEL_CPP_DESTRUCTOR_CPP

#include "Class.h"
#include "Destructor.h"
#include "Primitive.h"
#include "../../Exception.h"
#include "../MemberVisitor.h"
#include "../ModelInconsistencyException.h"
#include "Namespace.h"
#include "../Node.h"
#include "../Reason.h"
#include "Scope.h"
#include "ScopeMember.h"
#include "Variable.h"
#include <memory>
#include <sstream>

using class Macaroni::Model::ModelInconsistencyException;
using class Macaroni::Model::Node;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

namespace
{
	inline TypePtr voidType()
	{
		TypeModifiers modifiers;
		modifiers.Const = 
			modifiers.ConstPointer = 
			modifiers.Pointer = 
			modifiers.Reference = 
			false;
		return TypePtr(new Type(NodePtr(), modifiers));		
	}
};

Destructor::Destructor(Node * home, Model::ReasonPtr reason, bool isInline, Access access)
:Function(home, "Destructor", reason, isInline, access, true, voidType(), false)
{
}

Destructor::~Destructor()
{
}

bool Destructor::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Class *>(other) != nullptr;
}

DestructorPtr Destructor::Create(NodePtr host, bool isInline, Access access, Model::ReasonPtr reason)
{
	if (!host->GetMember())
	{
		return DestructorPtr(new Destructor(host.get(), reason, isInline, access));
	}
	Member * member = host->GetMember().get();
	Destructor * existingFunc = dynamic_cast<Destructor *>(member);
	if (existingFunc == nullptr)
	{
		// Will throw an error message.
		return DestructorPtr(new Destructor(host.get(), reason, isInline, access));
	}

	// Re-use the previously set variable.
	return DestructorPtr(boost::dynamic_pointer_cast<Destructor>(host->GetMember()));
}

const char * Destructor::GetTypeName() const
{
	return "Destructor";
}

void intrusive_ptr_add_ref(Destructor * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Destructor * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

void Destructor::Visit(MemberVisitor * visitor) const
{
	visitor->VisitDestructor(*this);
}
	
END_NAMESPACE

#endif