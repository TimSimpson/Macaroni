#ifndef MACARONI_MODEL_NODE_CPP
#define MACARONI_MODEL_NODE_CPP

#include "Node.h"
#include "Class.h"
#include "Context.h"
#include "../Exception.h"
#include <memory>
#include "Namespace.h"
#include <sstream>
#include "UnknownScope.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Node::Node(Node * scope, const std::string & name)
:context(nullptr), name(name), scope(scope)
{
	if (scope != nullptr)
	{
		context = scope->context;
	}
}

Node::~Node()
{
	for(size_t i = 0; i < children.size(); i ++)
	{
		delete children[i];
	}
}

void Node::addScopeMember(ScopeMember * member)
{
	children.push_back(member);
}

//Class * Node::createClass(const std::string & simpleName)
//{	
//	std::auto_ptr<Class> newInstance(new Class(this, simpleName));
//	this->addScopeMember(newInstance.get());
//	return newInstance.release();
//}
//
//Namespace * Node::createNamespace(const std::string & simpleName)
//{
//	std::auto_ptr<Namespace> newInstance(new Namespace(this, simpleName));
//	this->addScopeMember(newInstance.get());
//	return newInstance.release();
//}

//Node * Node::createUnknownScope(const std::string & name)
//{
//	std::auto_ptr<UnknownScope> newNode(new UnknownScope(this, name));
//	addScopeMember(newNode.get());
//	return newNode.release();
//}

ScopeMemberPtr Node::Find(const std::string & name)
{
	ScopeMember * child = find(name);
	return ScopeMemberPtr(child);
}

ScopeMember * Node::find(const std::string & name) const
{
	for(size_t i = 0; i < GetMemberCount(); i ++)
	{
		if (getMember(i)->GetName() == name)
		{
			return getMember(i);
		}
	}
	return nullptr;
}

//ClassPtr Node::FindOrCreateClass(const std::string & name)
//{
//	NodePtr resultNode;
//	std::string resultSimpleName;
//	ParseComplexName(NodePtr(this), name, resultNode, resultSimpleName);
//	return ClassPtr(resultNode->createClass(resultSimpleName));
//}
//
//NamespacePtr Node::FindOrCreateNamespace(const std::string & name)
//{
//	NodePtr resultNode;
//	std::string resultSimpleName;
//	ParseComplexName(NodePtr(this), name, resultNode, resultSimpleName);
//	return NamespacePtr(resultNode->createNamespace(resultSimpleName));
//}

NodePtr Node::FindOrCreateNode(const std::string & name)
{
	Node * nakedPtr = findOrCreateNode(name);
	return NodePtr(nakedPtr);
}

Node * Node::findOrCreateNode(const std::string & name)
{
	std::string firstPart;
	std::string lastPart;
	int index;
	if ((index = name.find("::", 0)) == std::string::npos)
	{
		firstPart = name;
		lastPart = "";
	}
	else
	{
		firstPart = name.substr(0, index);
		lastPart = name.substr(index + 2);
	}

	ScopeMember * member = this->find(firstPart);
	Node * s = dynamic_cast<Node *>(member);
	if (member != nullptr && s == nullptr)
	{
		std::stringstream ss;
		ss << "While searching for name \"" << name
			<< "\" in Node \"" << GetFullName() << "\"-"
			<< "Can not create UnknownScope for name \""
			<< firstPart << "\"- found type which is ScopeMember, not Node.";
		throw new Macaroni::Exception(ss.str().c_str());
	}

	if (s == nullptr)
	{
		s = createUnknownScope(firstPart);
	}

	if (lastPart.size() < 1)
	{
		return s;
	}
	else
	{
		return s->findOrCreateNode(lastPart);
	}
}
//
//NodePtr Node::FindNode(const std::string & complexName)
//{
//	std::vector<std::string> names;
//	SplitComplexName(complexName, names);
//	for(unsigned int i = 0; i < names.size(); i ++)
//	{
//		Node = 
//	}
//}

std::string Node::GetFullName() const
{
	std::stringstream ss;
	if (this->scope != nullptr && !(this->scope->IsRoot()))
	{
		ss << this->scope->GetFullName();
		ss << "::";		
	}	// do not include root node.
	ss << this->name;
	return ss.str();
}

size_t Node::GetMemberCount() const
{
	return children.size();
}

const std::string & Node::GetName() const
{
	return name;
}

NodePtr Node::GetNode() const
{
	return NodePtr(scope);
}

Node * Node::getNode() const
{
	return scope;
}

void intrusive_ptr_add_ref(Node * p)
{
	intrusive_ptr_add_ref(p->context);
}

void intrusive_ptr_release(Node * p)
{
	intrusive_ptr_release(p->context);
}

bool Node::IsComplexName(const std::string & name)
{
	return (name.find("::", 0) != std::string::npos);
}

bool Node::IsSimpleName(const std::string & name)
{
	return (name.find("::", 0) == std::string::npos);
}

bool Node::IsRoot() const
{
	return getNode() == nullptr;
}

void Node::ParseComplexName(NodePtr searchRoot, const std::string & complexName,
							 NodePtr & resultNode,
							 std::string & resultSimpleName)
{
	MACARONI_ASSERT(searchRoot != false, "Root Node of search can not be null.");
	
	int index = complexName.find_last_of("::");
	if (index == std::string::npos) // Not found
	{
		resultNode = searchRoot;		
		resultSimpleName = complexName;
	}
	else
	{
		std::string additionalNodeName;
		SplitNodeAndMemberName(complexName, additionalNodeName, resultSimpleName);		
		resultNode = searchRoot->FindOrCreateNode(additionalNodeName);
	}
}

void Node::SplitFirstNameOffComplexName(const std::string & complexName,
									     std::string & firstPart,
										 std::string & lastPart)
{
	int index = complexName.find_first_of("::");
	if (index == std::string::npos) // Not found
	{
		lastPart = "";
		firstPart = complexName;
	}
	else
	{
		firstPart = complexName.substr(0, index);
		lastPart = complexName.substr(index + 2);
	}
}

void Node::SplitNodeAndMemberName(const std::string & complexName,
			  				 std::string & scopeName, std::string & memberName)
{
	int index = complexName.find_last_of("::");
	if (index == std::string::npos) // Not found
	{
		scopeName = "";
		memberName = complexName;
	}
	else
	{
		scopeName = complexName.substr(0, index - 1);
		memberName = complexName.substr(index + 1);
	}
}

void Node::SplitComplexName(const std::string & complexName,
			  				 std::vector<std::string> & subNames)
{
	MACARONI_ASSERT(complexName.length() < 16384, ":p < yeah right");
	unsigned int currentPos = 0;
	while(currentPos < complexName.length())
	{
		int index = complexName.find_first_of("::", currentPos);
		if (index == std::string::npos) // Not found
		{
			subNames.push_back(complexName.substr(currentPos));
			currentPos = complexName.length();
		}
		else
		{
			subNames.push_back(complexName.substr(0, index));
			currentPos = index + 2;
		}
	}
}

END_NAMESPACE2

#endif
