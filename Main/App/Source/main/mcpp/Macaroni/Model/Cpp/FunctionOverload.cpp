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
#include <Macaroni/Model/ModelInconsistencyException.h>
#include <Macaroni/Model/Reason.h>
#include <Macaroni/Model/Source.h>
#include <Macaroni/Model/Cpp/Variable.h>

using Macaroni::Model::ModelInconsistencyException;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

FunctionOverload::FunctionOverload
(
 Node * home, Model::ReasonPtr reason, bool isInline, 
 Access access, const bool isStatic, bool isVirtual, 
 const TypePtr rtnTypeInfo, bool constMember, bool throwSpecifier
)
:ScopeMember(home, "FunctionOverload", 
			 reason, access, isStatic),
 codeAttached(false),
 codeBlock(),
 constMember(constMember), 
 isInline(isInline),
 isVirtual(isVirtual),
 returnType(rtnTypeInfo),
 throwSpecifier(throwSpecifier)
{	
}

FunctionOverload::FunctionOverload
(
 Node * home, const char * typeName, Model::ReasonPtr reason, bool isInline, 
 Access access, const bool isStatic, bool isVirtual, 
 const TypePtr rtnTypeInfo, bool constMember, bool throwSpecifier
)
:ScopeMember(home, typeName, 
			 reason, access, isStatic),
 codeAttached(false),
 codeBlock(),
 constMember(constMember),
 isInline(isInline),
 isVirtual(isVirtual),
 returnType(rtnTypeInfo),
 throwSpecifier(throwSpecifier)
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
											 bool throwSpecifier,
											 Model::ReasonPtr reason)
{
	Function * fn = home.get();
	return Create(fn, isInline, access, isStatic, isVirtual, rtnType, 
		          constMember, throwSpecifier, reason);
}

FunctionOverloadPtr FunctionOverload::Create(Function * fn, 
											 bool isInline, const AccessPtr access, 
											 const bool isStatic, 
											 bool isVirtual,
											 const TypePtr rtnType, 
											 bool constMember, 
											 bool throwSpecifier,
											 Model::ReasonPtr reason)
{	
	Node * node = fn->GetNode().get();
	NodePtr foNode = node->CreateNextInSequence("Overload#");	
	return Create(foNode, isInline, access, isStatic, isVirtual, rtnType, 
		          constMember, throwSpecifier, reason);	
}

FunctionOverloadPtr FunctionOverload::Create(NodePtr foNode, 
											 bool isInline, const AccessPtr access, 
											 const bool isStatic, 
											 bool isVirtual,
											 const TypePtr rtnType, 
											 bool constMember, 
											 bool throwSpecifier,
											 Model::ReasonPtr reason)
{		
	FunctionOverload * fo = new FunctionOverload(foNode.get(), reason, 
		isInline, *access,
		isStatic, isVirtual, rtnType, constMember, throwSpecifier);
	return FunctionOverloadPtr(fo);
}

bool FunctionOverload::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Function *>(other) != nullptr;
}

bool FunctionOverload::DoesDefinitionReference(NodePtr node) const
{
	return this->returnType->GetNode() == node ? true
		: this->Member::DoesDefinitionReference(node);
}

NodeListPtr FunctionOverload::GetArguments() const
{
	NodeListPtr argList(new NodeList());	
	NodeListPtr args(new NodeList());
	for (unsigned int i = 0; i < getNode()->GetChildCount(); i ++)
	{
		NodePtr child = getNode()->GetChild(i);
		MACARONI_ASSERT(!!child->GetMember(), "Member for function argument set to null.");
		MemberPtr member = child->GetMember();
		MACARONI_ASSERT(boost::dynamic_pointer_cast<Variable>(member), 
					"Member was not of type variable - code is out of date.");
		argList->push_back(child);
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

void intrusive_ptr_add_ref(FunctionOverload * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(FunctionOverload * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

void FunctionOverload::SetCodeBlock(std::string & code, SourcePtr startOfCode)
{
	if (codeAttached)
	{
		std::stringstream msg;
		msg << "Cannot create a code block for function "
			<< this->getNode()->GetFullName() 
			<< " because one was already defined at "
			<< codeSource->ToString() 
			<< ".";
		throw ModelInconsistencyException(startOfCode, msg.str());
	}
	codeBlock = code;
	codeAttached = true;
	codeSource = startOfCode;
}

void FunctionOverload::Visit(MemberVisitor * visitor) const
{	
}

END_NAMESPACE

#endif
