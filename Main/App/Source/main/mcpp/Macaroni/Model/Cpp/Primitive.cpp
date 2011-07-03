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
#ifndef MACARONI_MODEL_CPP_PRIMITIVE_CPP
#define MACARONI_MODEL_CPP_PRIMITIVE_CPP

#include "Primitive.h"
#include "../../Exception.h"
#include "Namespace.h"
#include "../Node.h"
#include "Scope.h"
#include "ScopeMember.h"
#include <memory>
#include <sstream>

using Macaroni::Model::Node;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Primitive::Primitive(Node * parent, ReasonPtr reason)
:ScopeMember(parent, "Primitive", reason)
{
}

Primitive::~Primitive()
{
	
}

/*void Primitive::AddCppPrimitives(ContextPtr context)
{
	NodePtr root = context->GetRoot();
	NodePtr integer = root->FindNode("integer");
	if (!integer)
	{
		integer = root->FindOrCreateNode("int");
		Primitive::Create(integer, Reason::Create(
	}
}*/

bool Primitive::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Scope *>(other) != nullptr;
}

PrimitivePtr Primitive::Create(NodePtr parent, ReasonPtr reason)
{
	return PrimitivePtr(new Primitive(parent.get(), reason));
}

const char * Primitive::GetTypeName() const
{
	return "Primitive";
}


void intrusive_ptr_add_ref(Primitive * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Primitive * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

void Primitive::Visit(MemberVisitor * visitor) const
{
	// Do nothing.
}

END_NAMESPACE

#endif
