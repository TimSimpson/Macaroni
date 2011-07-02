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
#ifndef MACARONI_GENERATOR_DEBUGENUMERATOR_H
#define MACARONI_GENERATOR_DEBUGENUMERATOR_H

#include "../ME.h"
#include "../Model/Context.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>

BEGIN_NAMESPACE2(Macaroni, Generator)

class DebugEnumerator
{
public:
	DebugEnumerator();
	
	~DebugEnumerator();
	
	void Iterate(Model::ContextPtr context, std::ostream & out);

private:

};

END_NAMESPACE2

#endif

