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
#ifndef MACARONI_MODEL_NODE_CPP
#define MACARONI_MODEL_NODE_CPP

#include "Node.h"
#include <Macaroni/Model/Context.h>
#include <Macaroni/Exception.h>
#include <Macaroni/Model/FileName.h>
#include "Member.h"
#include "ModelInconsistencyException.h"
#include "Reason.h"
#include <memory>
#include <sstream>
#include <Macaroni/StringException.h>

BEGIN_NAMESPACE2(Macaroni, Model)

namespace
{

	static const char OPERATORS[][35] = {
		"new[]", "new", "delete[]", "delete",
		"[]", "->", "==", "!=", ">=", "<=", "&&", "||",
		"++", "--", "+=", "-=", "*=", "/=", "<<", ">>",
		"+", "-", "*", "/", "%", "^",
		"|", "&", "&", "~", "<", ">", "!", "=",
		0
	};

	const char * findOperatorName(const std::string & name)
	{
		if (name.find("operator", 0) != 0)
		{
			return nullptr;
		}
		std::string suffix = name.substr(8, name.length() - 8);
		for (int i = 0; OPERATORS[i] != nullptr; i ++)
		{
			const char * operatorName = OPERATORS[i];
			if (suffix == operatorName)
			{
				return operatorName;
			}
		}
		return nullptr;
	}

} // End anon namespace

Node::Node(Context & context, const std::string & name)
: adoptedHome(0),
annotations(context),
context(&context),
hFilePath(), hFilePathReason(),
element(nullptr), name(name), scope(nullptr)
{
}

Node::Node(Node * scope, const std::string & name)
: adoptedHome(0),
annotations(*(scope->context)),
context(scope->context),
hFilePath(), hFilePathReason(),
element(nullptr), name(name), scope(scope)
{

}

Node::~Node()
{
	for(size_t i = 0; i < children.size(); i ++)
	{
		delete children[i];
	}
}

bool Node::operator==(const Node & other) const
{
	return (this == &other); // || this->GetFullName() == other.GetFullName();
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

NodePtr Node::CreateNextInSequence(const std::string & prefix)
{
	MACARONI_ASSERT(IsSimpleName(prefix), "Prefix must be simple at this point.");
	int newIndex = 0;
	bool found = false;
	while(!found)
	{
		std::stringstream ss;
		ss << prefix << newIndex;
		std::string name = ss.str();
		Node * n = this->findSimpleName(name);
		if (n == nullptr)
		{
			Node * newNode = this->createNode(name);
			return NodePtr(newNode);
		}
		++ newIndex;
	}
	MACARONI_THROW("") // For compiler warning.
	NodePtr rtn;
	return rtn;
}

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
	const unsigned int index = name.find("::", 0);
	if (index == std::string::npos)
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
		throw Macaroni::StringException(ss.str().c_str());
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

ContextPtr Node::GetContext()
{
	return ContextPtr(context);
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

std::string Node::GetOperatorName() const
{
	const char * operatorName = findOperatorName(name);
	if (operatorName == nullptr)
	{
		std::stringstream ss;
		ss << "\"" << name << "\" is not an operator.";
		MACARONI_THROW(ss.str().c_str())
	}
	return operatorName;
}

const char * Node::GetTypeName() const
{
	if (this->element == nullptr)
	{
		return "";
	}
	return this->element->GetTypeName();
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

bool Node::IsOperator() const
{
	const char * operatorName = findOperatorName(name);
	return operatorName != nullptr;
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

	const unsigned int index = complexName.find_last_of("::");
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
	this->adoptedHome = node.get();
}

void Node::setElement(Element * const value, const ReasonPtr reasonCreated)
{
	if (this->element != nullptr)
	{
		std::stringstream ss;
		ss << "Member for node " << GetFullName()
			<< " is already a(n) " << element->GetTypeName() << " and cannot "
			"morph into a(n) " << this->element->GetTypeName() << ".";
		throw ModelInconsistencyException(element->GetReasonCreated(),
			reasonCreated,
			ss.str());
	}
	this->element = value;
	this->element->node = this;
}


void Node::SetHFilePath(ReasonPtr why, FileNamePtr file)
{
	if (!!this->hFilePath)
	{
		if (*file == *(this->hFilePath))
		{
			return; // Ignore if identical
		}
		throw ModelInconsistencyException(this->hFilePathReason, why, "HFilePath already set.");
	}
	this->hFilePath = file;
	this->hFilePathReason = why;
}

void Node::SplitFirstNameOffComplexName(const std::string & complexName,
										std::string & firstPart,
										std::string & lastPart)
{
	const unsigned int index = complexName.find_first_of("::");
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
	const unsigned int index = complexName.find_last_of("::");
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
		const unsigned int index = complexName.find_first_of("::", currentPos);
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
