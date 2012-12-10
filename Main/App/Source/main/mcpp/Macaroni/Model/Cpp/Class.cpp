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
#ifndef MACARONI_MODEL_CPP_CLASS_CPP
#define MACARONI_MODEL_CPP_CLASS_CPP

#include <boost/foreach.hpp>
#include "Class.h"
#include "Function.h"
#include <Macaroni/Exception.h>
#include <Macaroni/Model/MemberVisitor.h>
#include <Macaroni/Model/Node.h>
#include <Macaroni/Model/NodePtr.h>
#include <memory>
#include <Macaroni/Model/ModelInconsistencyException.h>
#include <sstream>
#include <Macaroni/Model/Project/Target.h>
#include <Macaroni/Model/Project/TargetPtr.h>
#include <Macaroni/Model/Cpp/Variable.h>

using Macaroni::Model::Element;
using Macaroni::Model::ElementPtr;
using Macaroni::Model::Cpp::Function;
using Macaroni::Model::Cpp::FunctionPtr;
using Macaroni::Model::NodeList;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;
using Macaroni::Model::NodeList;
using Macaroni::Model::Cpp::Variable;
using Macaroni::Model::Cpp::VariablePtr;
using Macaroni::Model::Project::Target;
using Macaroni::Model::Project::TargetPtr;


BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Class::Class(Library * library, Node * parent,
			 Access access,
			 NodeListPtr importedNodes, ReasonPtr reason)
:Scope(library, parent, "Class", reason),
 access(access),
 friends(new NodeList()),
 globals(new NodeList()),
 imports(importedNodes),
 parents(new ClassParentList())
{
}

Class::Class(Target * target, Node * parent,
			 Access access,
			 NodeListPtr importedNodes, ReasonPtr reason)
:Scope(target, parent, "Class", reason),
 access(access),
 friends(new NodeList()),
 globals(new NodeList()),
 imports(importedNodes),
 parents(new ClassParentList())
{
}

Class::~Class()
{

}

AccessPtr Class::GetAccess() const
{
	AccessPtr aPtr(new Access(this->access));
	return aPtr;
}

void Class::AddParent(TypePtr parent, AccessPtr access, bool _virtual)
{
	Class * const me = this;
	parents->push_back(ClassParent(me, parent, *access, _virtual));
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

ClassPtr Class::Create(LibraryPtr library, NodePtr parent, AccessPtr access,
					   NodeListPtr importedNodes, ReasonPtr reason)
{
	ElementPtr existingMember = parent->GetElement();
	if (!!existingMember)
	{
		ClassPtr existingClass = boost::dynamic_pointer_cast<Class>(existingMember);
		if (!existingClass)
		{
			std::stringstream ss;
			ss << "The member at node " << parent->GetFullName()
				<< " is a " << parent->GetElement()->GetTypeName()
				<< " and cannot be morphed into a class.";
			throw Model::ModelInconsistencyException(existingMember->GetReasonCreated(),
				reason, ss.str());
		}
		return existingClass;
	}
	ClassPtr other = boost::dynamic_pointer_cast<Class>(existingMember);
	return ClassPtr(new Class(library.get(), parent.get(), *access, importedNodes, reason));
}

ClassPtr Class::Create(TargetPtr target, NodePtr parent, AccessPtr access,
					   NodeListPtr importedNodes, ReasonPtr reason)
{
	ElementPtr existingMember = parent->GetElement();
	if (!!existingMember)
	{
		ClassPtr existingClass = boost::dynamic_pointer_cast<Class>(existingMember);
		if (!existingClass)
		{
			std::stringstream ss;
			ss << "The member at node " << parent->GetFullName()
				<< " is a " << parent->GetElement()->GetTypeName()
				<< " and cannot be morphed into a class.";
			throw Model::ModelInconsistencyException(existingMember->GetReasonCreated(),
				reason, ss.str());
		}
		return existingClass;
	}
	ClassPtr other = boost::dynamic_pointer_cast<Class>(existingMember);
	ClassPtr ptr(new Class(target.get(), parent.get(), *access, importedNodes, reason));
	return ptr;
}
bool Class::DoesDefinitionReference(NodePtr node) const
{
	return this->Member::DoesDefinitionReference(node);
}

ClassParentListPtr Class::GetParents() const
{
	return parents;
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

bool Class::IsInstance(ElementPtr other)
{
	if (!other)
	{
		return false;
	}
	ClassPtr classPtr = boost::dynamic_pointer_cast<Class>(other);
	return (!!classPtr);
}

bool Class::RequiresCppFile() const
{
	Node & node = *GetNode();
	for (size_t i = 0; i < node.GetChildCount(); i ++)
	{
		Node & child = *(node.GetChild(i));
		if (child.GetElement()->RequiresCppFile())
		{
			return true;
		}
	}

	NodeListPtr globals = this->GetGlobalNodes();
	BOOST_FOREACH(NodePtr & node, *globals)
	{
		if (node->GetElement()->RequiresCppFile())
		{
			return true;
		}
	}
	return false;
}

bool Class::RequiresHFile() const
{
	return getAccess().VisibleInHeader();
}

void Class::Visit(MemberVisitor * visitor) const
{
	//TODO:
	// std::auto_ptr<MemberVisitor> classVisitorDeleter(visitor->VisitClass(*this));
	// MemberVisitor * classVisitor = classVisitorDeleter.get();
	// if (classVisitor == visitor)
	// {
	// 	classVisitorDeleter.release(); // do not let auto_ptr destroy.
	// }

	// for(unsigned int i = 0; i < this->GetNode()->GetChildCount(); i ++)
	// {
	// 	NodePtr child = GetNode()->GetChild(i);
	// 	if (!!child->GetElement())
	// 	{
	// 		MemberPtr memberPtr = boost::dynamic_pointer_cast<Member>(child);
	// 		memberPtr->Visit(classVisitor);
	// 	}
	// }

	// classVisitor->VisitClassFooter();

	// for(unsigned int i = 0; i < this->globals->size(); i ++)
	// {
	// 	NodePtr globalNode = (*(this->globals))[i];
	// 	FunctionPtr fPtr = globalNode->GetElement<FunctionPtr>();
	// 	VariablePtr vPtr = globalNode->GetElement<VariablePtr>();
	// 	if (!!fPtr)
	// 	{
	// 		Function & func = *(fPtr.get());
	// 		classVisitor->VisitAdoptedFunction(func);
	// 	}
	// 	else if (!!vPtr)
	// 	{
	// 		Variable & var = *(vPtr.get());
	// 		classVisitor->VisitAdoptedVariable(var);
	// 	}
	//}
}


END_NAMESPACE

#endif
