#ifndef MACARONI_MODEL_ATTRIBUTEVALUE_CPP
#define MACARONI_MODEL_ATTRIBUTEVALUE_CPP

#include "AttributeDefinition.h"
#include "AttributeTable.h"
#include "AttributeValue.h"
#include "AttributeValueTypeException.h"
#include "Node.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

namespace 
{
	Node & nodePtrToRef(NodePtr node)
	{
		if (!node)
		{
			throw Macaroni::Exception("Attempted to assign AttributeValue a name which was a null node.");
		}
		return *(node.get());
	}

	Node & nodePtrToRefValue(NodePtr node)
	{
		if (!node)
		{
			throw Macaroni::Exception("Attempted to assign AttributeValue a node value which was null.");
		}
		return *(node.get());
	}
}; // end anonymous namespace


AttributeValue::AttributeValue(NodePtr name, const bool value, const ReasonPtr & reason)
:name(nodePtrToRef(name)), value(value)
{
	AttributeDefinition::Define(name, AttributeDefinition::Type_Bool, reason);
}
	
AttributeValue::AttributeValue(NodePtr name, const double value, const ReasonPtr & reason)
:name(nodePtrToRef(name)), value(value)
{
	AttributeDefinition::Define(name, AttributeDefinition::Type_Number, reason);
}

AttributeValue::AttributeValue(NodePtr name, NodePtr value, const ReasonPtr & reason)
:name(nodePtrToRef(name)), value(nodePtrToRefValue(value))
{
	AttributeDefinition::Define(name, AttributeDefinition::Type_Node, reason);
}

AttributeValue::AttributeValue(NodePtr name, const std::string & value, const ReasonPtr & reason)
:name(nodePtrToRef(name)), value(value)
{
	AttributeDefinition::Define(name, AttributeDefinition::Type_String, reason);
}

AttributeValue::AttributeValue(NodePtr name, const ReasonPtr & reason)
:name(nodePtrToRef(name)), value(AttributeTableInternalPtr(new AttributeTable(*(name.get()))))
{
	AttributeDefinition::Define(name, AttributeDefinition::Type_Table, reason);
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
		AttributeValue::TypeCode operator()(const Node & node) const
		{
			return AttributeValue::Type_Node;
		}
		AttributeValue::TypeCode operator()(const std::string & str) const
		{
			return AttributeValue::Type_String;
		}
		AttributeValue::TypeCode operator()(const AttributeTableInternalPtr & table) const
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
		std::string operator()(const Node & node) const
		{
			return std::string("node");
		}
		std::string operator()(const std::string & str) const
		{
			return std::string("string");
		}
		std::string operator()(const AttributeTableInternalPtr & table) const
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
	Node & node = boost::get<Node &>(value);
	return NodePtr(&node);
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
	AttributeTableInternalPtr table = boost::get<AttributeTableInternalPtr>(value);
	return AttributeTablePtr(table.get());
}

void intrusive_ptr_add_ref(AttributeValue * p)
{
	intrusive_ptr_add_ref(&(p->name));
}

void intrusive_ptr_release(AttributeValue * p)
{
	intrusive_ptr_release(&(p->name));
}

END_NAMESPACE2

#endif

