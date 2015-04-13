/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MACARONI_MODEL_CPP_VARIABLE_CPP
#define MACARONI_MODEL_CPP_VARIABLE_CPP

#include <Macaroni/Model/Cpp/Primitive.h>
#include <Macaroni/Exception.h>
#include "../ModelInconsistencyException.h"
#include <Macaroni/Model/Cpp/Namespace.h>
#include "../Node.h"
#include <Macaroni/Model/Cpp/Scope.h>
#include <Macaroni/Model/Cpp/ScopeMember.h>
#include "Variable.h"
#include <memory>
#include <sstream>
#include <Macaroni/Core/Visitor.h>


using Macaroni::Core::BaseVisitor;
using Macaroni::Model::ModelInconsistencyException;
using Macaroni::Model::Node;
using Macaroni::Model::Project::Target;
using Macaroni::Model::Project::TargetPtr;


BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Variable::Variable(Target * t, Node * parent, ReasonPtr reason, Access access,
	               bool isStatic, const TypePtr type, std::string initializer)
:ScopeMember(parent, reason, access, isStatic),
 initializer(initializer),
 type(type)
{
	if (t)
	{
		t->AddElement(this);
	}
}

Variable::~Variable()
{

}


bool Variable::Accept(BaseVisitor & v)
{
    return Visit(*this, v);
}

VariablePtr Variable::Create(TargetPtr tHome, NodePtr host, AccessPtr access,
	bool isStatic, const TypePtr type, std::string initializer, ReasonPtr reason)
{
	if (!host->GetElement())
	{
		//return Variable::Create(host, access, type, reason);
		return VariablePtr(new Variable(tHome.get(), host.get(), reason, *access, isStatic, type, initializer));
	}
	Element * element = host->GetElement().get();
	Variable * existingVar = dynamic_cast<Variable *>(element);
	if (existingVar == nullptr)
	{
		// Will throw an error message.
		//return Variable::Create(host, access, type, reason);
		return VariablePtr(new Variable(tHome.get(), host.get(), reason, *access, isStatic, type, initializer));
	}

	if (existingVar != nullptr && !(existingVar->type->operator==(*type.get())))
	{
		std::stringstream ss;
		ss << "Variable was already defined with conflicting type information. ";

		const auto & newMods = type->GetModifiers();
		const auto & oldMods = existingVar->type->GetModifiers();
		if (oldMods != newMods)
		{
			ss << "Previous definition had different type modifiers.";
		}
		ss << "(" << oldMods << " vs " << newMods << ")";
		throw ModelInconsistencyException(element->GetReasonCreated(),
											  reason,
											  ss.str());
	}
	// Re-use the previously set variable.
	return VariablePtr(boost::dynamic_pointer_cast<Variable>(host->GetElement()));
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

bool Variable::RequiresCppFile() const
{
	return IsStatic();
}

bool Variable::RequiresHFile() const
{
	return getAccess().VisibleInHeader();
}


END_NAMESPACE

#endif
