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
#ifndef MACARONI_MODEL_CPP_DESTRUCTOR_H
#define MACARONI_MODEL_CPP_DESTRUCTOR_H

#include "../../ME.h"
#include "DestructorPtr.h"
#include "Function.h"
#include "FunctionOverload.h"
#include "../Member.h"
#include "../ReasonPtr.h"
#include "ScopeMember.h"
#include "../SourcePtr.h"
#include "TypeInfo.h"
#include "VariablePtr.h"
#include "VariableAssignment.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Destructor : public Function
{
friend void intrusive_ptr_add_ref(Destructor * p);
friend void intrusive_ptr_release(Destructor * p);

public:

	static DestructorPtr Create(NodePtr home, bool isInline, AccessPtr access, 
		                        bool isVirtual, Model::ReasonPtr reason);

	virtual ~Destructor();

	virtual bool canBeChildOf(const Member * other) const;

	/** Destructors can only have one overload so they are created with it,
	    unlike Constructors or Functions which need to have them manually 
		added.  This helper just returns it. */
	FunctionOverloadPtr GetFunctionOverload();

	virtual const char * GetTypeName() const;

	virtual void Visit(MemberVisitor * visitor) const;

private:
	
	Destructor(Node * home, Model::ReasonPtr reason, bool isInline, 
		       AccessPtr access, bool isVirtual);
	
};

END_NAMESPACE

#endif
