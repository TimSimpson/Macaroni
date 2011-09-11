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
#ifndef MACARONI_GENERATOR_DEBUGENUMERATOR_CPP
#define MACARONI_GENERATOR_DEBUGENUMERATOR_CPP

#include "DebugEnumerator.h"
#include <Macaroni/Model/Element.h>
#include <Macaroni/Model/ElementPtr.h>
#include "../Model/Node.h"
#include "../Model/Member.h"

using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::Element;
using Macaroni::Model::ElementPtr;
using Macaroni::Model::Member;
using Macaroni::Model::MemberPtr;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;

BEGIN_NAMESPACE2(Macaroni, Generator)

DebugEnumerator::DebugEnumerator()
{
}
	
DebugEnumerator::~DebugEnumerator()
{
}

void iterateNode(std::ostream & out, int depth, NodePtr node)
{
	std::string deeper(depth, ' ');
	out << deeper << node->GetFullName() << "";
	ElementPtr member = node->GetElement();
	out << deeper << " : ";
	if (member)
	{
		out << member->GetTypeName() ;
	}
	else
	{
		out << "nullptr";
	}		
	out << "\n";
	for(unsigned int i = 0; i < node->GetChildCount(); i ++)
	{
		NodePtr child = node->GetChild(i);
		iterateNode(out, depth + 1, child);
	}
}

void DebugEnumerator::Iterate(ContextPtr context, std::ostream & out)
{
	iterateNode(out, 0, context->GetRoot());
}

END_NAMESPACE2

#endif

