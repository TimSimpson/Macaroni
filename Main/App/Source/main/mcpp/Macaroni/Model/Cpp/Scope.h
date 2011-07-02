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
#include "../LibraryMember.h"
#include "../Member.h"
#include "../NodePtr.h"
#include "ScopeMember.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Scope : public ScopeMember, public virtual Model::LibraryMember
{
friend void intrusive_ptr_add_ref(Scope * p);
friend void intrusive_ptr_release(Scope * p);

public:		
	virtual Model::LibraryPtr GetLibrary() const;

	size_t GetMemberCount() const;

	NodePtr GetMember(int index) const;

	virtual const char * GetTypeName() const = 0;

protected:
	Scope(Library * library, Node * scope, const char * typeName, ReasonPtr reason);
	Scope(const Scope & other);
	void operator=(const Scope & other);
	virtual ~Scope(){}

private:
	Model::Library * library;
};

END_NAMESPACE

#endif
