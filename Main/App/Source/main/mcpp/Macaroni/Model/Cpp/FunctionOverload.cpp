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

#include <Macaroni/Model/Cpp/Class.h>
#include <Macaroni/Model/Cpp/ClassPtr.h>
#include "FunctionOverload.h"
#include <boost/format.hpp>
#include <Macaroni/Model/ModelInconsistencyException.h>
#include <Macaroni/Model/Reason.h>
#include <Macaroni/Model/Source.h>
 #include <Macaroni/Model/Project/Target.h>
#include <Macaroni/Model/Cpp/Variable.h>

using Macaroni::Model::ModelInconsistencyException;
using Macaroni::Model::NakedNodeList;
using boost::optional;
using Macaroni::Model::Project::Target;
using Macaroni::Model::Project::TargetPtr;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

namespace
{
	bool isInTemplateClass(Node & node)
	{
		MACARONI_ASSERT(node.GetNode(), "Must have function Node as parent.");
		MACARONI_ASSERT(node.GetNode()->GetNode(),
			            "Function node must live somewhere.");
		ClassPtr cls = node.GetNode()->GetNode()->GetElement<ClassPtr>();
		return (cls && cls->GetTemplateParameterList());
	}
}


FunctionOverload::FunctionOverload
(
 Target * target,
 Node * home, Model::ReasonPtr reason, bool isInline,
 Access access, const bool isStatic, bool isVirtual,
 const TypePtr rtnTypeInfo, bool constMember,
 const optional<ExceptionSpecifier> exceptionSpecifier,
 Node * templateParameterList,
 optional<NodeListPtr> importedNodes=boost::none
)
:Scope(target, home, reason, access, isStatic),
 codeBlock(),
 codeBlockAddRedirect(false),
 constMember(constMember),
 codeSource(),
 imports(),
 isInline(isInline),
 isPureVirtual(false),
 isTemplateClassMethod(isInTemplateClass(*home)),
 isVirtual(isVirtual),
 returnType(rtnTypeInfo),
 exceptionSpecifier(exceptionSpecifier),
 templateParameterList(templateParameterList),
 usesDefaultKeyword(false),
 usesDeleteKeyword(false)
{
	if (importedNodes)
	{
		imports = NakedNodeList{};
		NodeListPtr & list  = importedNodes.get();
		for(NodePtr & n : *list)
		{
			(imports.get()).push_back(n.get());
		}
	}
}

FunctionOverload::~FunctionOverload()
{
}

FunctionOverloadPtr FunctionOverload::Create(
	TargetPtr target,
	NodePtr foNode,
	bool isInline, const AccessPtr access,
	const bool isStatic,
	bool isVirtual,
	const TypePtr rtnType,
	bool constMember,
	const optional<ExceptionSpecifier> exceptionSpecifier,
	Model::ReasonPtr reason,
	NodePtr templateParameterList,
	optional<Macaroni::Model::NodeListPtr> imports
)
{
	FunctionOverload * fo = new FunctionOverload(target.get(),
		foNode.get(), reason,
		isInline, *access,
		isStatic, isVirtual, rtnType, constMember, exceptionSpecifier,
		templateParameterList.get(), imports);
	return FunctionOverloadPtr(fo);
}

NodePtr FunctionOverload::CreateNewFunctionOverloadNode(FunctionPtr home)
{
	NodePtr foNode = home->GetNode()->CreateNextInSequence("Overload#");
	return foNode;
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

NodeListPtr FunctionOverload::GetImportedNodes() const
{
	if (imports)
	{
		NodeListPtr rtnList(new NodeList());
		const NakedNodeList & list = imports.get();
		for (::Macaroni::Model::Node * n : list)
		{
			rtnList->push_back(NodePtr(n));
		}
		return rtnList;
	}
	else
	{
		return NodeListPtr{};
	}
}

NodePtr FunctionOverload::GetTemplateParameterList()
{
	return templateParameterList;
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

bool FunctionOverload::IsInline() const
{
	return isInline || nullptr != templateParameterList
		|| isTemplateClassMethod;
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
