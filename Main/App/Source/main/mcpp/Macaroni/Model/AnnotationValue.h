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
#ifndef MACARONI_MODEL_ATTRIBUTEVALUE_H
#define MACARONI_MODEL_ATTRIBUTEVALUE_H

#include "../ME.h"
#include "AnnotationTablePtr.h"
#include "AnnotationValuePtr.h"
#include "NodePtr.h"
#include <boost/shared_ptr.hpp>
#include <Macaroni/Model/Reason.h>
#include <Macaroni/Model/ReasonPtr.h>
#include <boost/variant.hpp>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

class AnnotationValue
{
friend void intrusive_ptr_add_ref(AnnotationValue * p);
friend void intrusive_ptr_release(AnnotationValue * p);
public:

	enum TypeCode
	{
		Type_Bool,
		Type_Node,
		Type_Number,
		Type_String,
		Type_Table
	};

	AnnotationValue(NodePtr name, const bool value, const ReasonPtr & reason);
	
	AnnotationValue(NodePtr name, const double value, const ReasonPtr & reason);

	AnnotationValue(NodePtr name, NodePtr node, const ReasonPtr & reason);

	AnnotationValue(NodePtr name, const std::string & value, const ReasonPtr & reason);

	//TODO: Review later if its unclear that this constructs a table.
	AnnotationValue(NodePtr name, const ReasonPtr & reason);

	~AnnotationValue();

	inline NodePtr GetName() const
	{
		return NodePtr(&name);
	}

	std::string GetTypeString() const;

	bool GetValueAsBool() const;

	NodePtr GetValueAsNode() const;

	double GetValueAsNumber() const;

	std::string GetValueAsString() const;

	AnnotationTablePtr GetValueAsTable() const;

	inline bool IsBool() const
	{
		return getTypeCode() == Type_Bool;
	}

	inline bool IsNode() const
	{
		return getTypeCode() == Type_Node;
	}	

	inline bool IsNumber() const
	{
		return getTypeCode() == Type_Number;
	}

	inline bool IsString() const
	{
		return getTypeCode() == Type_String;
	}

	inline bool IsTable() const
	{
		return getTypeCode() == Type_Table;
	}

private: 
	
	typedef boost::variant<bool, Node &, double, const std::string, AnnotationTableInternalPtr> Type;
			
	Node & name;
	Type value;

	TypeCode getTypeCode() const;

	std::string getTypeCodeString() const;
};

END_NAMESPACE2

#endif

