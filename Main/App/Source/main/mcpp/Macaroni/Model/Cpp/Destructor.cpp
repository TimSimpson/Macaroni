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
#ifndef MACARONI_MODEL_CPP_DESTRUCTOR_CPP
#define MACARONI_MODEL_CPP_DESTRUCTOR_CPP

#include <Macaroni/Model/Cpp/Class.h>
#include "Destructor.h"
#include "FunctionOverload.h"
#include "Primitive.h"
#include <Macaroni/Exception.h>
#include "../ModelInconsistencyException.h"
#include "Namespace.h"
#include <Macaroni/Model/Node.h>
#include <Macaroni/Model/Reason.h>
#include "Scope.h"
#include "ScopeMember.h"
#include <Macaroni/StringException.h>
#include <Macaroni/Model/Project/Target.h>
#include <Macaroni/Model/Project/TargetPtr.h>
#include "Variable.h"
#include <memory>
#include <sstream>

using boost::optional;
using Macaroni::Model::ModelInconsistencyException;
using Macaroni::Model::Node;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

namespace
{
	inline TypePtr voidType()
	{
		TypeModifiers modifiers;
		return TypePtr(new Type(NodePtr(), modifiers));
	}
};

Destructor::Destructor(Node * home, Model::ReasonPtr reason, bool isInline,
                       AccessPtr access, bool isVirtual,
                       const optional<ExceptionSpecifier> exceptionSpecifier)
:Function(home, reason)
{
	Macaroni::Model::Project::TargetPtr none;
	FunctionOverload::Create(
		none, FunctionOverload::CreateNewFunctionOverloadNode(this),
		isInline, access, false, isVirtual,
		voidType(), false, exceptionSpecifier, reason,
		NodePtr{},
		boost::none);
}

Destructor::~Destructor()
{
}

DestructorPtr Destructor::Create(NodePtr host, bool isInline, AccessPtr access,
								 bool isVirtual,
					 const optional<ExceptionSpecifier> exceptionSpecifier,
								 Model::ReasonPtr reason)
{
	if (!host->GetElement())
	{
		return DestructorPtr(new Destructor(host.get(), reason, isInline,
			                                access, isVirtual,
			                                exceptionSpecifier));
	}
	DestructorPtr dPtr = host->GetElement<DestructorPtr>();
	if (!dPtr)
	{
		// Something already exists here, and it isn't a destructor.
		// Will throw an error message.
		return DestructorPtr(new Destructor(host.get(), reason, isInline,
			access, isVirtual, exceptionSpecifier));
	}
	// Re-use previously set variable.
	return dPtr;
}

FunctionOverloadPtr Destructor::GetFunctionOverload()
{
	Node * node = this->getNode();
	NodePtr nodePtr = node->Find("Overload#0");
	//MemberPtr member = nodePtr->GetMember();
	//FunctionOverloadPtr fol = boost::dynamic_pointer_cast<FunctionOverload>(member);
	FunctionOverloadPtr fol = nodePtr->GetElement<FunctionOverloadPtr>();
	if (!fol)
	{
		throw Macaroni::StringException("Tried to grab the destructors "
			"FunctionOverload, but somehow it was a different member.");
	}
	return fol;
}

const char * Destructor::GetTypeName() const
{
	return "Destructor";
}

void intrusive_ptr_add_ref(Destructor * p)
{
	intrusive_ptr_add_ref((FunctionOverload *)p);
}

void intrusive_ptr_release(Destructor * p)
{
	intrusive_ptr_release((FunctionOverload *)p);
}

END_NAMESPACE

#endif
