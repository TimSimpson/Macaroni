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
	inline TypeInfo voidTypeInfo()
	{
		//ContextPtr context = home->GetContext();

		TypeInfo typeInfo;
		typeInfo.IsConst = false;
		typeInfo.IsConstPointer = false;
		typeInfo.IsPointer = false;
		typeInfo.IsReference = false;
		typeInfo.Node = NodePtr();//CppContext::GetPrimitives(context)->Find("void");
		return typeInfo;
	}
};

Destructor::Destructor(Node * home, Model::ReasonPtr reason)
:Function(home, "Destructor", reason, voidTypeInfo(), false)
{
}

Destructor::~Destructor()
{
}

bool Destructor::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Class *>(other) != nullptr;
}

DestructorPtr Destructor::Create(NodePtr host, Model::ReasonPtr reason)
{
	if (!host->GetMember())
	{
		return DestructorPtr(new Destructor(host.get(), reason));
	}
	Member * member = host->GetMember().get();
	Destructor * existingFunc = dynamic_cast<Destructor *>(member);
	if (existingFunc == nullptr)
	{
		// Will throw an error message.
		return DestructorPtr(new Destructor(host.get(), reason));
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
