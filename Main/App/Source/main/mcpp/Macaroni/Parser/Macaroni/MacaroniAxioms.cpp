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
#ifndef MACARONI_PARSER_MACARONI_MACARONIAXIOMS_CPP
#define MACARONI_PARSER_MACARONI_MACARONIAXIOMS_CPP

#include "MacaroniAxioms.h"
#include "MacaroniAxiom.h"
#include <Macaroni/Environment/Messages.h>

using Macaroni::Environment::Messages;

BEGIN_NAMESPACE(Macaroni, Parser, Macaroni)
namespace MacaroniAxioms
{

AxiomPtr AnnotationImplicitDefinition()
{
	return AxiomPtr(new MacaroniAxiom("MacaroniAxioms.Annotation.Definition.Implicit"));
}

}

END_NAMESPACE

#endif
