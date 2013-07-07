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
#ifndef MACARONI_MODEL_MESSAGEAXIOM_CPP
#define MACARONI_MODEL_MESSAGEAXIOM_CPP

#include "MessageAxiom.h"
#include <Macaroni/Environment/Messages.h>

using Macaroni::Environment::Messages;

BEGIN_NAMESPACE2(Macaroni, Model)

MessageAxiom::MessageAxiom(const char * msgKey)
:msgKey(msgKey), refCount(0)
{
}

MessageAxiom::~MessageAxiom()
{
}

AxiomPtr MessageAxiom::Create(const char * msgKey)
{
	return AxiomPtr(new MessageAxiom(msgKey));
}

bool MessageAxiom::onDereference()
{
	refCount --;
	return refCount > 0;
}

void MessageAxiom::onReference()
{
	refCount ++;
}

const std::string MessageAxiom::ToString() const
{
	return Messages::Get(msgKey);
}

END_NAMESPACE2

#endif
