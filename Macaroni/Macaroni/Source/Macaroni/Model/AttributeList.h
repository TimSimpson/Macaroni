#ifndef MACARONI_MODEL_ATTRIBUTELIST_H
#define MACARONI_MODEL_ATTRIBUTELIST_H

#include "../ME.h"
#include "AttributeTablePtr.h"
#include "AttributeValuePtr.h"
#include "NodePtr.h"
#include <boost/shared_ptr.hpp>
#include "Reason.h"
#include <boost/variant.hpp>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

///** Attached to a Node or Library, these map a NodePtr to one or more 
// * AttributeValues.
// */
//class AttributeList
//{
//public:
//	AttributeList();
//	~AttributeList();
//
//	std::vector<
//private:
//	std::vector<std::pair<Node & node, AttributeValue>> attributes;
//};

END_NAMESPACE2

#endif

