#ifndef MACARONI_MODEL_NODE_H
#define MACARONI_MODEL_NODE_H

#include "../ME.h"
#include "ContextPtr.h"
#include "FileNamePtr.h"
#include "Library.h"
#include "MemberPtr.h"
#include "NodePtr.h"
#include "ReasonPtr.h"
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Model)

class Namespace;

class Node
{ 
friend Context;
friend Member;
friend void intrusive_ptr_add_ref(Node * p);
friend void intrusive_ptr_release(Node * p);

public:		

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

	//NamespacePtr Find(std::string & name);	
	NodePtr GetAdoptedHome();

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

	virtual size_t GetChildCount() const;

	const std::string & GetName() const;

	/** If false, this node does not appear as the part of it's children's names. */
	bool IsNameVisible() const;

	bool IsRoot() const;

	inline NodePtr GetChild(int index) const
	{
		return NodePtr(children[index]);
	}

	MemberPtr GetMember()
	{
		return MemberPtr(member);
	}

	//TO-DO: Rename to "getParent" or something.
	NodePtr GetNode() const;

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

	Member * getMember() 
	{
		return member;
	}

	Node * getNode() const;

	FileNamePtr hFilePath;

	ReasonPtr hFilePathReason;

	void setMember(Member * member, const char * typeName, const ReasonPtr reasonCreated);

private:

	/** This refers not to where a node officially lives, but where it might
	 * be stored for organization purposes.  For example, if you have an 
	 * overloaded operator it might be better to store it in the same H file
	 * as the class even though that's not really it's scope.  Can be null. */
	NodePtr adoptedHome;

	std::vector<Node *> children;

	Context * context;

	Member * member;

	std::string name;

	//TO-DO: Rename to "parent"
	Node * scope;
	
};

END_NAMESPACE2

#endif
