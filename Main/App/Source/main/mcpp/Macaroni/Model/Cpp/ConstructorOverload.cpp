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
#ifndef MACARONI_MODEL_CPP_CONSTRUCTOROVERLOAD_CPP
#define MACARONI_MODEL_CPP_CONSTRUCTOROVERLOAD_CPP

#include "ConstructorOverload.h"
#include <Macaroni/Model/Cpp/Class.h>
#include "Constructor.h"
#include "Primitive.h"
#include <Macaroni/Exception.h>
#include "../ModelInconsistencyException.h"
#include "Namespace.h"
#include "../Node.h"
#include <Macaroni/Model/Reason.h>
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
		NodePtr node(home);
		return TypePtr(new Type(node, modifiers));
	}
};

ConstructorOverload::ConstructorOverload(Node * home, Model::ReasonPtr reason,
										 bool isInline, Access access,
										 bool isExplicit,
				   const boost::optional<ExceptionSpecifier> exceptionSpecifier)
:FunctionOverload(home, reason, isInline, access, true,
				  false, voidTypeInfo(home), false, exceptionSpecifier),
 assignments(),
 isExplicit(isExplicit)
{
}

ConstructorOverload::~ConstructorOverload()
{
}

void ConstructorOverload::AddAssignment(const VariableAssignment & assignment)
{
	assignments.push_back(assignment);
}

ConstructorOverloadPtr ConstructorOverload::Create(
	NodePtr node, bool isInline, AccessPtr access, bool isExplicit,
	const boost::optional<ExceptionSpecifier> exceptionSpecifier,
	Model::ReasonPtr reason)
{
	ConstructorOverload * co = new ConstructorOverload(
		node.get(), reason, isInline, *access, isExplicit, exceptionSpecifier);
	return ConstructorOverloadPtr(co);
}

NodePtr ConstructorOverload::CreateNode(NodePtr ctorHomeNode)
{
	return ctorHomeNode->CreateNextInSequence("Overload#");
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

END_NAMESPACE

#endif

