#ifndef MACARONI_MODEL_NODE_H
#define MACARONI_MODEL_NODE_H

#include "../ME.h"
#include <vector>
#include "Class.lh"
#include "Context.lh"
#include "Namespace.lh"
#include "Node.lh"
#include "ScopeMember.h"
#include "UnknownScope.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

class Namespace;

class Node
{
friend Context;
friend Namespace; // Only friends so Namespace can find the root namespace- may change this in the future.
friend void intrusive_ptr_add_ref(Node * p);
friend void intrusive_ptr_release(Node * p);

public:		

	ScopeMemberPtr Find(const std::string & name);	

	///** Returns an existing class with the given name, or returns nullptr if
	// * a clash is discovered. */
	//ClassPtr FindOrCreateClass(const std::string & name);

	///** Find or creates a Namespace, or returns nullptr if a clash is found. */
	//NamespacePtr FindOrCreateNamespace(const std::string & name);

	/** Finds of creates an UnknownScopePtr, which is returned as a NodePtr. */
	NodePtr FindOrCreateNode(const std::string & name);	

	/** Find or creates an UnknownScopePtr, or returns nullptr if a clash. */
	//UnknownScopePtr FindOrCreateUnknownScope(const std::string & name);

	NodePtr FindNode(const std::string & complexName);

	//NamespacePtr Find(std::string & name);	

	std::string GetFullName() const;

	virtual size_t GetMemberCount() const;

	const std::string & GetName() const;

	bool IsRoot() const;

	inline ScopeMemberPtr GetMember(int index) const
	{
		return ScopeMemberPtr(children[index]);
	}

	//TO-DO: Rename to "getParent" or something.
	NodePtr GetNode() const;

	static bool IsComplexName(const std::string & name);

	static bool IsSimpleName(const std::string & name);

	static void ParseComplexName(NodePtr searchRoot, const std::string & complexName,
						         NodePtr & resultNode,
						         std::string & resultSimpleName);

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

	void addScopeMember(ScopeMember * member);

	//Class * createClass(const std::string & simpleName);

	//Namespace * createNamespace(const std::string & simpleName);

	Node * createUnknownScope(const std::string & name);

	ScopeMember * find(const std::string & name) const;

	/** Given a complex name, iterates through all parts of the name using
	 * the preexisting Node objects or creating new UnknownScope's if needed.*/
	Node * findOrCreateNode(const std::string & complexName);

	inline ScopeMember * getMember(int index) const
	{
		return children[index];
	}

	Node * getNode() const;

private:
	std::vector<ScopeMember *> children;

	Context * context;

	std::string name;

	//TO-DO: Rename to "parent"
	Node * scope;
};

END_NAMESPACE2

#endif
