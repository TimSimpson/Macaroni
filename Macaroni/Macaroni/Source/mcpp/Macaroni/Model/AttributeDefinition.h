#ifndef MACARONI_MODEL_ATTRIBUTEDEFINITION_H
#define MACARONI_MODEL_ATTRIBUTEDEFINITION_H

#include "../ME.h"
#include "AttributeDefinitionPtr.h"
#include "Member.h"
#include "NodePtr.h"
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

/**
 * Defines an Attribute.  This is used to mark that the Node is used to refer
 * to an Attribute.
 */
class AttributeDefinition :  public Member
{
friend void intrusive_ptr_add_ref(AttributeDefinition * p);
friend void intrusive_ptr_release(AttributeDefinition * p);

public:

	enum TypeCode
	{
		Type_Bool,
		Type_Node,
		Type_Number,
		Type_String,
		Type_Table
	};

	AttributeDefinition(Node * node, TypeCode type, const ReasonPtr & reasonCreated);
	
	~AttributeDefinition();

	virtual bool canBeChildOf(const Member * other) const;

	static AttributeDefinitionPtr Create(NodePtr home, TypeCode type, const ReasonPtr & reason);

	/** Defines the given Node to be an Attribute of the given type.
	 * Throws "ModelInconsistencyException" on failure. 
	 */
	static void Define(NodePtr node, TypeCode type, const ReasonPtr & reasonCreated);	

	const char * GetAttributeTypeName() const;

	inline TypeCode GetTypeCode() const
	{
		return type;
	}

	virtual const char * GetTypeName() const;

	virtual void Visit(MemberVisitor * visitor) const;
private:
	static const char * getAttributeTypeName(TypeCode type);

	TypeCode type;
};

END_NAMESPACE2

#endif

