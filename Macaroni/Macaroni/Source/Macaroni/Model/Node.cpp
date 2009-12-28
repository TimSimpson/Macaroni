#ifndef MACARONI_MODEL_NODE_CPP
#define MACARONI_MODEL_NODE_CPP

#include "Node.h"
#include "Context.h"
#include "../Exception.h"
#include "Member.h"
#include "ModelInconsistencyException.h"
#include "Reason.h"
#include <memory>
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

Node::Node(Node * scope, const std::string & name)
: adoptedHome(), context(nullptr), 
  hFilePath(), hFilePathReason(),
  member(nullptr), name(name), scope(scope)
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

Node * Node::createNode(const std::string & simpleName)
{
	MACARONI_ASSERT(IsSimpleName(simpleName), "Name must be simple at this point.");
	Node * child = new Node(this, simpleName);
	children.push_back(child);
	return child;
}

NodePtr Node::Find(const std::string & name)
{
	return NodePtr(findComplexName(name));
}

Node * Node::findComplexName(const std::string & name) const
{
	std::vector<std::string> subNames;
	SplitComplexName(name, subNames);
	if (subNames.size() < 0)
	{
		return nullptr;
	}
	Node * currentNode = this->findSimpleName(subNames[0]);
	for(unsigned int i = 1; i < subNames.size() && currentNode != nullptr; i ++)
	{
		currentNode = currentNode->findSimpleName(subNames[i]);
	}
	return currentNode;
}

Node * Node::findSimpleName(const std::string & name) const
{
	MACARONI_ASSERT(IsSimpleName(name), "Name must be simple at this point.");
	for(size_t i = 0; i < GetChildCount(); i ++)
	{
		if (getChild(i)->GetName() == name)
		{
			return getChild(i);
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

NodePtr Node::FindOrCreate(const std::string & name)
{
	Node * nakedPtr = findOrCreate(name, std::string(""));
	return NodePtr(nakedPtr);
}

NodePtr Node::FindOrCreate(const std::string & name, const std::string & hFilePath)
{
	Node * nakedPtr = findOrCreate(name, hFilePath);
	return NodePtr(nakedPtr);
}

Node * Node::findOrCreate(const std::string & name, const std::string & hFilePath)
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

	Node * member = this->findSimpleName(firstPart);
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
		s = createNode(firstPart);
	}

	if (lastPart.size() < 1)
	{
		//MACARONI_ASSERT(s->GetHFilePath() != hFilePath, "Attempt to create node with different hFile data.");
		return s;
	}
	else
	{
		return s->findOrCreate(lastPart, hFilePath);
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


NodePtr Node::GetAdoptedHome()
{
	return adoptedHome;
}

std::string Node::GetFullName() const
{
	std::stringstream ss;
	if (this->scope != nullptr && this->scope->IsNameVisible())
	{
		ss << this->scope->GetFullName();
		ss << "::";		
	}	// do not include root node.
	ss << this->name;
	return ss.str();
}

std::string Node::GetPrettyFullName(const char * seperator) const
{
	std::stringstream ss;
	if (this->scope != nullptr && this->scope->IsNameVisible())
	{
		ss << this->scope->GetPrettyFullName(seperator);
		ss << seperator;		
	}	// do not include root node.
	if (this->IsNameVisible())
	{
		ss << this->name;
	}
	return ss.str();
}

size_t Node::GetChildCount() const
{
	return children.size();
}

FileNamePtr Node::GetHFilePath() const
{
	return hFilePath;
}

//FileName Node::GetHFilePath() const
//{
//	if (hFilePath.size() < 1)
//	{
//		std::stringstream ss;
//		ss << "<" << GetPrettyFullName("/") << ".h>";
//		return ss.str();
//	}
//	return hFilePath;
//}

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

bool Node::IsNameVisible() const
{
	return !IsRoot() && name.find("{", 0) == std::string::npos;
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
		resultNode = searchRoot->FindOrCreate(additionalNodeName);
	}
}

void Node::SetAdoptedHome(NodePtr node)
{
	this->adoptedHome = node;
}

void Node::SetHFilePath(ReasonPtr why, FileNamePtr file)
{
	if (!!this->hFilePath)
	{
		throw ModelInconsistencyException(this->hFilePathReason, why, "HFilePath already set.");
	}
	this->hFilePath = file;
	this->hFilePathReason = why;
}

void Node::setMember(Member * value, const char * typeName, const ReasonPtr reasonCreated)
{
	if (this->member != nullptr)
	{
		std::stringstream ss;
		ss << "Member for node " << GetFullName() 
		   << " is already a(n) " << member->GetTypeName() << " and cannot "
		   "morph into a(n) " << typeName << ".";
		throw ModelInconsistencyException(member->GetReasonCreated(),
											  reasonCreated,
											  ss.str());	
	}
	this->member = value;
	this->member->node = this;
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
			subNames.push_back(complexName.substr(currentPos, index - currentPos));
			currentPos = index + 2;
		}
	}
}

END_NAMESPACE2

#endif
