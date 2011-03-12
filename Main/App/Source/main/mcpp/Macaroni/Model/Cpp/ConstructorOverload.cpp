#ifndef MACARONI_MODEL_CPP_CONSTRUCTOROVERLOAD_CPP
#define MACARONI_MODEL_CPP_CONSTRUCTOROVERLOAD_CPP

#include "ConstructorOverload.h"
#include "Class.h"
#include "Constructor.h"
#include "Primitive.h"
#include "../../Exception.h"
#include "../MemberVisitor.h"
#include "../ModelInconsistencyException.h"
#include "Namespace.h"
#include "../Node.h"
#include "../Reason.h"
#include "Scope.h"
#include "ScopeMember.h"
#include <Macaroni/Model/Type.h>
#include <Macaroni/Model/TypePtr.h>
#include "Variable.h"
#include <memory>
#include <sstream>

using Macaroni::Model::ModelInconsistencyException;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;
using Macaroni::Model::Type;
using Macaroni::Model::TypeModifiers;
using Macaroni::Model::TypePtr;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

namespace
{
	inline TypePtr voidTypeInfo(Node * home)
	{
		TypeModifiers modifiers;
		modifiers.Const = 
			modifiers.ConstPointer = 
			modifiers.Pointer = 
			modifiers.Reference = 
			false;
		NodePtr node(home);
		return TypePtr(new Type(node, modifiers));		
	}
};

ConstructorOverload::ConstructorOverload(Node * home, Model::ReasonPtr reason, bool isInline, Access access)
:FunctionOverload(home, "ConstructorOverload", reason, isInline, access, true, voidTypeInfo(home), false), 
 assignments()
{
}

ConstructorOverload::~ConstructorOverload()
{
}

void ConstructorOverload::AddAssignment(const VariableAssignment & assignment)
{
	assignments.push_back(assignment);
}

bool ConstructorOverload::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const ConstructorOverload *>(other) != nullptr;
}


ConstructorOverloadPtr ConstructorOverload::Create(ConstructorPtr host, bool isInline, Access access, Model::ReasonPtr reason)
{
	NodePtr node = host->GetNode()->CreateNextInSequence("Overload#");
	ConstructorOverload * co = new ConstructorOverload(node.get(), reason, 
													   isInline, access);
	return ConstructorOverloadPtr(co);
}

const VariableAssignment & ConstructorOverload::GetAssignment(int index) const
{
	return assignments[index];
}

int ConstructorOverload::GetAssignmentCount() const
{
	return assignments.size();
}


const char * ConstructorOverload::GetTypeName() const
{
	return "ConstructorOverload";
}


void intrusive_ptr_add_ref(ConstructorOverload * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(ConstructorOverload * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

void ConstructorOverload::Visit(MemberVisitor * visitor) const
{
	// Do nadda
}

END_NAMESPACE

#endif

