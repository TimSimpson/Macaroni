/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MACARONI_MODEL_CPP_VARIABLEASSIGNMENT_H
#define MACARONI_MODEL_CPP_VARIABLEASSIGNMENT_H

#include "../../ME.h"
#include <boost/shared_ptr.hpp>
#include "../NodePtr.h"
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
