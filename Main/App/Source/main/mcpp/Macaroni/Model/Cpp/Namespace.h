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
#include <Macaroni/Model/Element.h>
#include <Macaroni/Model/ElementPtr.h>
#include <boost/shared_ptr.hpp>
#include "NamespacePtr.h"
#include "Scope.h"
#include <Macaroni/Model/Project/TargetPtr.h>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Namespace : public Scope
{
friend struct NamespaceLuaFunctions;
friend void intrusive_ptr_add_ref(Namespace * p);
friend void intrusive_ptr_release(Namespace * p);
public:
	static NamespacePtr Create(Macaroni::Model::Project::TargetPtr target,
		NodePtr home, ReasonPtr reason);

	virtual ~Namespace();

	virtual const char * GetTypeName() const;

	/** Returns true if the given member is an instance of Class. */
	static bool IsInstance(Model::ElementPtr other);

protected:
	Namespace(Macaroni::Model::Project::Target * target,
	          Node * home, ReasonPtr reason);
private:
};

void intrusive_ptr_add_ref(Namespace * p);

void intrusive_ptr_release(Namespace * p);

END_NAMESPACE

#endif
