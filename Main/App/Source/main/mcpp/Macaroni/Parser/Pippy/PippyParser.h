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
#ifndef MACARONI_PARSER_PIPPY_PIPPYPARSER_H
#define MACARONI_PARSER_PIPPY_PIPPYPARSER_H

#include "../../ME.h"
#include "../../Model/LibraryPtr.h"
#include "PippyParserPtr.h"
#include <Macaroni/Parser/Parser.h>
#include <Macaroni/Model/Project/TargetPtr.h>

BEGIN_NAMESPACE(Macaroni, Parser, Pippy)

class PippyParser : public Macaroni::Parser::Parser
{
public:
	PippyParser();

	~PippyParser();

	static PippyParserPtr Create();

	virtual int Read(Macaroni::Model::Project::TargetPtr target,
	                 Model::SourcePtr source, const std::string & text);

	static void RunTests();

private:

private:

};

END_NAMESPACE

#endif
