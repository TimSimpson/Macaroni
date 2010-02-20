#ifndef MACARONI_GENERATOR_DEBUGENUMERATOR_CPP
#define MACARONI_GENERATOR_DEBUGENUMERATOR_CPP

#include "DebugEnumerator.h"
#include "../Model/Node.h"
#include "../Model/Member.h"

using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
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
	MemberPtr member = node->GetMember();
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

