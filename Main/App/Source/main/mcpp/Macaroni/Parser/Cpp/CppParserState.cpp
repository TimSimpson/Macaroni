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
#ifndef MACARONI_PARSER_CPP_CPPPARSERSTATE_CPP
#define MACARONI_PARSER_CPP_CPPPARSERSTATE_CPP

#include "CppParserState.h"
#include <Macaroni/Model/Source.h>

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

using Macaroni::Model::SourcePtr;

CppParserState::CppParserState()
:context(), currentNode(), lineNumber(0)
{
}

Model::SourcePtr CppParserState::GetCurrentSource()
{
	this->currentSource = this->currentSource->JumpToLine(this->lineNumber);	
	return this->currentSource;
}

void CppParserState::IncrementLineNumber()
{
	this->lineNumber ++;
}

void CppParserState::SetContext(Model::ContextPtr value)
{
	if (context != value)
	{
		context = value;
		currentNode = context->GetRoot();
		currentScope = context->GetRoot();
	}
}

void CppParserState::SetCurrentSource(SourcePtr source)
{
	this->currentSource = source;
	this->lineNumber = source->GetLine();
}

END_NAMESPACE

#endif
