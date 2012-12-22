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
#ifndef MACARONI_MODEL_CPP_SCOPEMEMBER_H
#define MACARONI_MODEL_CPP_SCOPEMEMBER_H

#include "../../ME.h"
#include <Macaroni/Model/Cpp/Access.h>
#include "../Member.h"
#include "ScopeMemberPtr.h"
#include <string>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Concreate data concerning a Node.  In Macaroni, all nodes exist in a Scope
 * and are therefore ScopeMembers.  This class provides a link back to a node
 * which it uses to get certain useful data such as the name.  Methods for the
 * creation of new Model data remains in the Node. */
class ScopeMember : public Model::Member
{
friend void intrusive_ptr_add_ref(ScopeMember * p);
friend void intrusive_ptr_release(ScopeMember * p);
public:
	virtual ~ScopeMember();

	const std::string & GetName() const;

	AccessPtr GetAccess() const;

	std::string GetFullName() const;

	inline bool IsStatic() const
	{
		return isStatic;
	}

protected:
	ScopeMember(Model::Node * node, const char * typeName, ReasonPtr reason);

	ScopeMember(Model::Node * node, const char * typeName, ReasonPtr reason, Access access, bool isStatic);

	ScopeMember(const ScopeMember & other);

	void operator=(const ScopeMember & other);

	inline const Access & getAccess() const
	{
		return access;
	}

private:
	Access access;
	const bool isStatic;
};

END_NAMESPACE

#endif
