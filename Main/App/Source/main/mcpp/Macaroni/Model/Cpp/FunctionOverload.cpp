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
#ifndef MACARONI_MODEL_CPP_FUNCTIONOVERLOAD_CPP
#define MACARONI_MODEL_CPP_FUNCTIONOVERLOAD_CPP

#include "FunctionOverload.h"
#include <boost/format.hpp>
#include <Macaroni/Model/ModelInconsistencyException.h>
#include <Macaroni/Model/Reason.h>
#include <Macaroni/Model/Source.h>
#include <Macaroni/Model/Cpp/Variable.h>

using Macaroni::Model::ModelInconsistencyException;
using boost::optional;
using Macaroni::Model::Project::TargetPtr;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

FunctionOverload::FunctionOverload
(
 Node * home, Model::ReasonPtr reason, bool isInline,
 Access access, const bool isStatic, bool isVirtual,
 const TypePtr rtnTypeInfo, bool constMember,
 const optional<ExceptionSpecifier> exceptionSpecifier
)
:ScopeMember(home, reason, access, isStatic),
 codeBlock(),
 codeBlockAddRedirect(false),
 constMember(constMember),
 codeSource(),
 isInline(isInline),
 isPureVirtual(false),
 isVirtual(isVirtual),
 returnType(rtnTypeInfo),
 exceptionSpecifier(exceptionSpecifier),
 usesDefaultKeyword(false),
 usesDeleteKeyword(false)
{
}

FunctionOverload::~FunctionOverload()
{
}

FunctionOverloadPtr FunctionOverload::Create(FunctionPtr home,
											 bool isInline, const AccessPtr access,
											 const bool isStatic,
											 bool isVirtual,
											 const TypePtr rtnType,
											 bool constMember,
						  const optional<ExceptionSpecifier> exceptionSpecifier,
											 Model::ReasonPtr reason)
{
	Function * fn = home.get();
	return Create(fn, isInline, access, isStatic, isVirtual, rtnType,
		          constMember, exceptionSpecifier, reason);
}

FunctionOverloadPtr FunctionOverload::Create(Function * fn,
											 bool isInline, const AccessPtr access,
											 const bool isStatic,
											 bool isVirtual,
											 const TypePtr rtnType,
											 bool constMember,
						  const optional<ExceptionSpecifier> exceptionSpecifier,
											 Model::ReasonPtr reason)
{
	Node * node = fn->GetNode().get();
	NodePtr foNode = node->CreateNextInSequence("Overload#");
	return Create(foNode, isInline, access, isStatic, isVirtual, rtnType,
		          constMember, exceptionSpecifier, reason);
}

FunctionOverloadPtr FunctionOverload::Create(NodePtr foNode,
											 bool isInline, const AccessPtr access,
											 const bool isStatic,
											 bool isVirtual,
											 const TypePtr rtnType,
											 bool constMember,
						const optional<ExceptionSpecifier> exceptionSpecifier,
											 Model::ReasonPtr reason)
{
	FunctionOverload * fo = new FunctionOverload(foNode.get(), reason,
		isInline, *access,
		isStatic, isVirtual, rtnType, constMember, exceptionSpecifier);
	return FunctionOverloadPtr(fo);
}

NodeListPtr FunctionOverload::GetArguments() const
{
	NodeListPtr argList(new NodeList());
	NodeListPtr args(new NodeList());
	for (unsigned int i = 0; i < getNode()->GetChildCount(); i ++)
	{
		NodePtr child = getNode()->GetChild(i);
		ElementPtr element = child->GetElement();
		if (element && nullptr != boost::dynamic_pointer_cast<Variable>(element))
		{
			argList->push_back(child);
		}
	}
	return argList;
}

const std::string & FunctionOverload::GetCodeBlock() const
{
	return codeBlock;
}

const char * FunctionOverload::GetTypeName() const
{
	return "FunctionOverload";
}

bool FunctionOverload::HasCodeBlock() const
{
	return (!!codeSource);
}

void intrusive_ptr_add_ref(FunctionOverload * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(FunctionOverload * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

bool FunctionOverload::RequiresCppFile() const
{
	return HasCodeBlock() && !(
		IsInline() || IsPureVirtual() || UsesDefault() || IsDeleted()
	);
}

bool FunctionOverload::RequiresHFile() const
{
	return getAccess().VisibleInHeader();
}

void FunctionOverload::SetCodeBlock(std::string & code, SourcePtr startOfCode,
									bool codeBlockAddRedirect)
{
	setCodeDefinitionSource(startOfCode,
		"Cannot create a code block for function %s because one was "
		"already defined at %s.");
	codeBlock = code;
	this->codeBlockAddRedirect = codeBlockAddRedirect;
}

void FunctionOverload::SetDefault(SourcePtr startOfCode)
{
	setCodeDefinitionSource(startOfCode,
		"Cannot set function %s to use the default keyword because it already "
		"was defined at %s.");
	this->codeSource = startOfCode;
	this->usesDefaultKeyword = true;
}

void FunctionOverload::SetDelete(SourcePtr startOfCode)
{
	setCodeDefinitionSource(startOfCode,
		"Cannot set function %s to use the delete keyword because it already "
		"was defined at %s.");
	this->codeSource = startOfCode;
	this->usesDeleteKeyword = true;
}

void FunctionOverload::SetPureVirtual(SourcePtr startOfCode)
{
	setCodeDefinitionSource(startOfCode,
		"Cannot set function %s as pure virtual because it already "
		"was defined at %s.");
	this->codeSource = startOfCode;
	this->isPureVirtual = true;
}

void FunctionOverload::setCodeDefinitionSource(SourcePtr startOfCode,
	                                           const char * const msg)
{
	if (!!codeSource)
	{
		std::string text = str(boost::format(msg)
			% this->getNode()->GetFullName() % codeSource->ToString());
		throw ModelInconsistencyException(startOfCode, text);
	}
	this->codeSource = startOfCode;
}

END_NAMESPACE

#endif
