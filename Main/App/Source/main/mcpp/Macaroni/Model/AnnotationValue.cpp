#ifndef MACARONI_MODEL_ATTRIBUTEVALUE_CPP
#define MACARONI_MODEL_ATTRIBUTEVALUE_CPP

#include "AnnotationDefinition.h"
#include "AnnotationTable.h"
#include "AnnotationValue.h"
#include "AnnotationValueTypeException.h"
#include "Node.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

namespace 
{
	Node & nodePtrToRef(NodePtr node)
	{
		if (!node)
		{
			throw Macaroni::Exception("Attempted to assign AnnotationValue a name which was a null node.");
		}
		return *(node.get());
	}

	Node & nodePtrToRefValue(NodePtr node)
	{
		if (!node)
		{
			throw Macaroni::Exception("Attempted to assign AnnotationValue a node value which was null.");
		}
		return *(node.get());
	}
}; // end anonymous namespace


AnnotationValue::AnnotationValue(NodePtr name, const bool value, const ReasonPtr & reason)
:name(nodePtrToRef(name)), value(value)
{
	AnnotationDefinition::Define(name, AnnotationDefinition::Type_Bool, reason);
}
	
AnnotationValue::AnnotationValue(NodePtr name, const double value, const ReasonPtr & reason)
:name(nodePtrToRef(name)), value(value)
{
	AnnotationDefinition::Define(name, AnnotationDefinition::Type_Number, reason);
}

AnnotationValue::AnnotationValue(NodePtr name, NodePtr value, const ReasonPtr & reason)
:name(nodePtrToRef(name)), value(nodePtrToRefValue(value))
{
	AnnotationDefinition::Define(name, AnnotationDefinition::Type_Node, reason);
}

AnnotationValue::AnnotationValue(NodePtr name, const std::string & value, const ReasonPtr & reason)
:name(nodePtrToRef(name)), value(value)
{
	AnnotationDefinition::Define(name, AnnotationDefinition::Type_String, reason);
}

AnnotationValue::AnnotationValue(NodePtr name, const ReasonPtr & reason)
:name(nodePtrToRef(name)), value(AnnotationTableInternalPtr(new AnnotationTable(*(name.get()))))
{
	AnnotationDefinition::Define(name, AnnotationDefinition::Type_Table, reason);
}

AnnotationValue::~AnnotationValue()
{
}

AnnotationValue::TypeCode AnnotationValue::getTypeCode() const
{
	class typeCodeVisitor : public boost::static_visitor<AnnotationValue::TypeCode>
	{
	public:
		AnnotationValue::TypeCode operator()(bool b) const
		{
			return AnnotationValue::Type_Bool;
		}
		AnnotationValue::TypeCode operator()(double d) const
		{
			return AnnotationValue::Type_Number;
		}
		AnnotationValue::TypeCode operator()(const Node & node) const
		{
			return AnnotationValue::Type_Node;
		}
		AnnotationValue::TypeCode operator()(const std::string & str) const
		{
			return AnnotationValue::Type_String;
		}
		AnnotationValue::TypeCode operator()(const AnnotationTableInternalPtr & table) const
		{
			return AnnotationValue::Type_Table;
		}
	};

	return boost::apply_visitor(typeCodeVisitor(), value);	
}

std::string AnnotationValue::GetTypeString() const
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
		std::string operator()(const AnnotationTableInternalPtr & table) const
		{
			return std::string("table");
		}
	};

	return boost::apply_visitor(typeCodeVisitor(), value);	
}

bool AnnotationValue::GetValueAsBool() const
{
	if (!IsBool())
	{
		std::stringstream ss;
		ss << "Attempted to access " << GetTypeString() 
			<< " attribute as a bool.";
		throw AnnotationValueTypeException(ss.str());
	}
	return boost::get<bool>(value);
}

NodePtr AnnotationValue::GetValueAsNode() const
{
	if (!IsNode())
	{
		std::stringstream ss;
		ss << "Attempted to access " << GetTypeString() 
			<< " attribute as a node.";
		throw AnnotationValueTypeException(ss.str());
	}
	Node & node = boost::get<Node &>(value);
	return NodePtr(&node);
}

double AnnotationValue::GetValueAsNumber() const
{
	if (!IsNumber())
	{
		std::stringstream ss;
		ss << "Attempted to access " << GetTypeString() 
			<< " attribute as a number.";
		throw AnnotationValueTypeException(ss.str());
	}
	return boost::get<double>(value);
}

std::string AnnotationValue::GetValueAsString() const
{
	if (!IsString())
	{
		std::stringstream ss;
		ss << "Attempted to access " << GetTypeString() 
			<< " attribute as a string.";
		throw AnnotationValueTypeException(ss.str());
	}
	return boost::get<std::string>(value);
}

AnnotationTablePtr AnnotationValue::GetValueAsTable() const
{
	if (!IsTable())
	{
		std::stringstream ss;
		ss << "Attempted to access " << GetTypeString() 
			<< " attribute as a table.";
		throw AnnotationValueTypeException(ss.str());
	}
	AnnotationTableInternalPtr table = boost::get<AnnotationTableInternalPtr>(value);
	return AnnotationTablePtr(table.get());
}

void intrusive_ptr_add_ref(AnnotationValue * p)
{
	intrusive_ptr_add_ref(&(p->name));
}

void intrusive_ptr_release(AnnotationValue * p)
{
	intrusive_ptr_release(&(p->name));
}

END_NAMESPACE2

#endif

