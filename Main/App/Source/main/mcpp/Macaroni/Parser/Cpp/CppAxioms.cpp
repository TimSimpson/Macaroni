#ifndef MACARONI_PARSER_CPP_CPPAXIOMS_CPP
#define MACARONI_PARSER_CPP_CPPAXIOMS_CPP

#include "CppAxioms.h"
#include "CppAxiom.h"
#include "../../Environment/Messages.h"

using Macaroni::Environment::Messages;

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)
namespace CppAxioms
{

AxiomPtr AnnotationValueCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.AnnotationValueCreation"));
}

AxiomPtr BlockCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.BlockCreation"));
}

AxiomPtr ClassCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.ClassCreation"));
}

AxiomPtr CtorCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.ConstructorCreation"));
}

AxiomPtr DtorCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.DestructorCreation"));
}

AxiomPtr FunctionCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.FunctionCreation"));
}

AxiomPtr NamespaceCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.NamespaceCreation"));
}

AxiomPtr PrimitiveChar()
{
	return AxiomPtr(new CppAxiom("CppAxioms.PrimitiveChar"));
}

AxiomPtr PrimitiveDouble()
{
	return AxiomPtr(new CppAxiom("CppAxioms.PrimitiveDouble"));
}

AxiomPtr PrimitiveFloat()
{
	return AxiomPtr(new CppAxiom("CppAxioms.PrimitiveFloat"));
}

AxiomPtr PrimitiveSignedInt()
{
	return AxiomPtr(new CppAxiom("CppAxioms.PrimitiveSignedInt"));
}

AxiomPtr PrimitiveUnsignedInt()
{
	return AxiomPtr(new CppAxiom("CppAxioms.PrimitiveUnsignedInt"));
}

AxiomPtr SetExistingHFilePath()
{
	return AxiomPtr(new CppAxiom("CppAxioms.SetExistingHFilePath"));
}

AxiomPtr TypedefCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.TypedefCreation"));
}

AxiomPtr VariableScopeCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.VariableScopeCreation"));
}

}// end of CppAxioms namespace
END_NAMESPACE

#endif