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
#ifndef MACARONI_MODEL_CPP_FUNCTIONOVERLOAD_H
#define MACARONI_MODEL_CPP_FUNCTIONOVERLOAD_H

#include "../../ME.h"
#include "Function.h"
#include "FunctionOverloadPtr.h"
#include "../Member.h"
#include "../ReasonPtr.h"
#include "ScopeMember.h"
#include "../SourcePtr.h"
#include <Macaroni/Model/Project/TargetPtr.h>
#include "../Type.h"
#include "VariablePtr.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a function in any possible Scope. */
class FunctionOverload : public ScopeMember
{
friend void intrusive_ptr_add_ref(FunctionOverload * p);
friend void intrusive_ptr_release(FunctionOverload * p);

public:
	static FunctionOverloadPtr Create(NodePtr home, bool isInline, 
									  const AccessPtr access, 
									  const bool isStatic, bool isVirtual,
									  const TypePtr rtnType, 
									  bool constMember, 
									  bool throwSpecifier,
									  Model::ReasonPtr reason);

	static FunctionOverloadPtr Create(FunctionPtr home, bool isInline, 
									  const AccessPtr access, 
									  const bool isStatic, bool isVirtual,
									  const TypePtr rtnType, 									  
									  bool constMember, bool throwSpecifier,
									  Model::ReasonPtr reason);

	static FunctionOverloadPtr Create(Function * home, bool isInline, 
									  const AccessPtr access, 
									  const bool isStatic, 
									  bool isVirtual,
									  const TypePtr rtnType,
									  bool constMember, bool throwSpecifier,
									  Model::ReasonPtr reason);

	virtual ~FunctionOverload();
	

	/** Clears the given list and fills it with the Argument nodes. */
	Model::NodeListPtr GetArguments() const;

	virtual bool DoesDefinitionReference(NodePtr node) const;

	const std::string & GetCodeBlock() const;
	
	inline bool CodeBlockShouldAddRedirect() const
	{
		return codeBlockAddRedirect;
	}

	SourcePtr GetCodeSource() const
	{
		return codeSource;
	}

	inline const TypePtr GetReturnType() const
	{
		return returnType;
	}

	virtual const char * GetTypeName() const;

	bool HasCodeBlock() const;

	bool HasThrowSpecifier() const
	{
		return throwSpecifier;
	}

	inline bool IsConst() const
	{
		return constMember;
	}

	inline bool IsInline() const
	{
		return isInline;
	}

	inline bool IsPureVirtual() const
	{
		return isPureVirtual;
	}

	inline bool IsVirtual() const
	{
		return isVirtual;
	}
	
	/** Attaches code to this function. If code was already attached, throws a
	 * ModelInconsistencyException. 
	 * If codeBlockAddRedirect is true, it will modify the generated source so
	 * that errors produced in the C++ file will be redirected to the original
	 * Macaroni source code.
	 */
	void SetCodeBlock(std::string & code, SourcePtr startOfCode, 
		              bool codeBlockAddRedirect);	

	/** Specifies that this function overload is a "pure virtual function"
	 *  which means it ends in "= 0". */
	void SetPureVirtual(SourcePtr startOfCode);

	virtual void Visit(MemberVisitor * visitor) const;

protected:
	//FunctionOverload(Node home, bool isInline, const Access access, const bool isStatic, const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);
	
	FunctionOverload(Node * home, Model::ReasonPtr reason, bool isInline, Access access, const bool isStatic, bool isVirtual, const TypePtr rtnTypeInfo, bool constMember, bool throwSpecifier);

	FunctionOverload(Node * home, const char * typeName, Model::ReasonPtr reason, bool isInline, Access access, const bool isStatic, bool isVirtual, const TypePtr rtnTypeInfo, bool constMember, bool throwSpecifier);

	virtual bool canBeChildOf(const Member * other) const;

private:		

	std::string codeBlock;

	bool codeBlockAddRedirect;
	
	SourcePtr codeSource;

	bool constMember;

	const bool isInline;

	bool isPureVirtual;

	const bool isVirtual;

	Macaroni::Model::ReasonPtr reasonCreated;

	TypePtr returnType;

	bool throwSpecifier;
};

END_NAMESPACE

#endif
