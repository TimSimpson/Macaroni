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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AnnotationValueTest
#include <boost/test/unit_test.hpp>

#include <Macaroni/Model/AnnotationTable.h>
#include <Macaroni/Model/AnnotationValue.h>
#include <Macaroni/Model/AnnotationValueTypeException.h>
#include <Macaroni/Model/Axiom.h>
#include <Macaroni/Model/Context.h>
#include <Macaroni/Model/FileName.h>
#include <Macaroni/Parser/MacaroniAxioms.h>
#include <Macaroni/Model/Member.h>
#include <Macaroni/Model/ModelInconsistencyException.h>
#include <Macaroni/Model/Node.h>
#include <Macaroni/Model/Reason.h>
#include <Macaroni/Model/Source.h>

using Macaroni::Model::AnnotationTable;
using Macaroni::Model::AnnotationTablePtr;
using Macaroni::Model::AnnotationValue;
using Macaroni::Model::AnnotationValuePtr;
using Macaroni::Model::AnnotationValueTypeException;
using Macaroni::Model::Axiom;
using Macaroni::Model::AxiomPtr;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::FileName;
using Macaroni::Model::FileNamePtr;
using Macaroni::Model::Member;
using Macaroni::Model::MemberPtr;
using Macaroni::Model::ModelInconsistencyException;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;
using Macaroni::Model::Reason;
using Macaroni::Model::ReasonPtr;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;

static ReasonPtr createReason()
{
	AxiomPtr axiom = Macaroni::Parser::MacaroniAxioms::AnnotationImplicitDefinition();
	SourcePtr source = Source::Create(FileName::Create("TestFake.cpp"), 1, 37);
	ReasonPtr reason = Reason::Create(axiom, source);
	return reason;
}

BOOST_AUTO_TEST_SUITE(AnnotationValueSuite)

BOOST_AUTO_TEST_CASE(Bool)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr boolAttrNode = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Annotations::Bool");
	BOOST_CHECK_MESSAGE(!boolAttrNode->GetElement(), "Annotation def is empty.");

	AnnotationValue value(boolAttrNode, true, createReason());

	BOOST_CHECK_EQUAL(value.GetName(), boolAttrNode);//"boolAnnotation");
	BOOST_CHECK_MESSAGE(boolAttrNode->GetElement(), "Annotation def is not empty.");
	BOOST_CHECK_EQUAL(boolAttrNode->GetElement()->GetTypeName(), "AnnotationDefinition");
	BOOST_CHECK_EQUAL(value.GetTypeString(), "bool");
	BOOST_CHECK_MESSAGE(value.IsBool(), "Should look like bool.");
	BOOST_CHECK_MESSAGE(!value.IsNode(), "Should not look like a node.");
	BOOST_CHECK_MESSAGE(!value.IsNumber(), "Should not look like a number.");
	BOOST_CHECK_MESSAGE(!value.IsString(), "Should not look like a string.");

	BOOST_CHECK_EQUAL(value.GetValueAsBool(), true);

	BOOST_CHECK_THROW(value.GetValueAsNode(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNumber(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsString(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsTable(), AnnotationValueTypeException);
}

BOOST_AUTO_TEST_CASE(Node)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr nodeAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Annotations::Node");
	NodePtr node = context->GetRoot()->FindOrCreate(
		"SmilingNowButYoullFindOut::UseYouUp");

	AnnotationValue value(nodeAttrDefinition, node, createReason());

	BOOST_CHECK_EQUAL(value.GetName(), nodeAttrDefinition);
	BOOST_CHECK_EQUAL(value.GetTypeString(), "node");
	BOOST_CHECK_MESSAGE(!value.IsBool(), "Should not look like bool.");
	BOOST_CHECK_MESSAGE(value.IsNode(), "Should look like a node.");
	BOOST_CHECK_MESSAGE(!value.IsNumber(), "Should not look like a number.");
	BOOST_CHECK_MESSAGE(!value.IsString(), "Should not look like a string.");

	BOOST_CHECK_EQUAL(value.GetValueAsNode(), node);

	BOOST_CHECK_THROW(value.GetValueAsBool(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNumber(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsString(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsTable(), AnnotationValueTypeException);
}

BOOST_AUTO_TEST_CASE(Number)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr numberAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Annotations::Number");
	const double number = 3945.13;
	AnnotationValue value(numberAttrDefinition, number, createReason());

	BOOST_CHECK_EQUAL(value.GetName(), numberAttrDefinition);
	BOOST_CHECK_EQUAL(value.GetTypeString(), "number");
	BOOST_CHECK_MESSAGE(!value.IsBool(), "Should not look like bool.");
	BOOST_CHECK_MESSAGE(!value.IsNode(), "Should not look like a node.");
	BOOST_CHECK_MESSAGE(value.IsNumber(), "Should look like a number.");
	BOOST_CHECK_MESSAGE(!value.IsString(), "Should not look like a string.");

	BOOST_CHECK_EQUAL(value.GetValueAsNumber(), number);

	BOOST_CHECK_THROW(value.GetValueAsBool(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNode(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsString(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsTable(), AnnotationValueTypeException);
}

BOOST_AUTO_TEST_CASE(String)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr stringAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Annotations::String");

	AnnotationValue value(stringAttrDefinition, std::string("Hello"), createReason());

	BOOST_CHECK_EQUAL(value.GetName(), stringAttrDefinition);
	BOOST_CHECK_EQUAL(value.GetTypeString(), "string");
	BOOST_CHECK_MESSAGE(!value.IsBool(), "Should not look like bool.");
	BOOST_CHECK_MESSAGE(!value.IsNode(), "Should not look like a node.");
	BOOST_CHECK_MESSAGE(!value.IsNumber(), "Should not look like a number.");
	BOOST_CHECK_MESSAGE(value.IsString(), "Should look like a string.");

	BOOST_CHECK_EQUAL(value.GetValueAsString(), "Hello");

	BOOST_CHECK_THROW(value.GetValueAsBool(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNode(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNumber(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsTable(), AnnotationValueTypeException);
}

BOOST_AUTO_TEST_CASE(Table)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr tableAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Annotations::Table");

	AnnotationValue value(tableAttrDefinition, createReason());

	AnnotationTablePtr table = value.GetValueAsTable();

	BOOST_CHECK_EQUAL(value.GetName(), tableAttrDefinition);
	BOOST_CHECK_EQUAL(value.GetTypeString(), "table");
	BOOST_CHECK_MESSAGE(!value.IsBool(), "Should not look like bool.");
	BOOST_CHECK_MESSAGE(!value.IsNode(), "Should not look like a node.");
	BOOST_CHECK_MESSAGE(!value.IsNumber(), "Should not look like a number.");
	BOOST_CHECK_MESSAGE(!value.IsString(), "Should not look like a string.");
	BOOST_CHECK_MESSAGE(value.IsTable(), "Should look like a tabke.");

	BOOST_CHECK_EQUAL(value.GetValueAsTable(), table);

	BOOST_CHECK_THROW(value.GetValueAsBool(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNode(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNumber(), AnnotationValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsString(), AnnotationValueTypeException);
}

BOOST_AUTO_TEST_CASE(Table_AddingNode)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr tableAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Annotations::Table");

	AnnotationValue tableValue(tableAttrDefinition, createReason());
	AnnotationTablePtr table = tableValue.GetValueAsTable();

	NodePtr nodeToAdd = context->GetRoot()->FindOrCreate("TychfoerInc::Blarg::Bear");

	AnnotationValuePtr nodeValue = table->Add("BearType", nodeToAdd, createReason());

	BOOST_CHECK_EQUAL(tableValue.GetValueAsTable()->GetByName("BearType")->GetValueAsNode(), nodeToAdd);
	BOOST_CHECK_EQUAL(table->Get(0), nodeValue);
	BOOST_CHECK_EQUAL(nodeValue->GetName()->GetFullName(), "Macaroni::Test::Annotations::Table::BearType");
	BOOST_CHECK_EQUAL(nodeValue->GetValueAsNode(), nodeToAdd);
}

BOOST_AUTO_TEST_CASE(Table_AddingNumber)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr tableAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Annotations::Table");

	AnnotationValue tableValue(tableAttrDefinition, createReason());
	AnnotationTablePtr table = tableValue.GetValueAsTable();

	double number = 3442894378.56;

	AnnotationValuePtr numberValue = table->Add("NumberOfBears", number, createReason());

	BOOST_CHECK_EQUAL(tableValue.GetValueAsTable()->GetByName("NumberOfBears"), numberValue);
	BOOST_CHECK_EQUAL(table->Get(0), numberValue);
	BOOST_CHECK_EQUAL(numberValue->GetName()->GetFullName(), "Macaroni::Test::Annotations::Table::NumberOfBears");
	BOOST_CHECK_EQUAL(numberValue->GetValueAsNumber(), number);
}

BOOST_AUTO_TEST_CASE(Table_AddingString)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr tableAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Annotations::Table");

	AnnotationValue tableValue(tableAttrDefinition, createReason());
	AnnotationTablePtr table = tableValue.GetValueAsTable();

	AnnotationValuePtr stringValue = table->Add("blah", std::string("dghfdjfs"), createReason());

	BOOST_CHECK_EQUAL(tableValue.GetValueAsTable()->GetByName("blah")->GetValueAsString(), "dghfdjfs");
	BOOST_CHECK_EQUAL(table->Get(0), stringValue);
	BOOST_CHECK_EQUAL(stringValue->GetName()->GetFullName(), "Macaroni::Test::Annotations::Table::blah");
	BOOST_CHECK_EQUAL(stringValue->GetValueAsString(), "dghfdjfs");
}

BOOST_AUTO_TEST_CASE(Table_AddingTable)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr tableAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Annotations::Table");

	AnnotationValue tableValue(tableAttrDefinition, createReason());
	AnnotationTablePtr table = tableValue.GetValueAsTable();

	AnnotationValuePtr nestedTableValue = table->Add("Options", createReason());
	AnnotationTablePtr nestedTable = nestedTableValue->GetValueAsTable();

	BOOST_CHECK_EQUAL(table->Get(0), nestedTableValue);
	BOOST_CHECK_EQUAL(tableValue.GetValueAsTable()->GetByName("Options")->GetValueAsTable(), nestedTable);
	BOOST_CHECK_EQUAL(nestedTableValue->GetName()->GetFullName(), "Macaroni::Test::Annotations::Table::Options");
	BOOST_CHECK_EQUAL(nestedTableValue->GetValueAsTable(), nestedTable);
}


BOOST_AUTO_TEST_CASE(When_Contradicting_A_Previous_Definition)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr attrName = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Annotations::Something");

	BOOST_CHECK_EQUAL(createReason()->ToString(),
		"TestFake.cpp, line 1, column 37: MacaroniAxioms.Annotation.Definition.Implicit");
	AnnotationValue value(attrName, std::string("Hello"), createReason());
	AnnotationValue value2(attrName, std::string("This will work as well..."), createReason());
	BOOST_CHECK_THROW(AnnotationValue(attrName, 43.0, createReason()), ModelInconsistencyException);
}


BOOST_AUTO_TEST_SUITE_END()
