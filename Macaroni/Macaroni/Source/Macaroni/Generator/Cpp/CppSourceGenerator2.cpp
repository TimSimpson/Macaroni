#ifndef MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR2_CPP
#define MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR2_CPP

#include "../../ME.h"
#include <algorithm>
#include "CppSourceGenerator2.h"
#include <boost/filesystem/operations.hpp>
#include "../../Model/Cpp/Class.h"
#include "../../Model/Cpp/Constructor.h"
#include "../../Model/Cpp/Destructor.h"
#include <fstream>
#include "../../Model/Cpp/Function.h"
#include <iostream>
#include "../../Model/Node.h"
#include "../../Model/MemberVisitor.h"
#include "../../Model/Cpp/Namespace.h"
#include "../../Model/Cpp/Primitive.h"
#include <string>
#include <sstream>
#include "../../Model/Cpp/TypeInfo.h"
#include "../../Model/Cpp/Variable.h"
#include "../../Model/Cpp/VariableAssignment.h"

using Macaroni::Model::Cpp::Class;
using Macaroni::Model::Cpp::ClassPtr;
using Macaroni::Model::Cpp::Constructor;
using Macaroni::Model::Cpp::ConstructorPtr;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::Cpp::Destructor;
using Macaroni::Model::Cpp::DestructorPtr;
using Macaroni::Model::Cpp::Function;
using Macaroni::Model::Cpp::FunctionPtr;
using Macaroni::Model::Member;
using Macaroni::Model::MemberPtr;
using Macaroni::Model::MemberVisitor;
using Macaroni::Model::Cpp::Namespace;
using Macaroni::Model::Cpp::NamespacePtr;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;
using Macaroni::Model::NodeList;
using Macaroni::Model::NodeListPtr;
using boost::filesystem::path;
using Macaroni::Model::Cpp::Primitive;
using Macaroni::Model::Cpp::TypeInfo;
using Macaroni::Model::Cpp::Variable;
using Macaroni::Model::Cpp::VariableAssignment;
using Macaroni::Model::Cpp::VariablePtr;


BEGIN_NAMESPACE(Macaroni, Generator, Cpp)


CppSourceGenerator2::CppSourceGenerator2(ContextPtr context, const boost::filesystem::path & path)
:context(context), rootPath(path)
{

}
	
CppSourceGenerator2::~CppSourceGenerator2()
{
}
	
bool operator<(const NodePtr & a, const NodePtr & b)
{
	return a->GetName() < b->GetName();
}

void sortNodes(NodeList & list)
{
	std::sort(list.begin(), list.end());
}

void createNodeListFromNode(NodeList & rtnList, NodePtr source)
{
	rtnList.clear();
	for (unsigned int i = 0; i < source->GetChildCount(); i ++)
	{
		rtnList.push_back(source->GetChild(i));
	}
}

void createAlphabetizedNodeListFromNode(NodeList & rtnList, NodePtr source)
{
	createNodeListFromNode(rtnList, source);
	sortNodes(rtnList);
}

#include "ClassHFileGenerator._cpp" 

void iterateNodes(path path, NodePtr parentNode)
{
	NodeList nodes;
	createAlphabetizedNodeListFromNode(nodes, parentNode);

	for(NodeList::iterator itr = nodes.begin();
		itr < nodes.end(); itr ++)
	{
		NodePtr node = *itr;
		MemberPtr member = node->GetMember();
		if (!!member)
		{
			if (member->GetTypeName() == "Class")
			{
				
			}
		}
	}
}

void CppSourceGenerator2::Run()
{
}

END_NAMESPACE

#endif

