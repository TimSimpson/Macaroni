#ifndef MACARONI_MODEL_ATTRIBUTEVALUE_H
#define MACARONI_MODEL_ATTRIBUTEVALUE_H

#include "../ME.h"
#include "AttributeTablePtr.h"
#include "AttributeValuePtr.h"
#include "NodePtr.h"
#include <boost/shared_ptr.hpp>
#include "Reason.h"
#include <boost/variant.hpp>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

class AttributeValue
{
friend void intrusive_ptr_add_ref(AttributeValue * p);
friend void intrusive_ptr_release(AttributeValue * p);
public:

	enum TypeCode
	{
		Type_Bool,
		Type_Node,
		Type_Number,
		Type_String,
		Type_Table
	};

	AttributeValue(NodePtr name, const bool value, const ReasonPtr & reason);
	
	AttributeValue(NodePtr name, const double value, const ReasonPtr & reason);

	AttributeValue(NodePtr name, NodePtr node, const ReasonPtr & reason);

	AttributeValue(NodePtr name, const std::string & value, const ReasonPtr & reason);

	//TODO: Review later if its unclear that this constructs a table.
	AttributeValue(NodePtr name, const ReasonPtr & reason);

	~AttributeValue();

	inline NodePtr GetName() const
	{
		return NodePtr(&name);
	}

	std::string GetTypeString() const;

	bool GetValueAsBool() const;

	NodePtr GetValueAsNode() const;

	double GetValueAsNumber() const;

	std::string GetValueAsString() const;

	AttributeTablePtr GetValueAsTable() const;

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
	
	typedef boost::variant<bool, Node &, double, const std::string, AttributeTableInternalPtr> Type;
			
	Node & name;
	Type value;

	TypeCode getTypeCode() const;

	std::string getTypeCodeString() const;
};

END_NAMESPACE2

#endif

