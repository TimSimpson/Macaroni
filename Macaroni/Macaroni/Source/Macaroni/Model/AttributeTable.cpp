#ifndef MACARONI_MODEL_ATTRIBUTETABLE_CPP
#define MACARONI_MODEL_ATTRIBUTETABLE_CPP

#include "AttributeTable.h"
#include <boost/foreach.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

AttributeTable::AttributeTable()
{
} 
	
AttributeTable::~AttributeTable()
{
}

void AttributeTable::Add(AttributeValuePtr value) 
{
	values.push_back(value);
}

AttributeValuePtr AttributeTable::GetByName(const std::string & name) const
{
	BOOST_FOREACH(AttributeValuePtr ptr, values)
	{
		if (ptr->GetName() == name)
		{
			return ptr;
		}
	}
	return AttributeValuePtr();
}

AttributeValuePtr AttributeTable::Get(int index) const
{
	return values[index];
}

END_NAMESPACE2

#endif

