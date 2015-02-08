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
#ifndef MACARONI_MODEL_CPP_TYPEDEF_H
#define MACARONI_MODEL_CPP_TYPEDEF_H

#include "../../ME.h"
#include <Macaroni/Model/Element.h>
#include "../Node.h"
#include <Macaroni/Model/Cpp/Scope.h>
#include <Macaroni/Model/TypePtr.h>
#include "TypedefPtr.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a Type which is stored in the Node space. */
class Typedef : public Scope
{
friend void intrusive_ptr_add_ref(Typedef * p);
friend void intrusive_ptr_release(Typedef * p);

public:

	static TypedefPtr Create(Macaroni::Model::Project::TargetPtr target,
		                     Macaroni::Model::NodePtr home,
		                     Macaroni::Model::ReasonPtr reason,
		                     Access access,
		                     TypePtr type);

	~Typedef();

	Macaroni::Model::TypePtr GetType() const;

	const char * GetTypeName() const override;

	bool RequiresHFile() const override;

protected:
	Typedef(Macaroni::Model::Project::Target * target,
		    Macaroni::Model::Node *		home,
		    Macaroni::Model::ReasonPtr	reason,
		    Access access,
		    Macaroni::Model::TypePtr	type);

private:
	Macaroni::Model::TypePtr type;
};

END_NAMESPACE

#endif
