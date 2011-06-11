#ifndef MACARONI_MODEL_ATTRIBUTETABLE_H
#define MACARONI_MODEL_ATTRIBUTETABLE_H

#include "AnnotationValue.h"
#include "AnnotationTablePtr.h"
#include "ContextPtr.h"

#include "NodePtr.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Represents a collection of AnnotationValues.
 * This is always attached to a Context, though it may be attached also to
 * a single Node.  If not, AnnotationValues with their names matching
 * any node may be inserted in this table, but if this table is rooted
 * at a Node, only AnnotationValues with names who are direct children
 * of the root Node may be inserted.
 * Think of it as two modes of operation.
 */
class AnnotationTable
{
friend void intrusive_ptr_add_ref(AnnotationTable * p);
friend void intrusive_ptr_release(AnnotationTable * p);
public:
	typedef std::vector<AnnotationValueInternalPtr>::iterator iterator;
	typedef std::vector<AnnotationValueInternalPtr>::const_iterator const_iterator;	

	AnnotationTable();

	AnnotationTable(Context & context);

	AnnotationTable(Node & home);
	
	~AnnotationTable();

	AnnotationValuePtr Add(AnnotationValueInternalPtr value);

	AnnotationValuePtr Add(const std::string & name, const bool value, const ReasonPtr & reason);
	
	AnnotationValuePtr Add(const std::string & name, const double value, const ReasonPtr & reason);

	AnnotationValuePtr Add(const std::string & name, NodePtr node, const ReasonPtr & reason);

	AnnotationValuePtr Add(const std::string & name, const std::string & value, const ReasonPtr & reason);

	AnnotationValuePtr Add(const std::string & name, const ReasonPtr & reason);

	const_iterator begin() { return values.begin(); }

	const_iterator end() { return values.end(); }	

	AnnotationValuePtr GetByName(const NodePtr & name) const;

	AnnotationValuePtr GetByName(const std::string & name) const;

	AnnotationValuePtr Get(int index) const;

	NodePtr GetHomeNode();

private:	
	bool allowNestedNames() const;
	Context & context;
	Node & getHome() const;
	Node * home;
	std::vector<AnnotationValueInternalPtr> values;
};

END_NAMESPACE2

#endif

