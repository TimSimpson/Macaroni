#ifndef MACARONI_MODEL_MEMBER_H
#define MACARONI_MODEL_MEMBER_H

#include "../ME.h"

BEGIN_NAMESPACE2(Macaroni, Model)

class Member
{
	/** Returns true if a Member of this type can be legally nested under the
	 * type of the given member. */
	bool CanBeChildOf(const Member * other) const;

	ReasonPtr GetReasonCreated();
	
	virtual const std::string & GetTypeName() const = 0;
protected:
	Member();

	/** This is called from the public method. */
	virtual bool canBeChildOf(const Member * other) const = 0;

	virtual construct() = 0;

	void setNode(Node * node);

	void setReasonCreation(const ReasonPtr reason);

private:
	Node * node;

	const ReasonPtr reasonCreated;
};

END_NAMESPACE2

#endif

