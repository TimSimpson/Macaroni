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
#ifndef MACARONI_MODEL_CPP_NAMESPACE_CPP
#define MACARONI_MODEL_CPP_NAMESPACE_CPP

#include "Namespace.h"
#include <Macaroni/Exception.h>
#include "../Node.h"
#include <memory>
#include <sstream>
#include <Macaroni/Model/Project/Target.h>
#include <Macaroni/Model/Project/TargetPtr.h>

using Macaroni::Model::Node;
using Macaroni::Model::Project::Target;
using Macaroni::Model::Project::TargetPtr;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Namespace::Namespace(Target * target, Node * parent, ReasonPtr reason)
:Scope(target, parent, reason)
{
}

Namespace::~Namespace()
{

}

NamespacePtr Namespace::Create(TargetPtr target, NodePtr parent, ReasonPtr reason)
{
	return NamespacePtr(new Namespace(target.get(), parent.get(), reason));
}


const char * Namespace::GetTypeName() const
{
	return "Namespace";
}


bool Namespace::IsInstance(ElementPtr other)
{
	if (!other)
	{
		return false;
	}
	NamespacePtr nsPtr = boost::dynamic_pointer_cast<Namespace>(other);
	return (!!nsPtr);
}

void intrusive_ptr_add_ref(Namespace * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Namespace * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

END_NAMESPACE

#endif
