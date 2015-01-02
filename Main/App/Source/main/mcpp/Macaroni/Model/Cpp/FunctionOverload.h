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
#include <Macaroni/Model/Element.h>
#include <Macaroni/Model/Cpp/ExceptionSpecifier.h>
#include <Macaroni/Model/Cpp/Function.h>
#include <Macaroni/Model/Cpp/FunctionOverloadPtr.h>
#include <boost/optional.hpp>
#include "../ReasonPtr.h"
#include "Scope.h"
#include "../SourcePtr.h"
#include <Macaroni/Model/Project/TargetPtr.h>
#include "../Type.h"
#include "VariablePtr.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a function in any possible Scope. */
class FunctionOverload : public Scope
{
friend void intrusive_ptr_add_ref(FunctionOverload * p);
friend void intrusive_ptr_release(FunctionOverload * p);

public:
	static FunctionOverloadPtr Create(
		Macaroni::Model::Project::TargetPtr target,
		NodePtr home, bool isInline,
		const AccessPtr access,
		const bool isStatic, bool isVirtual,
		const TypePtr rtnType,
		bool constMember,
		const boost::optional<ExceptionSpecifier> exceptionSpecifier,
		Model::ReasonPtr reason,
		NodePtr templateParameterList=NodePtr{},
		boost::optional<Macaroni::Model::NodeListPtr> imports=boost::none
	);

	static NodePtr CreateNewFunctionOverloadNode(FunctionPtr home);

	virtual ~FunctionOverload();


	/** Clears the given list and fills it with the Argument nodes. */
	Model::NodeListPtr GetArguments() const;

	const std::string & GetCodeBlock() const;

	NodeListPtr GetImportedNodes() const;

	inline bool CodeBlockShouldAddRedirect() const
	{
		return codeBlockAddRedirect;
	}

	SourcePtr GetCodeSource() const
	{
		return codeSource;
	}

	inline const boost::optional<ExceptionSpecifier> GetExceptionSpecifier()
	const
	{
		return exceptionSpecifier;
	}

	inline const TypePtr GetReturnType() const
	{
		return returnType;
	}

	Model::NodePtr GetTemplateParameterList();

	virtual const char * GetTypeName() const;

	bool HasCodeBlock() const;

	inline bool IsConst() const
	{
		return constMember;
	}

	/** Uses the delete keyword. */
	inline bool IsDeleted() const
	{
		return usesDeleteKeyword;
	}

	bool IsInline() const;

	inline bool IsPureVirtual() const
	{
		return isPureVirtual;
	}

	inline bool IsVirtual() const
	{
		return isVirtual;
	}

	virtual bool RequiresCppFile() const;

	virtual bool RequiresHFile() const;

	/** Attaches code to this function. If code was already attached, throws a
	 * ModelInconsistencyException.
	 * If codeBlockAddRedirect is true, it will modify the generated source so
	 * that errors produced in the C++ file will be redirected to the original
	 * Macaroni source code.
	 */
	void SetCodeBlock(std::string & code, SourcePtr startOfCode,
		              bool codeBlockAddRedirect);

	/** Specifies that this function has the "default" keyword. */
	void SetDefault(SourcePtr startOfCode);

	/** Specifies this function is deleted. */
	void SetDelete(SourcePtr startOfCode);

	/** Specifies that this function overload is a "pure virtual function"
	 *  which means it ends in "= 0". */
	void SetPureVirtual(SourcePtr startOfCode);

	/** Uses the default keyword. */
	inline bool UsesDefault() const
	{
		return usesDefaultKeyword;
	}

	inline bool UsesInlineKeyword() const
	{
		return isInline;
	}

protected:
	//FunctionOverload(Node home, bool isInline, const Access access, const bool isStatic, const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);

	FunctionOverload(Macaroni::Model::Project::Target * target,
		Node * home, Model::ReasonPtr reason,
		bool isInline, Access access, const bool isStatic, bool isVirtual,
		const TypePtr rtnTypeInfo, bool constMember,
		const boost::optional<ExceptionSpecifier> exceptionSpecifier,
		Node * templateParameterList,
		boost::optional<Macaroni::Model::NodeListPtr> imports);

private:

	std::string codeBlock;

	bool codeBlockAddRedirect;

	SourcePtr codeSource;

	bool constMember;

	const boost::optional<ExceptionSpecifier> exceptionSpecifier;

	boost::optional<Macaroni::Model::NakedNodeList> imports;

	const bool isInline;

	bool isPureVirtual;

	const bool isVirtual;

	Macaroni::Model::ReasonPtr reasonCreated;

	TypePtr returnType;

	// Ensures the function wasn't already set and does error checking
	// before another function finishes.
	void setCodeDefinitionSource(SourcePtr startOfCode, const char * const msg);

	Node * templateParameterList;

	bool usesDefaultKeyword;

	bool usesDeleteKeyword;
};

END_NAMESPACE

#endif
