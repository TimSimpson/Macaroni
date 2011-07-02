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
#ifndef MACARONI_PARSER_PARSER_H
#define MACARONI_PARSER_PARSER_H

#include "../ME.h"
#include "../Model/ContextPtr.h"
#include "../Model/LibraryPtr.h"
#include "ParserPtr.h"
#include "../Model/SourcePtr.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

/** Interface for parsing text into Model objects. */
class Parser
{
public:
	Parser(){}
	virtual ~Parser(){}

	/** Returns lines read.  Throws Model::ModelInconsistencyException, or 
	 * Parser::ParserException. */
	virtual int Read(Model::LibraryPtr l, // Library to read into.
					  Model::SourcePtr source, // Where the text begins. 
					  const std::string & text // Text.
					  ) = 0;
};

END_NAMESPACE2

#endif