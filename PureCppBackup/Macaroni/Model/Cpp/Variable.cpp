#ifndef MACARONI_MODEL_CPP_VARIABLE_CPP
#define MACARONI_MODEL_CPP_VARIABLE_CPP

#include "Primitive.h"
#include "../../Exception.h"
#include "../MemberVisitor.h"
#include "../ModelInconsistencyException.h"
#include "Namespace.h"
#include "../Node.h"
#include "Scope.h"
#include "ScopeMember.h"
#include "Variable.h"
#include <memory>
#include <sstream>

using class Macaroni::Model::ModelInconsistencyException;
using class Macaroni::Model::Node;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Variable::Variable(Node * parent, ReasonPtr reason, Access access, bool isStatic, const TypePtr type, std::string initializer)
:ScopeMember(parent, "Variable", reason, access, isStatic),
 initializer(initializer),
 type(type)
{
}

Variable::~Variable()
{
	
}


bool Variable::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Scope *>(other) != nullptr;
}

VariablePtr Variable::Create(NodePtr host, Access access, bool isStatic, const TypePtr type, std::string initializer, ReasonPtr reason)
{
	if (!host->GetMember())
	{
		//return Variable::Create(host, access, type, reason);
		return VariablePtr(new Variable(host.get(), reason, access, isStatic, type, initializer));
	}
	Member * member = host->GetMember().get();
	Variable * existingVar = dynamic_cast<Variable *>(member);
	if (existingVar == nullptr)
	{
		// Will throw an error message.
		//return Variable::Create(host, access, type, reason);
		return VariablePtr(new Variable(host.get(), reason, access, isStatic, type, initializer));
	}

	if (existingVar != nullptr && !(existingVar->type->operator==(*type.get())))
	{
		std::stringstream ss;
		ss << "Variable was already defined with conflicting type information. ";	
		if (existingVar->type->IsConst() && !type->IsConst())
		{
			ss << "Previous definition was const.";
		}
		if (existingVar->type->IsConstPointer() && !type->IsConstPointer())
		{
			ss << "Previous definition was const pointer.";
		}
		if (existingVar->type->IsPointer() && !type->IsPointer())
		{
			ss << "Previous definition was pointer.";
		}
		if (existingVar->type->IsReference() && !type->IsReference())
		{
			ss << "Previous definition was reference.";
		}
		existingVar->type->DescribeDifferences(type, ss);
		throw ModelInconsistencyException(member->GetReasonCreated(),
											  reason,
											  ss.str());	
	}
	// Re-use the previously set variable.
	return VariablePtr(boost::dynamic_pointer_cast<Variable>(host->GetMember()));
}

bool Variable::DoesDefinitionReference(NodePtr node) const
{
	return type->GetNode() == node ? true 
		: this->Member::DoesDefinitionReference(node);
}

const char * Variable::GetTypeName() const
{
	return "Variable";
}

////Model::NodePtr Variable::GetTypeNode() const
////{
////	return typeInfo.Node;
////}

void intrusive_ptr_add_ref(Variable * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Variable * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

void Variable::Visit(MemberVisitor * visitor) const
{
	visitor->VisitVariable(*this);
}


END_NAMESPACE

#endif
