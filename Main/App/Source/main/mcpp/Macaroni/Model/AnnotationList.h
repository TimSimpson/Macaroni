#ifndef MACARONI_MODEL_ATTRIBUTELIST_H
#define MACARONI_MODEL_ATTRIBUTELIST_H

#include "../ME.h"
#include "AnnotationTablePtr.h"
#include "AnnotationValuePtr.h"
#include "NodePtr.h"
#include <boost/shared_ptr.hpp>
#include "Reason.h"
#include <boost/variant.hpp>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

///** Attached to a Node or Library, these map a NodePtr to one or more 
// * AnnotationValues.
// */
//class AnnotationList
//{
//public:
//	AnnotationList();
//	~AnnotationList();
//
//	std::vector<
//private:
//	std::vector<std::pair<Node & node, AnnotationValue>> attributes;
//};

END_NAMESPACE2

#endif

