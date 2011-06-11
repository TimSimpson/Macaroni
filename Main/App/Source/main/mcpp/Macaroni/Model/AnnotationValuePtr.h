#ifndef MACARONI_MODEL_ATTRIBUTEVALUEPTR_H
#define MACARONI_MODEL_ATTRIBUTEVALUEPTR_H

#include "../ME.h"
#include "AnnotationValue.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>


BEGIN_NAMESPACE2(Macaroni, Model)

class AnnotationValue;

typedef boost::shared_ptr<AnnotationValue> AnnotationValueInternalPtr;
typedef boost::intrusive_ptr<AnnotationValue> AnnotationValuePtr;
void intrusive_ptr_add_ref(AnnotationValue * p);
void intrusive_ptr_release(AnnotationValue * p);

END_NAMESPACE2

#endif

