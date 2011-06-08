#ifndef MACARONI_MODEL_CPP_CLASSPARENTPTR_H
#define MACARONI_MODEL_CPP_CLASSPARENTPTR_H

#include "../../ME.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class ClassParent;

typedef boost::intrusive_ptr<ClassParent> ClassParentPtr;
typedef std::vector<Macaroni::Model::Cpp::ClassParent > ClassParentList;
typedef boost::shared_ptr<Macaroni::Model::Cpp::ClassParentList > ClassParentListPtr;

END_NAMESPACE

#endif
