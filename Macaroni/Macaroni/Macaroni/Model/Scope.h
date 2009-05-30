#ifndef MACARONI_MODEL_SCOPE_H
#define MACARONI_MODEL_SCOPE_H

#include "../ME.h"
#include <vector>
#include "Class.lh"
#include "Context.lh"
#include "Namespace.lh"
#include "Scope.lh"
#include "ScopeMember.h"
#include "UnknownScope.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

class Namespace;

class Scope : public ScopeMember
{
friend Context;
friend Namespace; // Only friends so Namespace can find the root namespace- may change this in the future.
friend void intrusive_ptr_add_ref(Scope * p);
friend void intrusive_ptr_release(Scope * p);

public:		

	ScopeMemberPtr Find(const std::string & name);	

	/** Returns an existing class with the given name, or returns nullptr if
	 * a clash is discovered. */
	ClassPtr FindOrCreateClass(const std::string & name);

	/** Find or creates a Namespace, or returns nullptr if a clash is found. */
	NamespacePtr FindOrCreateNamespace(const std::string & name);

	/** Finds of creates an UnknownScopePtr, which is returned as a ScopePtr. */
	ScopePtr FindOrCreateScope(const std::string & name);	

	/** Find or creates an UnknownScopePtr, or returns nullptr if a clash. */
	UnknownScopePtr FindOrCreateUnknownScope(const std::string & name);

	//ScopePtr FindScope(const std::string & complexName);

	//NamespacePtr Find(std::string & name);	

	virtual size_t GetMemberCount() const;

	bool IsRoot() const;

	inline ScopeMemberPtr GetMember(int index) const
	{
		return ScopeMemberPtr(children[index]);
	}

	static bool IsComplexName(const std::string & name);

	static bool IsSimpleName(const std::string & name);

	static void ParseComplexName(ScopePtr searchRoot, const std::string & complexName,
						         ScopePtr & resultScope,
						         std::string & resultSimpleName);

	static void SplitFirstNameOffComplexName(const std::string & complexName,
											 std::string & firstPart,
											 std::string & lastPart);
																				 
	static void SplitScopeAndMemberName(const std::string & complexName,
										std::string & scopeName, 
										std::string & memberName);

	static void SplitComplexName(const std::string & complexName,
		   						 std::vector<std::string> & subNames);

protected:
	Scope(Scope * scope, const std::string & name);
	Scope(const Scope & other);
	void operator=(const Scope & other);
	~Scope();

	void addScopeMember(ScopeMember * member);

	Class * createClass(const std::string & simpleName);

	Namespace * createNamespace(const std::string & simpleName);

	Scope * createUnknownScope(const std::string & name);

	ScopeMember * find(const std::string & name) const;

	/** Given a complex name, iterates through all parts of the name using
	 * the preexisting Scope objects or creating new UnknownScope's if needed.*/
	Scope * findOrCreateScope(const std::string & complexName);

	inline ScopeMember * getMember(int index) const
	{
		return children[index];
	}

private:
	std::vector<ScopeMember *> children;

	Context * context;
};

END_NAMESPACE2

#endif
