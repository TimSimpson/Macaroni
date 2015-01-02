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
#ifndef MACARONI_PARSER_CPP_CPPAXIOMS_CPP
#define MACARONI_PARSER_CPP_CPPAXIOMS_CPP

#include "CppAxioms.h"
#include "CppAxiom.h"
#include <Macaroni/Environment/Messages.h>

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

AxiomPtr EnumCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.EnumCreation"));
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

AxiomPtr TemplateTypenameCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.TemplateTypenameCreation"));
}

AxiomPtr TemplateParameterListCreation()
{
	return AxiomPtr(new CppAxiom("CppAxioms.TemplateParameterListCreation"));
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
