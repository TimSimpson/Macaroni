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
#ifndef MACARONI_MODEL_CPP_CONSTRUCTOROVERLOAD_H
#define MACARONI_MODEL_CPP_CONSTRUCTOROVERLOAD_H

#include "../../ME.h"
#include <Macaroni/Model/Cpp/Constructor.h>
#include <Macaroni/Model/Cpp/ConstructorOverloadPtr.h>
#include <Macaroni/Model/Cpp/Function.h>
#include <Macaroni/Model/Cpp/FunctionOverload.h>
#include "../Member.h"
#include <Macaroni/Model/Node.h>
#include "../ReasonPtr.h"
#include "ScopeMember.h"
#include "../SourcePtr.h"
#include "TypeInfo.h"
#include "VariablePtr.h"
#include "VariableAssignment.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class ConstructorOverload : public FunctionOverload
{
friend void intrusive_ptr_add_ref(ConstructorOverload * p);
friend void intrusive_ptr_release(ConstructorOverload * p);

public:

	ConstructorPtr Create(NodePtr host, bool isInline, AccessPtr access,
						  Model::ReasonPtr reason);

	static ConstructorOverloadPtr Create(NodePtr home,
		bool isInline, AccessPtr access,
		bool isExplicit,
		const boost::optional<ExceptionSpecifier> exceptionSpecifier,
		Model::ReasonPtr reason);

	virtual ~ConstructorOverload();

	void AddAssignment(const VariableAssignment & assignment);

	virtual bool canBeChildOf(const Member * other) const;

	ConstructorOverloadPtr Create(FunctionPtr home, bool isInline, const Access access,
							   const bool isStatic,
							   const TypePtr rtnType,
							   bool constMember,
				   const boost::optional<ExceptionSpecifier> exceptionSpecifier,
							   Model::ReasonPtr reason);

	/** Creates a new Node under the given Node where the overload will be. */
	static NodePtr CreateNode(NodePtr ctorHomeNode);

	const VariableAssignment & GetAssignment(int index) const;

	int GetAssignmentCount() const;

	virtual const char * GetTypeName() const;

	inline bool IsExplicit() const
	{
		return isExplicit;
	}

private:

	ConstructorOverload(Model::Node * home, Model::ReasonPtr reason,
		                bool isInline, Access access, bool isExplicit,
		           const boost::optional<ExceptionSpecifier> exceptionSpecifier);

	std::vector<VariableAssignment> assignments;

	bool isExplicit;
};

END_NAMESPACE

#endif
