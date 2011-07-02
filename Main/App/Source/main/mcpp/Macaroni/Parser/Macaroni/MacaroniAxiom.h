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
#ifndef MACARONI_PARSER_MACARONI_MACARONIAXIOM_H
#define MACARONI_PARSER_MACARONI_MACARONIAXIOM_H

#include <Macaroni/ME.h>
#include <Macaroni/Model/Axiom.h>

BEGIN_NAMESPACE(Macaroni, Parser, Macaroni)

class MacaroniAxiom : public Model::Axiom
{
public:
	MacaroniAxiom(const char * msgKey);	

	~MacaroniAxiom();

	virtual const std::string ToString() const;

protected:
	
	virtual bool onDereference();

	virtual void onReference();

private:
	const char * msgKey;
	int refCount;
};

END_NAMESPACE

#endif