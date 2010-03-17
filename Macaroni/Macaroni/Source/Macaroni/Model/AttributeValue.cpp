#ifndef MACARONI_MODEL_ATTRIBUTEVALUE_CPP
#define MACARONI_MODEL_ATTRIBUTEVALUE_CPP

#include "AttributeTable.h"
#include "AttributeValue.h"
#include "AttributeValueTypeException.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

namespace 
{
	
}; // end anonymous namespace


AttributeValue::AttributeValue(const std::string & name, const bool value)
:name(name), value(value)
{
}
	
AttributeValue::AttributeValue(const std::string & name, const double value)
:name(name), value(value)
{
}

AttributeValue::AttributeValue(const std::string & name, NodePtr value)
:name(name), value(value)
{
}

AttributeValue::AttributeValue(const std::string & name, const std::string & value)
:name(name), value(value)
{
}

AttributeValue::AttributeValue(const std::string & name, AttributeTablePtr value)
:name(name), value(value)
{
}

AttributeValue::~AttributeValue()
{
}

AttributeValue::TypeCode AttributeValue::getTypeCode() const
{
	class typeCodeVisitor : public boost::static_visitor<AttributeValue::TypeCode>
	{
	public:
		AttributeValue::TypeCode operator()(bool b) const
		{
			return AttributeValue::Type_Bool;
		}
		AttributeValue::TypeCode operator()(double d) const
		{
			return AttributeValue::Type_Number;
		}
		AttributeValue::TypeCode operator()(const NodePtr & node) const
		{
			return AttributeValue::Type_Node;
		}
		AttributeValue::TypeCode operator()(const std::string & str) const
		{
			return AttributeValue::Type_String;
		}
		AttributeValue::TypeCode operator()(const AttributeTablePtr & table) const
		{
			return AttributeValue::Type_Table;
		}
	};

	return boost::apply_visitor(typeCodeVisitor(), value);	
}

std::string AttributeValue::GetTypeString() const
{
	class typeCodeVisitor : public boost::static_visitor<std::string>
	{
	public:
		std::string operator()(bool b) const
		{
			return std::string("bool");
		}
		std::string operator()(double d) const
		{
			return std::string("number");
		}
		std::string operator()(const NodePtr node) const
		{
			return std::string("node");
		}
		std::string operator()(const std::string & str) const
		{
			return std::string("string");
		}
		std::string operator()(const AttributeTablePtr & table) const
		{
			return std::string("table");
		}
	};

	return boost::apply_visitor(typeCodeVisitor(), value);	
}

bool AttributeValue::GetValueAsBool() const
{
	if (!IsBool())
	{
		std::stringstream ss;
		ss << "Attempted to access " << GetTypeString() 
			<< " attribute as a bool.";
		throw AttributeValueTypeException(ss.str());
	}
	return boost::get<bool>(value);
}

NodePtr AttributeValue::GetValueAsNode() const
{
	if (!IsNode())
	{
		std::stringstream ss;
		ss << "Attempted to access " << GetTypeString() 
			<< " attribute as a node.";
		throw AttributeValueTypeException(ss.str());
	}
	return boost::get<NodePtr>(value);
}

double AttributeValue::GetValueAsNumber() const
{
	if (!IsNumber())
	{
		std::stringstream ss;
		ss << "Attempted to access " << GetTypeString() 
			<< " attribute as a number.";
		throw AttributeValueTypeException(ss.str());
	}
	return boost::get<double>(value);
}

std::string AttributeValue::GetValueAsString() const
{
	if (!IsString())
	{
		std::stringstream ss;
		ss << "Attempted to access " << GetTypeString() 
			<< " attribute as a string.";
		throw AttributeValueTypeException(ss.str());
	}
	return boost::get<std::string>(value);
}

AttributeTablePtr AttributeValue::GetValueAsTable() const
{
	if (!IsTable())
	{
		std::stringstream ss;
		ss << "Attempted to access " << GetTypeString() 
			<< " attribute as a table.";
		throw AttributeValueTypeException(ss.str());
	}
	return boost::get<AttributeTablePtr>(value);
}


END_NAMESPACE2

#endif

