#ifndef MACARONI_MODEL_ATTRIBUTEVALUE_CPP
#define MACARONI_MODEL_ATTRIBUTEVALUE_CPP

#include "AttributeValue.h"

BEGIN_NAMESPACE2(Macaroni, Model)

AttributeValue::AttributeValue(const std::string & name, const bool value)
:name(name), type(Type_Bool), value(new bool(value))
{
}
	
AttributeValue::AttributeValue(const std::string & name, const double value)
:name(name), type(Type_String), value(new double(value))
{
}

AttributeValue::AttributeValue(const std::string & name, NodePtr node)
:name(name), type(Type_Node), value(new NodePtr(node))
{
}

AttributeValue::AttributeValue(const std::string & name, const std::string & value)
:name(name), type(Type_String), value(new std::string(name))
{
}

AttributeValue::~AttributeValue()
{
	delete value;	
}

END_NAMESPACE2

#endif

