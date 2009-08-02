#ifndef MACARONI_MODEL_CPP_CLASS_CPP
#define MACARONI_MODEL_CPP_CLASS_CPP

#include "Class.h"
#include "../../Exception.h"
#include "../MemberVisitor.h"
#include "../Node.h"
#include "../Node.lh"
#include <memory>
#include "../ModelInconsistencyException.h"
#include <sstream>

using class Macaroni::Model::Node;
using Macaroni::Model::NodePtr;
using Macaroni::Model::NodeList;


BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Class::Class(Node * parent, NodeListPtr importedNodes, ReasonPtr reason)
:Scope(parent, "Class", reason), friends(new NodeList()), imports(importedNodes)
{
}

Class::~Class()
{
	
}

void Class::AddFriend(NodePtr node)
{
	friends->push_back(node);
}

bool Class::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Scope *>(other) != nullptr;
}

ClassPtr Class::Create(NodePtr parent, NodeListPtr importedNodes, ReasonPtr reason)
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
	return ClassPtr(new Class(parent.get(), importedNodes, reason));
}

bool Class::DoesDefinitionReference(NodePtr node) const
{
	return this->Member::DoesDefinitionReference(node);
}

NodeListPtr Class::GetFriendNodes() const
{
	return friends;
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
}


END_NAMESPACE

#endif
