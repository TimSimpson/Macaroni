#ifndef MACARONI_MODEL_CPP_CPPCONTEXT_CPP
#define MACARONI_MODEL_CPP_CPPCONTEXT_CPP

#include "CppContext.h"
#include "../FileName.h"
#include "../MessageAxiom.h"
#include "../Node.h"
#include "Primitive.h"
#include "../Reason.h"
#include "../Source.h"
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
	NodePtr primitiveRoot = context->GetRoot()->FindOrCreate("{C++ Primitives}");
	addPrimitive(primitiveRoot, "char");
	addPrimitive(primitiveRoot, "double");
	addPrimitive(primitiveRoot, "float");
	addPrimitive(primitiveRoot, "signed int");
	addPrimitive(primitiveRoot, "unsigned int");		
	addPrimitive(primitiveRoot, "void");
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
