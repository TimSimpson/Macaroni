#ifndef MACARONI_MODEL_CPP_CLASS_CPP
#define MACARONI_MODEL_CPP_CLASS_CPP

#include "Class.h"
#include "Function.h"
#include "../../Exception.h"
#include "../MemberVisitor.h"
#include "../Node.h"
#include "../NodePtr.h"
#include <memory>
#include "../ModelInconsistencyException.h"
#include <sstream>
#include "Variable.h"

using Macaroni::Model::Cpp::Function;
using Macaroni::Model::Cpp::FunctionPtr;
using Macaroni::Model::NodeList;
using class Macaroni::Model::Node;
using Macaroni::Model::NodePtr;
using Macaroni::Model::NodeList;
using Macaroni::Model::Cpp::Variable;
using Macaroni::Model::Cpp::VariablePtr;


BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Class::Class(Library * library, Node * parent, NodeListPtr importedNodes, ReasonPtr reason)
:Scope(library, parent, "Class", reason), 
 friends(new NodeList()), 
 globals(new NodeList()), 
 imports(importedNodes)
{
}

Class::~Class()
{
	
}

void Class::AddParent(NodePtr parent, Access access, bool _virtual)
{
	Class * const me = this;
	parents.push_back(ClassParent(me, parent.get(), access, _virtual));
}

void Class::AddFriend(NodePtr node)
{
	friends->push_back(node);
}

void Class::AddGlobal(NodePtr node)
{
	globals->push_back(node);
	//friends->push_back(node);
}

bool Class::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Scope *>(other) != nullptr;
}

ClassPtr Class::Create(LibraryPtr library, NodePtr parent, NodeListPtr importedNodes, ReasonPtr reason)
{
	MemberPtr existingMember = parent->GetMember();
	if (!!existingMember)
	{
		ClassPtr existingClass = boost::dynamic_pointer_cast<Class>(existingMember);
		if (!existingClass)
		{
			std::stringstream ss;
			ss << "The member at node " << parent->GetFullName() 
				<< " is a " << parent->GetMember()->GetTypeName() 
				<< " and cannot be morphed into a class.";
			throw Model::ModelInconsistencyException(existingMember->GetReasonCreated(),
				reason, ss.str());
		}
		return existingClass;
	}
	ClassPtr other = boost::dynamic_pointer_cast<Class>(existingMember);
	return ClassPtr(new Class(library.get(), parent.get(), importedNodes, reason));
}

bool Class::DoesDefinitionReference(NodePtr node) const
{
	return this->Member::DoesDefinitionReference(node);
}

NodeListPtr Class::GetFriendNodes() const
{
	return friends;
}

NodeListPtr Class::GetGlobalNodes() const
{
	return globals;
}

NodeListPtr Class::GetImportedNodes() const
{
	return imports;
}

const char * Class::GetTypeName() const
{
	return "Class";
}

void intrusive_ptr_add_ref(Class * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Class * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}

bool Class::IsInstance(MemberPtr other)
{
	if (!other) 
	{
		return false;
	}
	ClassPtr classPtr = boost::dynamic_pointer_cast<Class>(other);
	return (!!classPtr);	
}

void Class::Visit(MemberVisitor * visitor) const
{
	std::auto_ptr<MemberVisitor> classVisitorDeleter(visitor->VisitClass(*this));
	MemberVisitor * classVisitor = classVisitorDeleter.get();
	if (classVisitor == visitor)
	{
		classVisitorDeleter.release(); // do not let auto_ptr destroy.
	}

	for(unsigned int i = 0; i < this->GetNode()->GetChildCount(); i ++)
	{
		NodePtr child = GetNode()->GetChild(i);
		if (child->GetMember() != nullptr)
		{
			child->GetMember()->Visit(classVisitor);
		}
	}

	classVisitor->VisitClassFooter();

	for(unsigned int i = 0; i < this->globals->size(); i ++)
	{
		NodePtr globalNode = (*(this->globals))[i];
		MemberPtr child = globalNode->GetMember();
		if (!!child)
		{
			FunctionPtr fPtr = boost::dynamic_pointer_cast<Function>(child);
			VariablePtr vPtr = boost::dynamic_pointer_cast<Variable>(child);
			if (!!fPtr)
			{
				Function & func = *(fPtr.get());
				classVisitor->VisitAdoptedFunction(func);
			}
			else if (!!vPtr)
			{
				Variable & var = *(vPtr.get());
				classVisitor->VisitAdoptedVariable(var);
			}

		}
	}
}


END_NAMESPACE

#endif
