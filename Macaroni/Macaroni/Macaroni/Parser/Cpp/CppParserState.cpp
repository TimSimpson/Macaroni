#ifndef MACARONI_PARSER_CPP_CPPPARSERSTATE_CPP
#define MACARONI_PARSER_CPP_CPPPARSERSTATE_CPP

#include "CppParserState.h"
#include "../../Model/Source.h"

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
	this->lineNumber = source->GetLineNumber();
}

END_NAMESPACE

#endif
