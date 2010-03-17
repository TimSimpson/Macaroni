#ifndef MACARONI_MODEL_ATTRIBUTEVALUE_H
#define MACARONI_MODEL_ATTRIBUTEVALUE_H

#include "../ME.h"
#include "AttributeTablePtr.h"
#include "NodePtr.h"
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

class AttributeValue;

typedef boost::shared_ptr<AttributeValue> AttributeValuePtr;

class AttributeValue
{
public:

	enum TypeCode
	{
		Type_Bool,
		Type_Node,
		Type_Number,
		Type_String,
		Type_Table
	};

	AttributeValue(const std::string & name, const bool value);
	
	AttributeValue(const std::string & name, const double value);

	AttributeValue(const std::string & name, NodePtr node);

	AttributeValue(const std::string & name, const std::string & value);

	AttributeValue(const std::string & name, AttributeTablePtr value);

	~AttributeValue();

	inline const std::string & GetName() const
	{
		return name;
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
	
	typedef boost::variant<bool, const NodePtr, double, const std::string, const AttributeTablePtr> Type;
			
	const std::string name;
	Type value;

	TypeCode getTypeCode() const;

	std::string getTypeCodeString() const;
};

END_NAMESPACE2

#endif

