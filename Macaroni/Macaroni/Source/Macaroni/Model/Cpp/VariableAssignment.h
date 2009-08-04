#ifndef MACARONI_MODEL_CPP_VARIABLEASSIGNMENT_H
#define MACARONI_MODEL_CPP_VARIABLEASSIGNMENT_H

#include "../../ME.h"
#include "../Node.lh"
#include <string>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents an assignment to a variable. */
class VariableAssignment
{
public:
	std::string Expression;
	Model::NodePtr Variable;
};

END_NAMESPACE

#endif
