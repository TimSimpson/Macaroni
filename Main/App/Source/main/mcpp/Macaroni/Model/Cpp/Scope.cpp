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
#include "../Library.h"
#include "../Node.h"

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
 library(library)
{
}

LibraryPtr Scope::GetLibrary() const
{
	return LibraryPtr(library);
}

size_t Scope::GetMemberCount() const
{
	return getNode()->GetChildCount();
}

NodePtr Scope::GetMember(int index) const
{
	return getNode()->GetChild(index);
}

END_NAMESPACE

#endif