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
#ifndef MACARONI_MODEL_CPP_FUNCTION_H
#define MACARONI_MODEL_CPP_FUNCTION_H

#include "../../ME.h"
#include "FunctionPtr.h"
#include "../Member.h"
#include "../ReasonPtr.h"
#include "ScopeMember.h"
#include "../SourcePtr.h"
#include "../Type.h"
#include "VariablePtr.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a function in any possible Scope. */
class Function : public Model::Member
{
friend void intrusive_ptr_add_ref(Function * p);
friend void intrusive_ptr_release(Function * p);

public:

	//VariablePtr GetArgument(int index) const;

	//int GetArgumentCount() const;
	/** Clears the given list and fills it with the Argument nodes. */
	// Model::NodeListPtr GetArguments() const;

	static FunctionPtr Create(NodePtr home, Model::ReasonPtr reason);

	virtual ~Function();

	virtual bool canBeChildOf(const Member * other) const;

	virtual bool DoesDefinitionReference(NodePtr node) const;

	//const std::string & GetCodeBlock() const;

	virtual const char * GetTypeName() const;

	Model::NodePtr GetTypeNode() const;

	///*inline const TypePtr GetReturnType() const
	//{
	//	return returnType;
	//}

	//inline bool IsConst() const
	//{
	//	return constMember;
	//}

	//inline bool IsInline() const
	//{
	//	return isInline;
	//}*/

	///** Attaches code to this function. If code was already attached, throws a
	// * ModelInconsistencyException. */
	//void SetCodeBlock(std::string & code, SourcePtr startOfCode);

	virtual void Visit(MemberVisitor * visitor) const;

protected:
	Function(Node * home, const char * typeName, Model::ReasonPtr reason);

private:
	
	Function(Node * home, Model::ReasonPtr reason);

	//bool codeAttached;

	//std::string codeBlock;
	
	//SourcePtr codeSource;

	//bool constMember;

	//const bool isInline;

	//TypePtr returnType;
};

END_NAMESPACE

#endif
