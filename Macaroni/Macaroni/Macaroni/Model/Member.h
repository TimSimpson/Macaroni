#ifndef MACARONI_MODEL_MEMBER_H
#define MACARONI_MODEL_MEMBER_H

#include "../ME.h"
#include "Node.lh"
#include "Reason.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

class Member
{
friend class Node;
friend void intrusive_ptr_add_ref(Member * p);
friend void intrusive_ptr_release(Member * p);
public:

	/** Returns true if a Member of this type can be legally nested under the
	 * type of the given member. */
	bool CanBeChildOf(const Member * other) const;

	NodePtr GetNode() const;

	ReasonPtr GetReasonCreated() const;
	
	virtual const char * GetTypeName() const = 0;

protected:
	Member(Node * node, const char * typeName, const ReasonPtr reasonCreated);

	// Check all children to see if they can legally be children of this object.
	void assertChildrenAreLegal();

	// Subclasses should call this to ensure everything happened as intended.
	void assertValidConstruction();

	/** This is called from the public method. */
	virtual bool canBeChildOf(const Member * other) const = 0;

	Node * getNode() const;

	//virtual construct() = 0;

	//void setNode(Node * node);

	void setReasonCreation(const ReasonPtr reason);

private:
	Node * node;

	ReasonPtr reasonCreated;
};

END_NAMESPACE2

#endif

