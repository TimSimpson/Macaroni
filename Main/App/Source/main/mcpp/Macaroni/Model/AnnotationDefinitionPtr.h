#ifndef MACARONI_MODEL_ATTRIBUTEDEFINITIONPTR_H
#define MACARONI_MODEL_ATTRIBUTEDEFINITIONPTR_H

#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class AnnotationDefinition;
typedef boost::intrusive_ptr<AnnotationDefinition> AnnotationDefinitionPtr;

void intrusive_ptr_add_ref(AnnotationDefinition * p);
void intrusive_ptr_release(AnnotationDefinition * p);

END_NAMESPACE2

#endif
