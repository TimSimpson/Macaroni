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
#ifndef MACARONI_MODEL_NODE_H
#define MACARONI_MODEL_NODE_H

#include "../ME.h"
#include "AnnotationValuePtr.h"
#include "AnnotationTable.h"
#include <Macaroni/Model/ContextPtr.h>
#include <Macaroni/Model/Element.h>
#include <Macaroni/Model/ElementPtr.h>
#include <Macaroni/Model/FileNamePtr.h>
#include "NodePtr.h"
#include "ReasonPtr.h"
#include <vector>


BEGIN_NAMESPACE2(Macaroni, Model)

class Namespace;


class Node
{
friend class Context;
friend class Element;
friend class Type;
friend void intrusive_ptr_add_ref(Node * p);
friend void intrusive_ptr_release(Node * p);

public:
	struct NodeRelativePosition
	{
		std::vector<Node *> a;
		std::vector<Node *> b;
		int commonIndex;
	};

	bool operator==(const Node & other) const;

	bool operator!=(const Node & other) const;

	/** Creates a node with the given prefix and some number
	 * attached to it. */
	NodePtr CreateNextInSequence(const std::string & prefix);

	///** Returns an existing class with the given name, or returns nullptr if
	// * a clash is discovered. */
	//ClassPtr FindOrCreateClass(const std::string & name);

	///** Find or creates a Namespace, or returns nullptr if a clash is found. */
	//NamespacePtr FindOrCreateNamespace(const std::string & name);

	/** Finds of creates an UnknownScopePtr, which is returned as a NodePtr. */
	NodePtr FindOrCreate(const std::string & name);

	/** Same as above, but also sets the hFilePath if creating a new node. */
	NodePtr FindOrCreate(const std::string & name, const std::string & hFilePath);

	/** Find or creates an UnknownScopePtr, or returns nullptr if a clash. */
	//UnknownScopePtr FindOrCreateUnknownScope(const std::string & name);

	NodePtr Find(const std::string & complexName);

	/** Finds the node which this node and "other" descend from, or returns
	 *  an empty NodePtr. */
	NodeRelativePosition FindSharedRoot(NodePtr other);

	//NamespacePtr Find(std::string & name);
	NodePtr GetAdoptedHome();

	inline AnnotationTable & GetAnnotations()
	{
		return annotations;
	}

	ContextPtr GetContext();

	// TODO: This is pitifully sloppy! Information on where the official C++
	// definition is should probably optionally be attached to its own object
	// instead of a string.  Please change this into a pointer to such an
	// object.
	/** This is set either by a Generator or by the Parser if a non-Macaroni
	 * element is being introduced. */
	FileNamePtr GetHFilePath() const;

	std::string GetFullName() const;

	/** Uses the provided char instead of "::". */
	std::string GetPrettyFullName(const char * seperator) const;

	size_t GetChildCount() const;

	const std::string & GetName() const;

	bool InRootNamespace() const;

	/** Returns true if this node exists inside of the given node.
	 *  Example: if this node is "std::vector" and other is "std" this is
	 *  true. */
	bool IsChildOf(const Node & other) const;

	/** If false, this node does not appear as the part of it's children's names. */
	bool IsNameVisible() const;

	/** Returns true if this Node represents an operator overload. */
	bool IsOperator() const;

	bool IsRoot() const;

	template<typename ElementType, typename Func>
	void IterateNodes(Func & func)
	{
		for (unsigned int i = 0; i < GetChildCount(); ++ i)
		{
			auto var = GetChild(i)->GetElementPtr<ElementType>();
			if (var)
			{
				func(*var);
			}
		}
	}

	inline NodePtr GetChild(int index) const
	{
		return NodePtr(children[index]);
	}

	/** Returns the operator string.  Throws if this Node is not an operator. */
	std::string GetOperatorName() const;

	/** Depth of the node, defined by distance from root. */
	int GetDepth() const;

	ElementPtr GetElement()
	{
		return ElementPtr(element);
	}

	/** Returns an element cast to some pointer type of subclass. If the
	 *  element is not of that type returns an empty pointer. */
	template<class PtrType> PtrType GetElement()
	{
		typedef typename PtrType::element_type UnderlyingType;
		ElementPtr element = GetElement();
		if (!element)
		{
			return PtrType();
		}
		PtrType rtnValue =
			boost::dynamic_pointer_cast<UnderlyingType>(element);
		if (!rtnValue)
		{
			return PtrType();
		}
		else
		{
			return rtnValue;
		}
	}

	Element * GetElementPtr()
	{
		return element;
	}

	// Returns a naked pointer to the element.
	template<class Type> Type * GetElementPtr()
	{
		Type * typePtr = dynamic_cast<Type *>(element);
		return typePtr;
	}


	// Old alias for GetParent.
	NodePtr GetNode() const;

	// The parent.
	NodePtr GetParent() const;

	Node * GetParentPtr() const;

	/** Returns true if the element of this node matches the given type.
	 *  The type-safe C++ alternative to GetTypeName. */
	template<typename T>
	bool HasElementOfType()
	{
		T * tPtr = dynamic_cast<T *>(getElement());
		return 0 != tPtr;
	}

	/** Allows you to check the type name of a Node's Member without having to
	 * first check if the Node has a member. If there is no Member, "" is
	 * returned. Useful for Lua methods. */
	const char * GetTypeName() const;

	static bool IsComplexName(const std::string & name);

	static bool IsSimpleName(const std::string & name);

	void ParseComplexName(const std::string & complexName,
				          NodePtr & resultNode,
				          std::string & resultSimpleName);

	// OBSOLETE
	static void ParseComplexName(NodePtr searchRoot, const std::string & complexName,
						         NodePtr & resultNode,
						         std::string & resultSimpleName);

	void SetAdoptedHome(NodePtr node);

	void SetHFilePath(ReasonPtr reason, FileNamePtr fileName);

	static void SplitFirstNameOffComplexName(const std::string & complexName,
											 std::string & firstPart,
											 std::string & lastPart);

	static void SplitNodeAndMemberName(const std::string & complexName,
										std::string & scopeName,
										std::string & memberName);

	static void SplitComplexName(const std::string & complexName,
		   						 std::vector<std::string> & subNames);

protected:
	Node(Node * scope, const std::string & name);
	Node(const Node & other);
	void operator=(const Node & other);
	~Node();

	void adoptFloatingNode(Node * node);

	Node * createNode(const std::string & simpleName);

	//Class * createClass(const std::string & simpleName);

	//Namespace * createNamespace(const std::string & simpleName);

	Node * findComplexName(const std::string & name) const;

	/** Given a complex name, iterates through all parts of the name using
	 * the preexisting Node objects or creating new UnknownScope's if needed.*/
	Node * findOrCreate(const std::string & complexName, const std::string & hFilePath);

	Node * findSimpleName(const std::string & name) const;

	inline Node * getChild(int index) const
	{
		return children[index];
	}

	Element * getElement()
	{
		return element;
	}

	Node * getNode() const;

	void setElement(Element * const member, const ReasonPtr reasonCreated);

private:

	// This constructor creates a Root node of a context, and is accessible
	// only by the Context class.
	Node(Context & context, const std::string & name);

	/** This refers not to where a node officially lives, but where it might
	 * be stored for organization purposes.  For example, if you have an
	 * overloaded operator it might be better to store it in the same H file
	 * as the class even though that's not really it's scope.  Can be null. */
	Node * adoptedHome;

	AnnotationTable annotations;

	std::vector<Node *> children;

	Context * context;

	Element * element;

	FileNamePtr hFilePath;

	ReasonPtr hFilePathReason;

	std::string name;

	//TO-DO: Rename to "parent"
	Node * scope;

};

END_NAMESPACE2

#endif
