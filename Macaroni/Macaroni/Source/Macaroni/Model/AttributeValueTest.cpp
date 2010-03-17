#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AttributeValueTest
#include <boost/test/unit_test.hpp>

#include "AttributeTable.h"
#include "AttributeValue.h"
#include "AttributeValueTypeException.h"
#include "Context.h"
#include "Node.h"

using Macaroni::Model::AttributeTable;
using Macaroni::Model::AttributeTablePtr;
using Macaroni::Model::AttributeValue;
using Macaroni::Model::AttributeValuePtr;
using Macaroni::Model::AttributeValueTypeException;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;

BOOST_AUTO_TEST_SUITE(AttributeValueSuite)

BOOST_AUTO_TEST_CASE(Bool)
{	
	AttributeValue value("boolAttribute", true);

	BOOST_CHECK_EQUAL(value.GetName(), "boolAttribute");	
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
	NodePtr node = context->GetRoot()->FindOrCreate(
		"SmilingNowButYoullFindOut::UseYouUp");
	
	AttributeValue value("nodeAttr", node);

	BOOST_CHECK_EQUAL(value.GetName(), "nodeAttr");
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
	const double number = 3945.13;
	AttributeValue value("numberAttr", number);

	BOOST_CHECK_EQUAL(value.GetName(), "numberAttr");	
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
	AttributeValue value("stringAttr", std::string("Hello"));

	BOOST_CHECK_EQUAL(value.GetName(), "stringAttr");
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
	AttributeTablePtr table(new AttributeTable());
	AttributeValuePtr blah(new AttributeValue("blah", std::string("dghfdjfs")));
	table->Add(blah);

	AttributeValue value("tableAttr", table);

	BOOST_CHECK_EQUAL(value.GetName(), "tableAttr");
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

	BOOST_CHECK_EQUAL(value.GetValueAsTable()->GetByName("blah")->GetValueAsString(), "dghfdjfs");
	BOOST_CHECK_EQUAL(value.GetValueAsTable()->Get(0), blah);
}


BOOST_AUTO_TEST_SUITE_END()
