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
#ifndef MACARONI_MODEL_CPP_SCOPE_H
#define MACARONI_MODEL_CPP_SCOPE_H

#include "../../ME.h"
#include <vector>
#include <Macaroni/Model/Element.h>
#include "../NodePtr.h"
#include "ScopeMember.h"
#include <Macaroni/Model/Project/TargetPtr.h>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Scope : public ScopeMember
{
friend void intrusive_ptr_add_ref(Scope * p);
friend void intrusive_ptr_release(Scope * p);

public:
	NodePtr GetMember(int index) const;

	virtual Macaroni::Model::Project::TargetPtr GetOwner() const;

	virtual const char * GetTypeName() const = 0;

	/** Returns true if this Scope's target is the given target or a child of
	 *  the given target. */
	virtual bool OwnedBy(Macaroni::Model::Project::TargetPtr target) const;

	virtual void SwitchOwner(Macaroni::Model::Project::TargetPtr target);

protected:
	Scope(Macaroni::Model::Project::Target * target, Node * scope,
		  ReasonPtr reason);
	Scope(const Scope & other);
	void operator=(const Scope & other);
	virtual ~Scope(){}

private:
	Macaroni::Model::Project::Target * target;
};

END_NAMESPACE

#endif
