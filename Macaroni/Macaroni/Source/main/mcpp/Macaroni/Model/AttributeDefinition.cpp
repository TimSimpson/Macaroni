#ifndef MACARONI_MODEL_ATTRIBUTEDEFINITION_CPP
#define MACARONI_MODEL_ATTRIBUTEDEFINITION_CPP

#include "AttributeDefinition.h"
#include <Macaroni/Exception.h>
#include "Node.h"
#include "Member.h"
#include "MemberPtr.h"
#include "ModelInconsistencyException.h"
#include <sstream>

#define TYPE_NAME "AttributeDefinition"

BEGIN_NAMESPACE2(Macaroni, Model)

void intrusive_ptr_add_ref(AttributeDefinition * p)
{
	intrusive_ptr_add_ref((Member *)p);
}

void intrusive_ptr_release(AttributeDefinition * p)
{
	intrusive_ptr_release((Member *)p);
}

AttributeDefinition::AttributeDefinition(Node * node, TypeCode type, const ReasonPtr & reasonCreated)
:Member(node, TYPE_NAME, reasonCreated), type(type)
{
}
	
AttributeDefinition::~AttributeDefinition()
{
}

AttributeDefinitionPtr AttributeDefinition::Create(NodePtr home, TypeCode type, const ReasonPtr &  reason)
{
	return AttributeDefinitionPtr(new AttributeDefinition(home.get(), type, reason));
}

bool AttributeDefinition::canBeChildOf(const Member * other) const
{
	return true;
}

void AttributeDefinition::Define(NodePtr node, TypeCode type, const ReasonPtr & reason)
{
	MemberPtr member = node->GetMember();
	if (!!member)
	{
		AttributeDefinitionPtr existingDef = boost::dynamic_pointer_cast<AttributeDefinition>(member); 
		if (!existingDef)
		{
			std::stringstream ss;
			ss << "Attempt to define previously defined Node "
				<< node->GetFullName() 
				<< " as an Attribute.";
			throw ModelInconsistencyException(existingDef->GetReasonCreated(),
				reason,
				ss.str());
		}
		else
		{
			if (existingDef->GetTypeCode() == type)
			{
				// Phew! Old def is the same as this one.
				return;
			}
			std::stringstream ss;
			ss << "Attempt to redefine Attribute "
				<< node->GetFullName() 
				<< " with value type of " 
				<< existingDef->GetAttributeTypeName()
				<< " as value type " 
				<< getAttributeTypeName(type);
			throw ModelInconsistencyException(existingDef->GetReasonCreated(),
				reason,
				ss.str());
		}
	} // end !!member
	Create(node, type, reason); 

}

const char * AttributeDefinition::GetAttributeTypeName() const
{
	return getAttributeTypeName(type);
}

const char * AttributeDefinition::getAttributeTypeName(TypeCode type)
{
	switch(type)
	{
	case Type_Bool:
		return "bool";
	case Type_Node:
		return "node";
	case Type_Number:
		return "number";
	case Type_String:
		return "string";
	case Type_Table:
		return "table";
	default:
		return "unknown";
	};
}

const char * AttributeDefinition::GetTypeName() const
{
	return TYPE_NAME;
}

void AttributeDefinition::Visit(MemberVisitor * visitor) const
{
	//TODO: Visit should no longer be here! Arg, I say.
	return;
}

END_NAMESPACE2

#endif

