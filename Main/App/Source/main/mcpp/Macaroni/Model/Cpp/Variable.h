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
#ifndef MACARONI_MODEL_CPP_VARIABLE_H
#define MACARONI_MODEL_CPP_VARIABLE_H

#include "../../ME.h"
#include <Macaroni/Model/Element.h>
#include "../ReasonPtr.h"
#include "ScopeMember.h"
#include "VariablePtr.h"
#include <Macaroni/Model/Type.h>
#include <Macaroni/Model/TypePtr.h>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a variable.  Can be in a namespace, in a class, or in a function
 * (the argument list). */
class Variable : public ScopeMember
{
friend void intrusive_ptr_add_ref(Variable * p);
friend void intrusive_ptr_release(Variable * p);

public:

	static VariablePtr Create(NodePtr home, AccessPtr access, bool isStatic, const TypePtr type, std::string initializer, Model::ReasonPtr reason);

	virtual ~Variable();

	virtual const char * GetTypeName() const;

	inline const TypePtr GetType() const
	{
		return type;
	}

	inline const std::string & GetInitializer() const
	{
		return initializer;
	}

	virtual bool RequiresCppFile() const;

	virtual bool RequiresHFile() const;

private:

	Variable(Node * home, Model::ReasonPtr reason, Access access, bool isStatic, const TypePtr type, std::string initializer);

	std::string initializer;

	TypePtr type;
};

END_NAMESPACE

#endif
