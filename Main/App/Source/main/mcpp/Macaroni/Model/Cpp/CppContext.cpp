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
#ifndef MACARONI_MODEL_CPP_CPPCONTEXT_CPP
#define MACARONI_MODEL_CPP_CPPCONTEXT_CPP

#include "CppContext.h"
#include <Macaroni/Model/FileName.h>
#include "../MessageAxiom.h"
#include "../Cpp/Namespace.h"
#include "../Node.h"
#include "Primitive.h"
#include <Macaroni/Model/Reason.h>
#include <Macaroni/Model/Source.h>
#include <sstream>

using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::FileName;
using Macaroni::Model::FileNamePtr;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;
using Macaroni::Model::Reason;
using Macaroni::Model::ReasonPtr;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

void CppContext::CreateCppNodes(ContextPtr & context)
{
	if (context->GetRoot()->GetMember() == nullptr)
	{
		FileNamePtr file = FileName::Create(std::string("Cpp Parser"));
		SourcePtr src = Source::Create(file, 0, 0);
		Namespace::Create(context->GetRootLibrary(),
			              context->GetRoot(), 
						  Reason::Create(MessageAxiom::Create("CppAxioms.NamespaceRoot"), src)
						  );
	}

	NodePtr primitiveRoot = context->GetRoot()->FindOrCreate("{C++ Primitives}");
	if (primitiveRoot->GetChildCount() == 0)
	{
		addPrimitive(primitiveRoot, "bool");
		addPrimitive(primitiveRoot, "char");
		addPrimitive(primitiveRoot, "double");
		addPrimitive(primitiveRoot, "float");
		addPrimitive(primitiveRoot, "signed int");
		addPrimitive(primitiveRoot, "size_t");
		addPrimitive(primitiveRoot, "int");
		addPrimitive(primitiveRoot, "unsigned int");		
		addPrimitive(primitiveRoot, "void");
		addPrimitive(primitiveRoot, "wchar_t");
	}
}

void CppContext::addPrimitive(NodePtr & node, const char * name)
{
	static FileNamePtr file = FileName::Create(std::string("Cpp Parser"));
	static SourcePtr src = Source::Create(file, 0, 0);

	NodePtr primitive = node->FindOrCreate(name);
	std::stringstream ss;
	ss << "CppAxioms.Primitive." << name;
	Primitive::Create(primitive, Reason::Create(
		MessageAxiom::Create(ss.str().c_str()), src));
}

NodePtr CppContext::GetPrimitives(ContextPtr & context)
{
	return context->GetRoot()->Find("{C++ Primitives}");
}


END_NAMESPACE

#endif
