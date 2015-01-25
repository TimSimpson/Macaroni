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
#ifndef MACARONI_MODEL_CPP_CONSTRUCTOR_H
#define MACARONI_MODEL_CPP_CONSTRUCTOR_H

#include "../../ME.h"
#include "ConstructorPtr.h"
#include <Macaroni/Model/Cpp/Function.h>
#include <Macaroni/Model/Element.h>
#include "../ReasonPtr.h"
#include "ScopeMember.h"
#include "../SourcePtr.h"
#include "TypeInfo.h"
#include "VariablePtr.h"
#include <Macaroni/Model/Cpp/VariableAssignment.h>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a function in any possible Scope. */
class Constructor : public Function
{
friend void intrusive_ptr_add_ref(Constructor * p);
friend void intrusive_ptr_release(Constructor * p);

public:

	static ConstructorPtr Create(NodePtr home, Model::ReasonPtr reason);

	virtual ~Constructor();

	virtual const char * GetTypeName() const;

private:

	Constructor(Node * home, Model::ReasonPtr reason);

};

END_NAMESPACE

#endif
