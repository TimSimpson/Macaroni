#ifndef MACARONI_MODEL_ATTRIBUTEVALUE_H
#define MACARONI_MODEL_ATTRIBUTEVALUE_H

#include "../ME.h"
#include "NodePtr.h"
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

class AttributeValue
{

	private: enum Type
	{
		Type_Attribute,
		Type_Bool,
		Type_Node,
		Type_Number,
		Type_String
	};
	
	private: const std::string name;
	private: Type type;
	private: void * value;	
	
	public: AttributeValue(const std::string & name, const bool value);
	
	public: AttributeValue(const std::string & name, const double value);

	public: AttributeValue(const std::string & name, NodePtr node);

	public: AttributeValue(const std::string & name, const std::string & value);

	public: ~AttributeValue();

	public: inline const std::string & GetName() const
	{
		return name;
	}
	
	public : inline bool IsAttribute() const
	{
		return type == Type_Attribute;
	}

	public : inline bool IsBool() const
	{
		return type == Type_Bool;
	}

	public : inline bool IsNode() const
	{
		return type == Type_Node;
	}	

	public : inline bool IsNumber() const
	{
		return type == Type_Number;
	}

	public : inline bool IsString() const
	{
		return type == Type_String;
	}


	

};

END_NAMESPACE2

#endif

