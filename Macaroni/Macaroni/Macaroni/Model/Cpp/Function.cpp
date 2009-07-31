#ifndef MACARONI_MODEL_CPP_FUNCTION_CPP
#define MACARONI_MODEL_CPP_FUNCTION_CPP

#include "Function.h"
#include "Primitive.h"
#include "../../Exception.h"
#include "../MemberVisitor.h"
#include "../ModelInconsistencyException.h"
#include "Namespace.h"
#include "../Node.h"
#include "../Reason.h"
#include "Scope.h"
#include "ScopeMember.h"
#include "Variable.h"
#include <memory>
#include <sstream>

using class Macaroni::Model::ModelInconsistencyException;
using class Macaroni::Model::Node;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)


Function::Function(Node * home, Model::ReasonPtr reason, const TypeInfo & rtnTypeInfo, bool constMember)
:ScopeMember(home, "Function", reason),
 codeAttached(false),
 codeBlock(),
 constMember(constMember),
 returnTypeInfo(rtnTypeInfo)
{
}

Function::Function(Node * home, const char * typeName, Model::ReasonPtr reason, const TypeInfo & rtnTypeInfo, bool constMember)
:ScopeMember(home, typeName, reason),
 codeAttached(false),
 codeBlock(),
 constMember(constMember),
 returnTypeInfo(rtnTypeInfo)
{
}

Function::~Function()
{
}

bool Function::DoesDefinitionReference(NodePtr node) const
{
	return this->returnTypeInfo.Node == node ? true
		: this->Member::DoesDefinitionReference(node);
}

VariablePtr Function::GetArgument(int index) const
{
	NodePtr child = getNode()->GetChild(index);
	MACARONI_ASSERT(!!child->GetMember(), "Member for function argument set to null.");
	MemberPtr member = child->GetMember();
	MACARONI_ASSERT(boost::dynamic_pointer_cast<Variable>(member), 
					"Member was not of type variable - code is out of date.");
	return boost::dynamic_pointer_cast<Variable>(member);
}

int Function::GetArgumentCount() const
{
	return getNode()->GetChildCount();
}

bool Function::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Scope *>(other) != nullptr;
}

FunctionPtr Function::Create(NodePtr host, const TypeInfo & rtnTypeInfo, 
							 bool constMember, Model::ReasonPtr reason)
{
	if (!host->GetMember())
	{
		return FunctionPtr(new Function(host.get(), reason, rtnTypeInfo, constMember));
	}
	Member * member = host->GetMember().get();
	Function * existingFunc = dynamic_cast<Function *>(member);
	if (existingFunc == nullptr)
	{
		// Will throw an error message.
		return FunctionPtr(new Function(host.get(), reason, rtnTypeInfo, constMember));
	}

	if (existingFunc != nullptr && !(existingFunc->returnTypeInfo == rtnTypeInfo))
	{
		std::stringstream ss;
		existingFunc->returnTypeInfo.DescribeDifferences(rtnTypeInfo, ss);

		ss << "Function was already defined with a conflicting return type. ";	
		existingFunc->returnTypeInfo.DescribeDifferences(rtnTypeInfo, ss);
		
		throw ModelInconsistencyException(member->GetReasonCreated(),
											  reason,
											  ss.str());	
	}
	// Re-use the previously set variable.
	return FunctionPtr(boost::dynamic_pointer_cast<Function>(host->GetMember()));
}

const std::string & Function::GetCodeBlock() const
{
	return codeBlock;
}

const char * Function::GetTypeName() const
{
	return "Function";
}

void intrusive_ptr_add_ref(Function * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Function * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

void Function::SetCodeBlock(std::string & code, SourcePtr startOfCode)
{
	if (codeAttached)
	{
		std::stringstream msg;
		msg << "Cannot create a code block for function "
			<< this->getNode()->GetFullName() 
			<< " because one was already defined at "
			<< codeSource->ToString() 
			<< ".";
		throw new ModelInconsistencyException(startOfCode, msg.str());
	}
	codeBlock = code;
	codeAttached = true;
	codeSource = startOfCode;
}

void Function::Visit(MemberVisitor * visitor) const
{
	visitor->VisitFunction(*this);
}
	
END_NAMESPACE

#endif
