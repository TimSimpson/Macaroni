#ifndef MACARONI_MODEL_CPP_CLASS_H
#define MACARONI_MODEL_CPP_CLASS_H

#include "../../ME.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Class;

END_NAMESPACE

#include "Access.h"
#include <Macaroni/Model/Cpp/ClassParent.h>
#include <Macaroni/Model/Cpp/_.h>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "../Library.h"
#include "../Member.h"
#include "ClassPtr.h"
#include "Scope.h"
 
BEGIN_NAMESPACE(Macaroni, Model, Cpp)


class Class : public Scope
{
friend void intrusive_ptr_add_ref(Class * p);
friend void intrusive_ptr_release(Class * p);
public:	
	static ClassPtr Create(LibraryPtr library, NodePtr home, Model::NodeListPtr importedNodes, ReasonPtr reason);

	virtual ~Class();
	
	void AddFriend(NodePtr node);
	
	void AddGlobal(NodePtr node);
	
	void AddParent(TypePtr parent, Access access, bool _virtual);

	virtual bool canBeChildOf(const Member * other) const;

	virtual bool DoesDefinitionReference(NodePtr node) const;
	
	/** Classes which this Class inherits from. */
	ClassParentListPtr GetParents() const;

	/** A list of Nodes who are "friends" of this class.  Friends with benefits,
	 * that is.  These nodes have intricate knowledge of this classes inner-
	 * most secrets. */
	NodeListPtr GetFriendNodes() const;

	/** A list of Nodes that may or may not have anything to do with this 
	 * class, but are adopted by this class.  They are usually defined in the
	 * same MCPP file and get stored in the same code files as the class.
	 * An example would be a global function thats callable from plain C but
	 * is useful only in the context of this class. */
	NodeListPtr GetGlobalNodes() const;

	/** Nodes imported by this class.  Always needed by the implementation file
	 *  of this class. */
	NodeListPtr GetImportedNodes() const;

	virtual const char * GetTypeName() const;
	
	/** Returns true if the given member is an instance of Class. */
	static bool IsInstance(Model::MemberPtr other);

	virtual void Visit(MemberVisitor * visitor) const;

protected:	
	Class(Library * library, Node * home, Model::NodeListPtr importedNodes, ReasonPtr reason);
private:

	// Awesome, all of these are memory leaks.  No wonder I used to get things
	// done quicker!
	NodeListPtr friends;

	NodeListPtr globals;

	NodeListPtr imports;
	
	Macaroni::Model::Cpp::ClassParentListPtr parents;
	
};



END_NAMESPACE

#endif
