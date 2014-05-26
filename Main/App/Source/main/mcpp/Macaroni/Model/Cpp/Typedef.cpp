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
#ifndef MACARONI_MODEL_CPP_TYPEDEF_CPP
#define MACARONI_MODEL_CPP_TYPEDEF_CPP

#include "Typedef.h"
#include "../Member.h"
#include "../Node.h"
#include "ScopeMember.h"
#include "../Type.h"
#include "Typedef.h"
#include <Macaroni/Model/Project/Target.h>
#include <Macaroni/Model/Project/TargetPtr.h>

using Macaroni::Model::Member;
using Macaroni::Model::Node;
using Macaroni::Model::Reason;
using Macaroni::Model::ReasonPtr;
using Macaroni::Model::Project::Target;
using Macaroni::Model::Project::TargetPtr;
using Macaroni::Model::Type;
using Macaroni::Model::TypePtr;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Typedef::Typedef(Library * library, Node * node, ReasonPtr reason,
				 TypePtr type)
:Scope(library, node, "Typedef", reason), type(type)
{
}

Typedef::Typedef(Target * target, Node * node, ReasonPtr reason, TypePtr type)
:Scope(target, node, "Typedef", reason), type(type)
{
}

Typedef::~Typedef()
{
}

bool Typedef::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Typedef *>(other) != nullptr;
}


TypedefPtr Typedef::Create(LibraryPtr library, NodePtr home, ReasonPtr reason,
						   TypePtr type)
{
	return TypedefPtr(new Typedef(library.get(), home.get(), reason, type));
}

TypedefPtr Typedef::Create(TargetPtr target, NodePtr home, ReasonPtr reason,
						   TypePtr type)
{
	return TypedefPtr(new Typedef(target.get(), home.get(), reason, type));
}

TypePtr Typedef::GetType() const
{
	return type;
}

const char * Typedef::GetTypeName() const
{
	return "Typedef";
}

void intrusive_ptr_add_ref(Typedef * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Typedef * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

bool Typedef::RequiresHFile() const
{
	return true;
}


END_NAMESPACE

#endif
