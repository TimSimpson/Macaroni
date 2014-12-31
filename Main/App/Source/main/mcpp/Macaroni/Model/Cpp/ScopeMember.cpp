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
#ifndef MACARONI_MODEL_CPP_SCOPEMEMBER_CPP
#define MACARONI_MODEL_CPP_SCOPEMEMBER_CPP

#include "ScopeMember.h"
#include "../Node.h"
#include <sstream>

using boost::optional;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

ScopeMember::ScopeMember(Node * node, ReasonPtr reason,
                         optional<Access> access, optional<bool> isStatic)
:   Element(node, reason),
    access(access.get_value_or( *(Access::Private())) ),
    isStatic(isStatic.get_value_or(false))
{
}

ScopeMember::~ScopeMember()
{
}

AccessPtr ScopeMember::GetAccess() const
{
	AccessPtr aPtr(new Access(this->access));
	return aPtr;
}

const std::string & ScopeMember::GetName() const
{
	return getNode()->GetName();
}

std::string ScopeMember::GetFullName() const
{
	return getNode()->GetFullName();
}

void intrusive_ptr_add_ref(ScopeMember * p)
{
	intrusive_ptr_add_ref(p->getNode());
}

void intrusive_ptr_release(ScopeMember * p)
{
	intrusive_ptr_release(p->getNode());
}


END_NAMESPACE

#endif
