#ifndef MACARONI_MODEL_ATTRIBUTEDEFINITION_H
#define MACARONI_MODEL_ATTRIBUTEDEFINITION_H

#include "../ME.h"
#include "AnnotationDefinitionPtr.h"
#include "Member.h"
#include "NodePtr.h"
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

/**
 * Defines an Annotation.  This is used to mark that the Node is used to refer
 * to an Annotation.
 */
class AnnotationDefinition :  public Member
{
friend void intrusive_ptr_add_ref(AnnotationDefinition * p);
friend void intrusive_ptr_release(AnnotationDefinition * p);

public:

	enum TypeCode
	{
		Type_Bool,
		Type_Node,
		Type_Number,
		Type_String,
		Type_Table
	};

	AnnotationDefinition(Node * node, TypeCode type, const ReasonPtr & reasonCreated);
	
	~AnnotationDefinition();

	virtual bool canBeChildOf(const Member * other) const;

	static AnnotationDefinitionPtr Create(NodePtr home, TypeCode type, const ReasonPtr & reason);

	/** Defines the given Node to be an Annotation of the given type.
	 * Throws "ModelInconsistencyException" on failure. 
	 */
	static void Define(NodePtr node, TypeCode type, const ReasonPtr & reasonCreated);	

	const char * GetAnnotationTypeName() const;

	inline TypeCode GetTypeCode() const
	{
		return type;
	}

	virtual const char * GetTypeName() const;

	virtual void Visit(MemberVisitor * visitor) const;
private:
	static const char * getAnnotationTypeName(TypeCode type);

	TypeCode type;
};

END_NAMESPACE2

#endif

