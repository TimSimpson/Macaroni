#ifndef MACARONI_MODEL_CPP_CONSTRUCTOR_CPP
#define MACARONI_MODEL_CPP_CONSTRUCTOR_CPP

#include "Class.h"
#include "Constructor.h"
#include "Primitive.h"
#include "../../Exception.h"
#include "../MemberVisitor.h"
#include "../ModelInconsistencyException.h"
#include "Namespace.h"
#include "../Node.h"
#include <Macaroni/Model/Reason.h>
#include "Scope.h"
#include "ScopeMember.h"
#include "Variable.h"
#include <memory>
#include <sstream>

using class Macaroni::Model::ModelInconsistencyException;
using class Macaroni::Model::Node;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)


Constructor::Constructor(Node * home, Model::ReasonPtr reason)
:Function(home, "Constructor", reason)
{
}

Constructor::~Constructor()
{
}
//
//void Constructor::AddAssignment(const VariableAssignment & assignment)
//{
//	assignments.push_back(assignment);
//}

bool Constructor::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Class *>(other) != nullptr;
}

ConstructorPtr Constructor::Create(NodePtr host, Model::ReasonPtr reason)
{
	if (!host->GetMember())
	{
		return ConstructorPtr(new Constructor(host.get(), reason));
	}
	Member * member = host->GetMember().get();
	Constructor * existingFunc = dynamic_cast<Constructor *>(member);
	if (existingFunc == nullptr)
	{
		// Will throw an error message.
		return ConstructorPtr(new Constructor(host.get(), reason));
	}

	// Re-use the previously set variable.
	return ConstructorPtr(boost::dynamic_pointer_cast<Constructor>(host->GetMember()));
}
//
//const VariableAssignment & Constructor::GetAssignment(int index) const
//{
//	return assignments[index];
//}
//
//int Constructor::GetAssignmentCount() const
//{
//	return assignments.size();
//}

const char * Constructor::GetTypeName() const
{
	return "Constructor";
}

void intrusive_ptr_add_ref(Constructor * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Constructor * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

void Constructor::Visit(MemberVisitor * visitor) const
{
	visitor->VisitConstructor(*this);
}
	
END_NAMESPACE

#endif
