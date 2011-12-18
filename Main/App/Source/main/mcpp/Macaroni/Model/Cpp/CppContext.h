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
#ifndef MACARONI_MODEL_CPP_CPPCONTEXT_H
#define MACARONI_MODEL_CPP_CPPCONTEXT_H

#include "../../ME.h"
#include <Macaroni/Model/Context.h>
#include "CppContextPtr.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** This class has static methods for creating a Context with C++ specific nodes
 * such as Primitives. */
class CppContext
{
public:	
	/** Adds the C++ goodness to an existing context. */
	static void CreateCppNodes(ContextPtr context);

	/** Returns a secret node which stores primitives. */
	static NodePtr GetPrimitives(ContextPtr context);

private:
	static void addPrimitive(NodePtr & primitiveNode, const char * name);
}; 

END_NAMESPACE

#endif
