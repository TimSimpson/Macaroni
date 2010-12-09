#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AttributeValueTest
#include <boost/test/unit_test.hpp>

#include "AttributeTable.h"
#include "AttributeValue.h"
#include "AttributeValueTypeException.h"
#include "Axiom.h"
#include "Context.h"
#include "FileName.h"
#include <Macaroni/Parser/Macaroni/MacaroniAxioms.h>
#include "Member.h"
#include "ModelInconsistencyException.h"
#include "Node.h"
#include "Reason.h"
#include "Source.h"

using Macaroni::Model::AttributeTable;
using Macaroni::Model::AttributeTablePtr;
using Macaroni::Model::AttributeValue;
using Macaroni::Model::AttributeValuePtr;
using Macaroni::Model::AttributeValueTypeException;
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
	AxiomPtr axiom = Macaroni::Parser::Macaroni::MacaroniAxioms::AttributeImplicitDefinition();
	SourcePtr source = Source::Create(FileName::Create("TestFake.cpp"), 1, 37);
	ReasonPtr reason = Reason::Create(axiom, source);
	return reason;
}

BOOST_AUTO_TEST_SUITE(AttributeValueSuite)

BOOST_AUTO_TEST_CASE(Bool)
{	
	ContextPtr context(new Context("%ROOT%"));
	NodePtr boolAttrNode = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Attributes::Bool");
	BOOST_CHECK_MESSAGE(!boolAttrNode->GetMember(), "Attribute def is empty.");

	AttributeValue value(boolAttrNode, true, createReason());
	
	BOOST_CHECK_EQUAL(value.GetName(), boolAttrNode);//"boolAttribute");	
	BOOST_CHECK_MESSAGE(boolAttrNode->GetMember(), "Attribute def is not empty.");
	BOOST_CHECK_EQUAL(boolAttrNode->GetMember()->GetTypeName(), "AttributeDefinition");
	BOOST_CHECK_EQUAL(value.GetTypeString(), "bool");	
	BOOST_CHECK_MESSAGE(value.IsBool(), "Should look like bool.");
	BOOST_CHECK_MESSAGE(!value.IsNode(), "Should not look like a node.");		
	BOOST_CHECK_MESSAGE(!value.IsNumber(), "Should not look like a number.");
	BOOST_CHECK_MESSAGE(!value.IsString(), "Should not look like a string.");

	BOOST_CHECK_EQUAL(value.GetValueAsBool(), true);

	BOOST_CHECK_THROW(value.GetValueAsNode(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNumber(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsString(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsTable(), AttributeValueTypeException);
}

BOOST_AUTO_TEST_CASE(Node)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr nodeAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Attributes::Node");
	NodePtr node = context->GetRoot()->FindOrCreate(
		"SmilingNowButYoullFindOut::UseYouUp");
	
	AttributeValue value(nodeAttrDefinition, node, createReason());

	BOOST_CHECK_EQUAL(value.GetName(), nodeAttrDefinition);
	BOOST_CHECK_EQUAL(value.GetTypeString(), "node");
	BOOST_CHECK_MESSAGE(!value.IsBool(), "Should not look like bool.");
	BOOST_CHECK_MESSAGE(value.IsNode(), "Should look like a node.");		
	BOOST_CHECK_MESSAGE(!value.IsNumber(), "Should not look like a number.");
	BOOST_CHECK_MESSAGE(!value.IsString(), "Should not look like a string.");

	BOOST_CHECK_EQUAL(value.GetValueAsNode(), node);

	BOOST_CHECK_THROW(value.GetValueAsBool(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNumber(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsString(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsTable(), AttributeValueTypeException);
}

BOOST_AUTO_TEST_CASE(Number)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr numberAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Attributes::Number");
	const double number = 3945.13;
	AttributeValue value(numberAttrDefinition, number, createReason());

	BOOST_CHECK_EQUAL(value.GetName(), numberAttrDefinition);	
	BOOST_CHECK_EQUAL(value.GetTypeString(), "number");
	BOOST_CHECK_MESSAGE(!value.IsBool(), "Should not look like bool.");
	BOOST_CHECK_MESSAGE(!value.IsNode(), "Should not look like a node.");		
	BOOST_CHECK_MESSAGE(value.IsNumber(), "Should look like a number.");
	BOOST_CHECK_MESSAGE(!value.IsString(), "Should not look like a string.");	

	BOOST_CHECK_EQUAL(value.GetValueAsNumber(), number);

	BOOST_CHECK_THROW(value.GetValueAsBool(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNode(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsString(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsTable(), AttributeValueTypeException);
}

BOOST_AUTO_TEST_CASE(String)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr stringAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Attributes::String");

	AttributeValue value(stringAttrDefinition, std::string("Hello"), createReason());

	BOOST_CHECK_EQUAL(value.GetName(), stringAttrDefinition);
	BOOST_CHECK_EQUAL(value.GetTypeString(), "string");
	BOOST_CHECK_MESSAGE(!value.IsBool(), "Should not look like bool.");
	BOOST_CHECK_MESSAGE(!value.IsNode(), "Should not look like a node.");		
	BOOST_CHECK_MESSAGE(!value.IsNumber(), "Should not look like a number.");
	BOOST_CHECK_MESSAGE(value.IsString(), "Should look like a string.");	

	BOOST_CHECK_EQUAL(value.GetValueAsString(), "Hello");

	BOOST_CHECK_THROW(value.GetValueAsBool(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNode(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNumber(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsTable(), AttributeValueTypeException);
}

BOOST_AUTO_TEST_CASE(Table)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr tableAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Attributes::Table");

	AttributeValue value(tableAttrDefinition, createReason());
	
	AttributeTablePtr table = value.GetValueAsTable();	

	BOOST_CHECK_EQUAL(value.GetName(), tableAttrDefinition);
	BOOST_CHECK_EQUAL(value.GetTypeString(), "table");
	BOOST_CHECK_MESSAGE(!value.IsBool(), "Should not look like bool.");
	BOOST_CHECK_MESSAGE(!value.IsNode(), "Should not look like a node.");		
	BOOST_CHECK_MESSAGE(!value.IsNumber(), "Should not look like a number.");
	BOOST_CHECK_MESSAGE(!value.IsString(), "Should not look like a string.");	
	BOOST_CHECK_MESSAGE(value.IsTable(), "Should look like a tabke.");	

	BOOST_CHECK_EQUAL(value.GetValueAsTable(), table);

	BOOST_CHECK_THROW(value.GetValueAsBool(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNode(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsNumber(), AttributeValueTypeException);
	BOOST_CHECK_THROW(value.GetValueAsString(), AttributeValueTypeException);
}

BOOST_AUTO_TEST_CASE(Table_AddingNode)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr tableAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Attributes::Table");

	AttributeValue tableValue(tableAttrDefinition, createReason());	
	AttributeTablePtr table = tableValue.GetValueAsTable();
	
	NodePtr nodeToAdd = context->GetRoot()->FindOrCreate("TychfoerInc::Blarg::Bear");

	AttributeValuePtr nodeValue = table->Add("BearType", nodeToAdd, createReason());

	BOOST_CHECK_EQUAL(tableValue.GetValueAsTable()->GetByName("BearType")->GetValueAsNode(), nodeToAdd);
	BOOST_CHECK_EQUAL(table->Get(0), nodeValue);
	BOOST_CHECK_EQUAL(nodeValue->GetName()->GetFullName(), "Macaroni::Test::Attributes::Table::BearType");
	BOOST_CHECK_EQUAL(nodeValue->GetValueAsNode(), nodeToAdd);
}

BOOST_AUTO_TEST_CASE(Table_AddingNumber)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr tableAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Attributes::Table");

	AttributeValue tableValue(tableAttrDefinition, createReason());	
	AttributeTablePtr table = tableValue.GetValueAsTable();
	
	double number = 3442894378.56;

	AttributeValuePtr numberValue = table->Add("NumberOfBears", number, createReason());

	BOOST_CHECK_EQUAL(tableValue.GetValueAsTable()->GetByName("NumberOfBears"), numberValue);
	BOOST_CHECK_EQUAL(table->Get(0), numberValue);
	BOOST_CHECK_EQUAL(numberValue->GetName()->GetFullName(), "Macaroni::Test::Attributes::Table::NumberOfBears");
	BOOST_CHECK_EQUAL(numberValue->GetValueAsNumber(), number);
}

BOOST_AUTO_TEST_CASE(Table_AddingString)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr tableAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Attributes::Table");

	AttributeValue tableValue(tableAttrDefinition, createReason());	
	AttributeTablePtr table = tableValue.GetValueAsTable();
	
	AttributeValuePtr stringValue = table->Add("blah", std::string("dghfdjfs"), createReason());

	BOOST_CHECK_EQUAL(tableValue.GetValueAsTable()->GetByName("blah")->GetValueAsString(), "dghfdjfs");
	BOOST_CHECK_EQUAL(table->Get(0), stringValue);
	BOOST_CHECK_EQUAL(stringValue->GetName()->GetFullName(), "Macaroni::Test::Attributes::Table::blah");
	BOOST_CHECK_EQUAL(stringValue->GetValueAsString(), "dghfdjfs");
}

BOOST_AUTO_TEST_CASE(Table_AddingTable)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr tableAttrDefinition = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Attributes::Table");

	AttributeValue tableValue(tableAttrDefinition, createReason());	
	AttributeTablePtr table = tableValue.GetValueAsTable();
	
	AttributeValuePtr nestedTableValue = table->Add("Options", createReason());
	AttributeTablePtr nestedTable = nestedTableValue->GetValueAsTable();

	BOOST_CHECK_EQUAL(table->Get(0), nestedTableValue);
	BOOST_CHECK_EQUAL(tableValue.GetValueAsTable()->GetByName("Options")->GetValueAsTable(), nestedTable);	
	BOOST_CHECK_EQUAL(nestedTableValue->GetName()->GetFullName(), "Macaroni::Test::Attributes::Table::Options");
	BOOST_CHECK_EQUAL(nestedTableValue->GetValueAsTable(), nestedTable);
}


BOOST_AUTO_TEST_CASE(When_Contradicting_A_Previous_Definition)
{
	ContextPtr context(new Context("%ROOT%"));
	NodePtr attrName = context->GetRoot()->FindOrCreate(
		"Macaroni::Test::Attributes::Something");

	BOOST_CHECK_EQUAL(createReason()->ToString(), 
		"TestFake.cpp, line 1, column 37: MacaroniAxioms.Attribute.Definition.Implicit");
	AttributeValue value(attrName, std::string("Hello"), createReason());
	AttributeValue value2(attrName, std::string("This will work as well..."), createReason());
	BOOST_CHECK_THROW(AttributeValue(attrName, 43.0, createReason()), ModelInconsistencyException);
}


BOOST_AUTO_TEST_SUITE_END()
