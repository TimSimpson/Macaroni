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
#ifndef MACARONI_MODEL_CPP_SCOPE_CPP
#define MACARONI_MODEL_CPP_SCOPE_CPP

#include "Scope.h"
#include <Macaroni/Exception.h>
#include "../Library.h"
#include "../Node.h"
#include <Macaroni/StringException.h>
#include <Macaroni/Model/Project/Target.h>
#include <Macaroni/Model/Project/TargetPtr.h>

using Macaroni::Model::Project::Target;
using Macaroni::Model::Project::TargetPtr;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

void intrusive_ptr_add_ref(Scope * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Scope * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

Scope::Scope(Library * library, Node * node, const char * typeName, ReasonPtr reason)
:ScopeMember(node, typeName, reason),
 library(library),
 target(0)
{
}

Scope::Scope(Target * target, Node * node, const char * typeName, ReasonPtr reason)
:ScopeMember(node, typeName, reason),
 library(0),
 target(target)
{
	target->AddElement(this);
}


LibraryPtr Scope::GetLibrary() const
{
	if (!target) {
		return LibraryPtr(library);
	} else {
		throw Macaroni::StringException("Deprecated- use 'OwnedBy' instead.");
	}
}

size_t Scope::GetMemberCount() const
{
	return getNode()->GetChildCount();
}

NodePtr Scope::GetMember(int index) const
{
	return getNode()->GetChild(index);
}

TargetPtr Scope::GetOwner() const
{
	if (!target) 
	{
		return Element::GetOwner();
	}
	return target;
}

bool Scope::OwnedBy(Macaroni::Model::Project::TargetPtr target) const {
	if (!target) 
	{
		return false;
	}
	if (*target == *(this->target)) 
	{
		return true;
	}
	return this->target->OwnedBy(target.get());		
}

void Scope::SwitchOwner(Macaroni::Model::Project::TargetPtr newTarget)
{	
	if (0 != target)
	{
		Target & from = *target;
		Target & to = *newTarget;
		this->target = newTarget.get();
		Target::SwitchOwner(*this, from, to);
	}
	else
	{
		this->target = newTarget.get();
		target->AddElement(this);
	}
}

END_NAMESPACE

#endif
