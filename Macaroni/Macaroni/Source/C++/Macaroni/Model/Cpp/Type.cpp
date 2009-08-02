#ifndef MACARONI_MODEL_CPP_TYPE_CPP
#define MACARONI_MODEL_CPP_TYPE_CPP

#include "Primitive.h"
#include "../../Exception.h"
#include "../ModelInconsistencyException.h"
#include "Namespace.h"
#include "../Node.h"
#include "Scope.h"
#include "ScopeMember.h"
#include "Type.h"
#include <memory>
#include <sstream>

using class Macaroni::Model::ModelInconsistencyException;
using class Macaroni::Model::Node;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Type::Type(Node * parent, ReasonPtr reason, const VariableTypeInfo & info)
:ScopeMember(parent, "Type", reason),
 typeInfo(info)
{
}

Type::~Type()
{
	
}


bool Type::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Scope *>(other) != nullptr;
}

TypePtr Type::Create(NodePtr host, const VariableTypeInfo & info, ReasonPtr reason)
{
	if (!host->GetMember())
	{
		return TypePtr(new Variable(host.get(), reason, info));
	}
	Member * member = host->GetMember().get();
	Type * existingVar = dynamic_cast<Variable *>(member);
	if (existingVar == nullptr)
	{
		// Will throw an error message.
		return VariablePtr(new Variable(host.get(), reason, info));
	}

	if (existingVar != nullptr && !(existingVar->typeInfo == info))
	{
		std::stringstream ss;
		ss << "Variable was already defined with conflicting type information. ";	
		if (existingVar->typeInfo.IsConst && !info.IsConst)
		{
			ss << "Previous definition was const.";
		}
		if (existingVar->typeInfo.IsConstPointer && !info.IsConstPointer)
		{
			ss << "Previous definition was const pointer.";
		}
		if (existingVar->typeInfo.IsPointer && !info.IsPointer)
		{
			ss << "Previous definition was pointer.";
		}
		if (existingVar->typeInfo.IsReference && !info.IsReference)
		{
			ss << "Previous definition was reference.";
		}
		throw ModelInconsistencyException(member->GetReasonCreated(),
											  reason,
											  ss.str());	
	}
	// Re-use the previously set variable.
	return TypePtr(boost::dynamic_pointer_cast<Variable>(host->GetMember()));
}

const char * Type::GetTypeName() const
{
	return "Type";
}

Model::NodePtr Type::GetTypeNode() const
{
	return typeInfo.Type;
}

void intrusive_ptr_add_ref(Variable * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Variable * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}


END_NAMESPACE

#endif
