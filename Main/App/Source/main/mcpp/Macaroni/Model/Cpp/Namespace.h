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
#ifndef MACARONI_MODEL_CPP_NAMESPACE_H
#define MACARONI_MODEL_CPP_NAMESPACE_H

#include "../../ME.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "../LibraryPtr.h"
#include "../Member.h"
#include "../MemberVisitor.h"
#include "NamespacePtr.h"
#include "Scope.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Namespace : public Scope
{
friend struct NamespaceLuaFunctions;
friend void intrusive_ptr_add_ref(Namespace * p);
friend void intrusive_ptr_release(Namespace * p);
public:	
	static NamespacePtr Create(LibraryPtr library, NodePtr home, ReasonPtr reason);

	//ClassPtr FindClass(std::string & name);

	//NamespacePtr GetParent() const;

	/** Gets the root at the core of this Namespace tree. */
	//NamespacePtr GetRoot();

	//ClassPtr InsertClass(ClassPtr newInstance);

	//static NamespacePtr GetRoot();

	virtual ~Namespace();

	virtual bool canBeChildOf(const Member * other) const;

	virtual const char * GetTypeName() const;

	/** Returns true if the given member is an instance of Class. */
	static bool IsInstance(Model::MemberPtr other);

	virtual void Visit(MemberVisitor * visitor) const;

protected:	
	Namespace(Library * library, Node * home, ReasonPtr reason);
private:
	
	/** Assumes the given namespace does not exist and creates it as a child
	 * of this namespace. */
	//Namespace * create(std::string & name);

	//Namespace * find(std::string & name);
};

void intrusive_ptr_add_ref(Namespace * p);

void intrusive_ptr_release(Namespace * p);

END_NAMESPACE

#endif
