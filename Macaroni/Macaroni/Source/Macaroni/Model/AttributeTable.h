#ifndef MACARONI_MODEL_ATTRIBUTETABLE_H
#define MACARONI_MODEL_ATTRIBUTETABLE_H

#include "AttributeValue.h"
#include "AttributeTablePtr.h"

BEGIN_NAMESPACE2(Macaroni, Model)

class AttributeTable;

typedef boost::shared_ptr<AttributeTable> AttributeTablePtr;

class AttributeTable
{
public:
	typedef std::vector<AttributeValuePtr>::iterator iterator;
	typedef std::vector<AttributeValuePtr>::const_iterator const_iterator;	

	AttributeTable();
	
	~AttributeTable();

	const_iterator begin() { return values.begin(); }

	const_iterator end() { return values.end(); }

	void Add(AttributeValuePtr value);

	AttributeValuePtr GetByName(const std::string & name) const;

	AttributeValuePtr Get(int index) const;

private:
	std::vector<AttributeValuePtr> values;
};

END_NAMESPACE2

#endif

