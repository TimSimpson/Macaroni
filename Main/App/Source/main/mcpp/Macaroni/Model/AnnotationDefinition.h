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

private:
	static const char * getAnnotationTypeName(TypeCode type);

	TypeCode type;
};

END_NAMESPACE2

#endif

