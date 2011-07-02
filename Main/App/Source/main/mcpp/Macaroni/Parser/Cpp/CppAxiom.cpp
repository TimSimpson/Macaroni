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
#ifndef MACARONI_PARSER_CPP_CPPAXIOM_CPP
#define MACARONI_PARSER_CPP_CPPAXIOM_CPP

#include "CppAxiom.h"
#include "../../Environment/Messages.h"

using Macaroni::Environment::Messages;

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

CppAxiom::CppAxiom(const char * msgKey)
:msgKey(msgKey), refCount(0)
{
}

CppAxiom::~CppAxiom()
{
}

bool CppAxiom::onDereference()
{
	refCount --;
	return refCount > 0;
}

void CppAxiom::onReference()
{
	refCount ++;
}

const std::string CppAxiom::ToString() const
{
	return Messages::Get(msgKey);
}

END_NAMESPACE

#endif