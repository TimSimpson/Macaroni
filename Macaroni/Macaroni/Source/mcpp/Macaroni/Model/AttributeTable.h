#ifndef MACARONI_MODEL_ATTRIBUTETABLE_H
#define MACARONI_MODEL_ATTRIBUTETABLE_H

#include "AttributeValue.h"
#include "AttributeTablePtr.h"
#include "ContextPtr.h"

#include "NodePtr.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Represents a collection of AttributeValues.
 * This is always attached to a Context, though it may be attached also to
 * a single Node.  If not, AttributeValues with their names matching
 * any node may be inserted in this table, but if this table is rooted
 * at a Node, only AttributeValues with names who are direct children
 * of the root Node may be inserted.
 * Think of it as two modes of operation.
 */
class AttributeTable
{
friend void intrusive_ptr_add_ref(AttributeTable * p);
friend void intrusive_ptr_release(AttributeTable * p);
public:
	typedef std::vector<AttributeValueInternalPtr>::iterator iterator;
	typedef std::vector<AttributeValueInternalPtr>::const_iterator const_iterator;	

	AttributeTable();

	AttributeTable(Context & context);

	AttributeTable(Node & home);
	
	~AttributeTable();

	AttributeValuePtr Add(AttributeValueInternalPtr value);

	AttributeValuePtr Add(const std::string & name, const bool value, const ReasonPtr & reason);
	
	AttributeValuePtr Add(const std::string & name, const double value, const ReasonPtr & reason);

	AttributeValuePtr Add(const std::string & name, NodePtr node, const ReasonPtr & reason);

	AttributeValuePtr Add(const std::string & name, const std::string & value, const ReasonPtr & reason);

	AttributeValuePtr Add(const std::string & name, const ReasonPtr & reason);

	const_iterator begin() { return values.begin(); }

	const_iterator end() { return values.end(); }	

	AttributeValuePtr GetByName(const NodePtr & name) const;

	AttributeValuePtr GetByName(const std::string & name) const;

	AttributeValuePtr Get(int index) const;

	NodePtr GetHomeNode();

private:	
	bool allowNestedNames() const;
	Context & context;
	Node & getHome() const;
	Node * home;
	std::vector<AttributeValueInternalPtr> values;
};

END_NAMESPACE2

#endif

