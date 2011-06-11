#ifndef MACARONI_MODEL_ATTRIBUTETABLEPTR_H
#define MACARONI_MODEL_ATTRIBUTETABLEPTR_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class AnnotationTable;

typedef boost::shared_ptr<AnnotationTable> AnnotationTableInternalPtr;
typedef boost::intrusive_ptr<AnnotationTable> AnnotationTablePtr;

void intrusive_ptr_add_ref(AnnotationTable * p);
void intrusive_ptr_release(AnnotationTable * p);

END_NAMESPACE2

#endif

