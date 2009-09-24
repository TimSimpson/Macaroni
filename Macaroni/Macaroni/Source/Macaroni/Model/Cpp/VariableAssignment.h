#ifndef MACARONI_MODEL_CPP_VARIABLEASSIGNMENT_H
#define MACARONI_MODEL_CPP_VARIABLEASSIGNMENT_H

#include "../../ME.h"
#include <boost/shared_ptr.hpp>
#include "../Node.lh"
#include <string>


BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class VariableAssignment;
typedef boost::shared_ptr<VariableAssignment> VariableAssignmentPtr;

/** Represents an assignment to a variable. */
class VariableAssignment
{
public:
	std::string Expression;
	Model::NodePtr Variable;
};

END_NAMESPACE

#endif
