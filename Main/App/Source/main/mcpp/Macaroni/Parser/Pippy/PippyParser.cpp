#ifndef MACARONI_PARSER_PIPPY_PIPPYPARSER_CPP
#define MACARONI_PARSER_PIPPY_PIPPYPARSER_CPP

#include "../../ME.h"
#include <Macaroni/Model/AttributeTable.h>
#include <Macaroni/Model/AttributeValue.h>
#include <Macaroni/Model/AttributeValuePtr.h>
#include <Macaroni/Model/Cpp/Access.h>
#include "../../Model/Block.h"
#include "../../Model/Cpp/Class.h"
#include "../../Model/Cpp/Constructor.h"
#include <Macaroni/Model/Cpp/ConstructorOverload.h>
#include "../../Model/Cpp/ConstructorPtr.h"
#include "../../Model/Context.h"
#include "../Cpp/CppAxioms.h"
#include "../../Model/Cpp/CppContext.h"
#include "../../Model/Cpp/Destructor.h"
#include "../../Model/Cpp/DestructorPtr.h"
#include "../../Exception.h"
#include "../../Model/FileName.h"
#include "../../Model/Cpp/Function.h"
#include <Macaroni/Model/Cpp/FunctionOverload.h>
#include "../../Model/Library.h"
#include "../../Environment/Messages.h"
#include "../../Model/ModelInconsistencyException.h"
#include "../../Model/Cpp/Namespace.h"
#include "../../Model/Node.h"
#include "PippyParser.h"
#include "../Parser.h"
#include "../ParserException.h"
#include "../../Model/Cpp/Primitive.h"
#include "../../Model/Reason.h"
#include "../../Model/Source.h"
#include "../../Model/Type.h"
#include "../../Model/TypeArgument.h"
#include "../../Model/Cpp/TypeDef.h"
#include "../../Model/Cpp/Variable.h"
#include "../../Model/Cpp/VariableAssignment.h"
#include "../../Model/Cpp/TypeInfo.h"

//#include "PippyParser.spirit"
#include <sstream>
#include <string>

using Macaroni::Model::Cpp::Access;
using Macaroni::Model::AttributeTable;
using Macaroni::Model::AttributeTableInternalPtr;
using Macaroni::Model::AttributeTablePtr;
using Macaroni::Model::AttributeValue;
using Macaroni::Model::AttributeValueInternalPtr;
using Macaroni::Model::AttributeValuePtr;
using Macaroni::Model::Block;
using Macaroni::Model::Cpp::Class;
using Macaroni::Model::Cpp::ClassPtr;
using Macaroni::Model::Cpp::Constructor;
using Macaroni::Model::Cpp::ConstructorOverload;
using Macaroni::Model::Cpp::ConstructorOverloadPtr;
using Macaroni::Model::Cpp::ConstructorPtr;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using namespace Macaroni::Parser::Cpp;
using Macaroni::Model::Cpp::CppContext;
using Macaroni::Model::Cpp::CppContextPtr;
using Macaroni::Model::Cpp::Destructor;
using Macaroni::Model::Cpp::DestructorPtr;
using Macaroni::Model::FileName;
using Macaroni::Model::FileNamePtr;
using Macaroni::Model::Cpp::Function;
using Macaroni::Model::Cpp::FunctionOverload;
using Macaroni::Model::Cpp::FunctionOverloadPtr;
using Macaroni::Model::Cpp::FunctionPtr;
using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using Macaroni::Environment::Messages;
using Macaroni::Model::ModelInconsistencyException;
using Macaroni::Model::Cpp::Namespace;
using Macaroni::Model::Node;
using Macaroni::Model::NodeList;
using Macaroni::Model::NodeListPtr;
using Macaroni::Model::NodePtr;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;
using Macaroni::Parser::ParserException;
using Macaroni::Model::Cpp::Primitive;
using Macaroni::Model::Reason;
using Macaroni::Model::ReasonPtr;
using Macaroni::Model::Type;
using Macaroni::Model::TypeArgument;
using Macaroni::Model::TypeArgumentList;
using Macaroni::Model::TypeArgumentListPtr;
using Macaroni::Model::TypeArgumentPtr;
using Macaroni::Model::Cpp::Typedef;
using Macaroni::Model::Cpp::TypedefPtr;
using Macaroni::Model::TypeList;
using Macaroni::Model::TypeListPtr;
using Macaroni::Model::TypeModifiers;
using Macaroni::Model::TypePtr;
using Macaroni::Model::Cpp::Variable;
using Macaroni::Model::Cpp::VariableAssignment;
using Macaroni::Model::Cpp::TypeInfo;

BEGIN_NAMESPACE(Macaroni, Parser, Pippy)

typedef std::string::const_iterator StringItr;

void Assert(bool b)
{
	if (!b)
	{
		MACARONI_FAIL("Fail!");
	}
}

class Iterator
{
public:
	Iterator(StringItr itr, StringItr end, SourcePtr source)
		: column(source->GetColumn()),  endItr(end),
		fileName(source->GetFileName()),
		itr(itr), line(source->GetLine())
	{
		
	}

	inline void Advance(int count)
	{
		for(int i = 0; i < count; i ++)
		{
			if (itr == endItr)
			{
				MACARONI_ASSERT(false, "Advanced past end of string.");
			}
			if (*itr == '\n')
			{
				line ++;
				column = 0;
			}
			itr ++;
			column ++;
		}
	}

	// Returns current line.
	inline char Current()
	{
		MACARONI_ASSERT(!Finished(), "Cannot get Current() if finished.");
		return *itr;
	}

	inline bool ConsumeChar(const char c)
	{
		if (Is(c))
		{
			Advance(1);
			return true;
		}
		return false;
	}

	inline bool ConsumeWord(const char * word)
	{
		if (Is(word))
		{
			Advance(strlen(word));
			return true;
		}
		return false;
	}

	SourcePtr GetSource()
	{
		return Source::Create(fileName, line, column);
	}

	SourcePtr GetSource(int plusLines, int plusColumns)
	{
		return Source::Create(fileName, line + plusLines, column + plusColumns);
	}

	// Consumes whitespace. Returns amount consumed.
	inline int ConsumeWhitespace()
	{
		int consumed = 0;
		while(!Finished() && IsWhiteSpace())
		{
			consumed ++;
			Advance(1);
		}
		return consumed;
	}

	inline bool Finished()
	{
		return (itr == endItr);
	}

	inline bool Is(const char other)
	{
		return Finished() ? false : (*itr) == other;
	}

	inline bool Is(const char * string)
	{
		StringItr src = itr;
		
		while(src != endItr)
		{
			if (*string == '\0')
			{
				return true;
			}
			if (*src != *string)
			{
				return false;
			}
			src ++;
			string ++;			
		}

		return *string == '\0';
	}
	
	inline bool IsWhiteSpace()
	{
		return *itr <= 32;
	}

	static void IsTests()
	{
		std::string ok("12ComplexWord");
		FileNamePtr file = FileName::Create(std::string("Blah.mcpp"));
		SourcePtr src = Source::Create(file, 1, 1);
		Iterator itr(ok.begin(), ok.end(), src);

		Assert(itr.Is("12ComplexWord"));
		Assert(!itr.Is("2Complex"));
		Assert(itr.Is("12"));
		Assert(itr.Is("12Com"));
		Assert(itr.Is('1'));
		Assert(!itr.Is('2'));
		itr.Advance(1);
		Assert(itr.Is('2'));
		itr.Advance(1);
		Assert(itr.Is("Complex"));
		Assert(itr.Is("ComplexWord"));
		itr.Advance(7);
		Assert(itr.Is("Word"));
		Assert(!itr.Is("C"));
		try
		{
			itr.Advance(5);
			// Advances one past Word, going out of bounds.
			// Thus, an exception must be thrown.
			Assert(false); 
		}
		catch(Macaroni::Exception &)
		{
			// Good
		}
	}

	inline bool IsAlpha()
	{
		return Finished() ? false : IsAlpha(*itr);
	}

	static inline bool IsAlpha(const char c)
	{
		return (65 <= c && c <= 90) ||
				(97 <= c && c <= 122);
	}

	static void IsAlphaTests()
	{
		Assert(IsAlpha('a'));
		Assert(IsAlpha('z'));
		Assert(IsAlpha('m'));
		Assert(IsAlpha('A'));
		Assert(IsAlpha('Z'));
		Assert(IsAlpha('M'));
		Assert(!IsAlpha('1'));
		Assert(!IsAlpha('['));
		Assert(!IsAlpha('@'));
		Assert(!IsAlpha('\''));
		Assert(!IsAlpha('{'));
	}

	inline bool IsDigit()
	{
		return Finished() ? false : IsDigit(*itr);
	}
	
	static inline bool IsDigit(const char c)
	{
		return c >=48 && c <= 57;
	}

	static void IsDigitTests()
	{
		Assert(IsDigit('0'));
		Assert(IsDigit('1'));
		Assert(IsDigit('2'));
		Assert(IsDigit('3'));
		Assert(IsDigit('4'));
		Assert(IsDigit('5'));
		Assert(IsDigit('6'));
		Assert(IsDigit('7'));
		Assert(IsDigit('8'));
		Assert(IsDigit('9'));
		Assert(!IsDigit('a'));
		Assert(!IsDigit('!'));
		Assert(!IsDigit(':'));
		Assert(!IsDigit('/'));
	}
	
	static void RunTests()
	{
		IsAlphaTests();
		IsDigitTests();
		IsTests();
	}
private:
	int column;
	FileNamePtr fileName;
	StringItr itr;
	int line;	
	StringItr endItr;
};

static std::string unsafeStaticNightmareString;

static Iterator createTestItr(const char * msgChars)
{
	unsafeStaticNightmareString = std::string(msgChars);
	FileNamePtr file = FileName::Create(std::string("Blah.mcpp"));
	SourcePtr src = Source::Create(file, 1, 1);
	Iterator p(unsafeStaticNightmareString.begin(), 
			   unsafeStaticNightmareString.end(), 
			   src);
	return p;
}



class ParserFunctions
{
private:
	ContextPtr context;
	NodePtr currentScope;
	std::string hFilesForNewNodes;
	NodeListPtr importedNodes;
	LibraryPtr library;
public:

	ParserFunctions(ContextPtr context, LibraryPtr library)
		:context(context), 
		 currentScope(context->GetRoot()), 
		 importedNodes(new NodeList()),
		 library(library)
	{			
		MACARONI_ASSERT(!!CppContext::GetPrimitives(context),
			"Cpp nodes must be found to parse successfully.");
		NodePtr primitiveRoot = CppContext::GetPrimitives(context);
		for(unsigned int i = 0; i < primitiveRoot->GetChildCount(); i ++)
		{
			importedNodes->push_back(primitiveRoot->GetChild(i));
		}
	}

	/** Attempts to consume whatever access it can, returning 
	 * "Access_NotSpecified" if it finds nothing. */
	Access AccessKeyword(Iterator & itr)
	{
		using Macaroni::Model::Cpp::Access;

		ConsumeWhitespace(itr);
		Access access = Access::NotSpecified();
		if (itr.ConsumeWord("~hidden"))
		{
			access = Access::Hidden();
		}
		/*else if (itr.ConsumeWord("~inner"))
		{
			itr.ConsumeWhitespace();
			if (itr.ConsumeWord("protected"))
			{
				access = Access_Inner_Protected;
			}
			else
			{
				access = Access_Inner;
			}			
		}	*/	
		else if (itr.ConsumeWord("private"))
		{
			access = Access::Private();
		}
		else if (itr.ConsumeWord("protected"))
		{
			/*itr.ConsumeWhitespace();
			if (itr.ConsumeWord("~inner"))
			{
				access = Access_Inner_Protected;
			}
			else
			{
				access = Access_Protected;
			}*/
			access == Access::Protected();
		}
		else if (itr.ConsumeWord("public"))
		{
			access = Access::Public();
		}
		else
		{
			access = Access::NotSpecified();
		}
		return access;
	}

	void AddImport(NodePtr node)
	{
		importedNodes->push_back(node);
	}

	bool Attribute(Iterator & itr)
	{		
		if (!itr.ConsumeChar('@'))
		{
			return false;
		}
		// COMMITTED
		if (!this->currentScope)
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Attribute.AttributeFoundOutsideOfAnyScope"));
		}
		NodePtr name;
		if (!ConsumeNodeName(itr, name))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Attribute.NodeNameExpectedAfterAt"));
		}

		AttributeValueInternalPtr attributeValue;
		Iterator attrBegin = itr;
		if (AttributeValue_(itr, attrBegin, name, attributeValue))
		{
			itr.ConsumeWhitespace();
			// Changing this, because if its in a typedef or Variable its 
			// ambiguous.
			// itr.ConsumeChar(';'); // Consume this (its optional)
			currentScope->GetAttributes().Add(attributeValue);
			return true;
		}
		throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Attribute.ValueExpectedFollowingNodeName"));
	}	

	bool AttributeValue_(Iterator & itr, Iterator & attrBegin, 
						 const NodePtr & name, 
						 AttributeValueInternalPtr & attributeValue)
	{
		itr.ConsumeWhitespace();
		// At this point, can see the following:
		// bool - true or false,
		// node - another node name
		// number - a number literal
		// string - a string in the form "blah", {blah}, or (blah)
		// table - starts with [
		
		bool valueBool;
		double valueNumber;
		NodePtr valueNode;
		std::string valueString;
		if (AttributeValueBool(itr, valueBool))
		{	
			attributeValue = AttributeValueInternalPtr(new AttributeValue(name, valueBool, 
					Reason::Create(CppAxioms::AttributeValueCreation(), attrBegin.GetSource())
				));
		}
		else if (AttributeValueNumber(itr, valueNumber))
		{
			attributeValue = AttributeValueInternalPtr(new AttributeValue(name, valueNumber, 
					Reason::Create(CppAxioms::AttributeValueCreation(), attrBegin.GetSource())
				));
		}
		else if (AttributeValueNode(itr, valueNode))
		{
			attributeValue = AttributeValueInternalPtr(new AttributeValue(name, valueNode, 
					Reason::Create(CppAxioms::AttributeValueCreation(), attrBegin.GetSource())
				));
		}
		else if (AttributeValueString(itr, valueString))
		{
			attributeValue = AttributeValueInternalPtr(new AttributeValue(name, valueString, 
					Reason::Create(CppAxioms::AttributeValueCreation(), attrBegin.GetSource())
				));
		}
		else if (itr.ConsumeChar('['))
		{
			attributeValue = AttributeValueInternalPtr(new AttributeValue(name, 
					Reason::Create(CppAxioms::AttributeValueCreation(), attrBegin.GetSource())
				));			
			attributeTableContents(itr, attributeValue->GetValueAsTable());
		}
		else
		{
			// Short hand syntax allows for an empty attribute.
			// For now this is a table but it'd be cool if it could be nothing
			// to avoid the waste.
			attributeValue = AttributeValueInternalPtr(new AttributeValue(name, 
				Reason::Create(CppAxioms::AttributeValueCreation(), attrBegin.GetSource())
				));						
		}
		return !!attributeValue;
	}


	void attributeTableContents(Iterator & itr, const AttributeTablePtr & table)
	{
		itr.ConsumeWhitespace();
		while(!itr.Is(']'))
		{
			Iterator attrBegin = itr;
			std::string name;
			if (!ConsumeSimpleName(itr, name))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Attribute.EndBracketOrSimpleNameExpectedInsideAttributeTable"));
			}			
			itr.ConsumeWhitespace();
			if (!itr.ConsumeChar('='))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Attribute.TableEntryEqualsExpected"));
			}
			itr.ConsumeWhitespace();

			NodePtr home = table->GetHomeNode();
			NodePtr entryName = home->FindOrCreate(name);

			AttributeValueInternalPtr attributeValue;
			if (!AttributeValue_(itr, attrBegin, entryName, attributeValue))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Attribute.TableEntryValueExpected"));
			}
			table->Add(attributeValue);
			itr.ConsumeWhitespace();
			if (itr.ConsumeChar(',')) // optional
			{
				itr.ConsumeWhitespace();
			}				
		}
		itr.Advance(1);
	}

	bool AttributeValueBool(Iterator & itr, bool & value)
	{
		if (itr.ConsumeWord("true"))
		{
			value = true;
			return true;
		}
		if (itr.ConsumeWord("false"))
		{
			value = false;
			return true;
		}
		return false;
	}

	bool AttributeValueNode(Iterator & itr, NodePtr & value)
	{
		if (this->ConsumeNodeName(itr, value))
		{
			if (!value)
			{
				// Means it looked like a Node, but no Node with that
				// name was found.
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Attribute.ValueNoNodeByTheGivenNameFound"));
			}
			return true;
		}
		return false;
	}

	bool AttributeValueNumber(Iterator & itr, double & value)
	{
		return RealNumber(itr, value);		
	}

	bool AttributeValueString(Iterator & itr, std::string & value)
	{
		if (ConsumeStringLiteral(itr, value))
		{
			return true;
		}
		if (itr.ConsumeChar('='))
		{
			if (CodeBlock(itr, value))
			{
				return true;
			}
			else
			{
				throw ParserException(itr.GetSource(), Messages::Get(
					"CppParser.Attribute.CodeBlockExpectedAfterEquals"));
			}			
		}		
		return false;
	}	

	bool Block(Iterator & itr)
	{
		{
			Iterator newItr = itr;
			ConsumeWhitespace(newItr);
			if (!newItr.ConsumeWord("~block"))
			{
				return false;
			}

			// We're committed.
			itr = newItr;
		}
		
		itr.ConsumeWhitespace();
		std::string id;
		if (!ConsumeStringLiteral(itr, id))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.StringLiteralStartExpected"));		
		}
		itr.ConsumeWhitespace();
		std::string code;
		if (!CodeBlock(itr, code))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.CodeBlock.Expected"));
		}		
		NodePtr blockHome = createNextBlockNode(currentScope, itr);
		Block::Create(blockHome, id, code,
			Reason::Create(CppAxioms::BlockCreation(), itr.GetSource()));
		return true;
	}	

	// looks for "class [complexName]{}" Ignores whitespace.
	// Modifies itr argument if match is found.
	bool Class(Iterator & itr)
	{
		Iterator newItr = itr; 
		ConsumeWhitespace(newItr);
		if (!newItr.ConsumeWord("class"))
		{
			return false;
		}
		
		// Once the parser has seen "class" it must see the correct
		// grammar or there will be hell to pay.

		ConsumeWhitespace(newItr); 
		
		std::string name;
		if (!ConsumeComplexName(newItr, name))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Namespace.NoID1"));
		}		

		NodePtr oldScope = currentScope;
		currentScope = currentScope->FindOrCreate(name, hFilesForNewNodes);
		ClassPtr newClass = 
			Class::Create(library, currentScope, importedNodes,  
				Reason::Create(CppAxioms::ClassCreation(), newItr.GetSource()));

		ClassParents(newItr, newClass);

		ConsumeWhitespace(newItr);   

		while(Attribute(newItr));

		ConsumeWhitespace(newItr);   

		SourcePtr firstBraceSrc = newItr.GetSource();

		if (!newItr.ConsumeChar('{'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Class.NoOpeningBrace"));
			//EXCEPTION MSG: Expectedd { after namespace identifier.")
		}		

		ScopeFiller(newItr);

		ConsumeWhitespace(newItr);
		
		//Model::Cpp::Namespace::Create(currentScope,
		//	
	///		);
		
		ConsumeWhitespace(newItr);
		if (!newItr.ConsumeChar('}'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Class.NoEndingBrace", firstBraceSrc->GetLine()));			
		}

		// The ';' following a class def is optional.
		ConsumeWhitespace(newItr);
		newItr.ConsumeChar(';');
		
		itr = newItr; // Success! :)
		currentScope = oldScope;
		return true;
	}

	bool ClassParents(Iterator & itr, ClassPtr & newClass)
	{
		using namespace Macaroni::Model::Cpp;

		itr.ConsumeWhitespace();
		if (!itr.ConsumeChar(':')) {
			return false;
		}		
		do 
		{
			itr.ConsumeWhitespace();
			Access access = AccessKeyword(itr);
			if (access == Access::NotSpecified())
			{
				access = Access::Private();
			}			
			itr.ConsumeWhitespace();

			bool _virtual = itr.ConsumeWord("virtual");
			itr.ConsumeWhitespace();

			TypePtr type;
			if (!Type(itr, type))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Class.Inheritance.NoType"));
			}
			newClass->AddParent(type, access, _virtual);
			itr.ConsumeWhitespace();
		} while (itr.ConsumeChar(','));
		return true;
	}
	
	/** Consumes white space, then returns false if '{' not seen.
	 * If it is, consumes all text until next '}'. */
	bool CodeBlock(Iterator & itr, std::string & codeBlock)
	{
		ConsumeWhitespace(itr);
		if (!itr.Is('{'))
		{
			return false;
		}
		itr.Advance(1);
		
		std::stringstream ss;
		int depth = 1;
		while(depth > 0)
		{
			if (itr.Is('{'))
			{
				depth ++;
			}
			if (itr.Is('}'))
			{
				depth --;
				if (depth <= 0)
				{
					itr.Advance(1);
					break;
				}
			}
			char next = itr.Current();
			if (next == '\n')
			{
				ss << "\n";
			}
			else if (next == 9)
			{
				ss << "    ";
			}
			else if (next <= 32)
			{
				ss << ' ';
			}
			else
			{
				ss << next;
			}
			//ss << itr.Current();
			itr.Advance(1);
		}
		// Add code block
		codeBlock = ss.str();
		return true;
	}

	/** Returns true if a complex name can be parsed from this location. */
	static int ComplexName(Iterator itr)
	{
		int consumed = 0;
		int simpleNameLength;

		while(true)
		{
			if (itr.Is("::"))
			{
				itr.Advance(2);
				consumed += 2;
			}
			else if ((simpleNameLength = OperatorName(itr)) > 0)
			{
				consumed += simpleNameLength;
				itr.Advance(simpleNameLength);
				// We've seen an operator.  A name like
				// Macaroni::operator +::Parser is not valid, so once we've
				// seen one of these exit.
				return consumed; 
			}
			else if ((simpleNameLength = SimpleName(itr)) > 0)
			{
				consumed += simpleNameLength;
				itr.Advance(simpleNameLength);
			}			
			else
			{
				return consumed;
			}
		}
	}
	
	static void ComplexNameTests()
	{
		Assert(ComplexName(createTestItr("{dsf")) == 0);
		Assert(ComplexName(createTestItr("abc")) == 3);
		Assert(ComplexName(createTestItr("abc::b3")) == 7);
		Assert(ComplexName(createTestItr("ab_::b3")) == 7);
		Assert(ComplexName(createTestItr("a123::c3")) == 8);
		Assert(ComplexName(createTestItr("::cat::dog")) == 10);
		Assert(ComplexName(createTestItr("::cat::do{")) == 9);
		Assert(ComplexName(createTestItr("")) == 0);
		Assert(ComplexName(createTestItr("13")) == 0);
		Assert(ComplexName(createTestItr("::cat::13")) == 7);
	}

	bool ConstructorOrDestructor(Iterator & itr)
	{
		using namespace Macaroni::Model::Cpp;

		Iterator newItr = itr;
		
		Access access = AccessKeyword(newItr);
		
		if (access == Access::NotSpecified())
		{
			access = Access::Private();
		}

		bool isInline = InlineKeyword(newItr);

		ConsumeWhitespace(newItr);
		bool tilda = false;
		if (newItr.ConsumeChar('~'))
		{
			tilda = true;
			ConsumeWhitespace(newItr);
		}
		if (!newItr.ConsumeWord(currentScope->GetName().c_str()))
		{
			if (!tilda)
			{
				return false;
			}
			return false;
			/*if (newItr.ConsumeWord("global"))
			{
				// Oh, it wasn't trying to define a destructor, it was just the
				// friendly global keyword! How silly of us!
				return false;
			}
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Constructor.ClassNameExpected")); */
		}		
		ConsumeWhitespace(newItr);
		
		if (!newItr.ConsumeChar('('))
		{
			if (tilda)
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser::Constructor::ArgumentListExpected"));
			}
			else
			{
				return false;
			}
		}

		std::string nodeName(!tilda ? "$ctor" : "$dtor");
		NodePtr ctorNode = currentScope->FindOrCreate(nodeName);		

		// Create CTOR or DTOR
		FunctionOverloadPtr fOlPtr;

		if (!tilda)
		{
			ReasonPtr ctorReason = Reason::Create(CppAxioms::CtorCreation(), 
												  itr.GetSource());
			ConstructorPtr ctor = Constructor::Create(ctorNode, ctorReason);			
			//isInline, access, 
			ConstructorOverloadPtr ctorOl =
				ConstructorOverload::Create(ctor, isInline, access, ctorReason); 
			fOlPtr = boost::dynamic_pointer_cast<FunctionOverload>(ctorOl);
		} // end !tilda
		else
		{
			DestructorPtr dtor = Destructor::Create(ctorNode,  isInline, access,
				Reason::Create(CppAxioms::DtorCreation(), itr.GetSource()));
			fOlPtr = dtor->GetFunctionOverload();
		}
		// End create

		NodePtr oldScope = currentScope;
		currentScope = fOlPtr->GetNode(); //ctorNode;
		
			FunctionArgumentList(newItr);

		currentScope = oldScope;

		if (!tilda)
		{
			//ReasonPtr ctorReason = Reason::Create(CppAxioms::CtorCreation(), 
			//									  itr.GetSource());
			//ConstructorPtr ctor = Constructor::Create(ctorNode, ctorReason);			
			////isInline, access, 
			//ConstructorOverloadPtr ctorOl =
			//	ConstructorOverload::Create(ctor, isInline, access, ctorReason); 
			//fOlPtr = boost::dynamic_pointer_cast<FunctionOverload>(ctorOl);

			ConsumeWhitespace(newItr);
			if (newItr.ConsumeChar(':'))
			{
				// Look for assignments to class vars, such as x(4), y(3)
				do
				{
					ConsumeWhitespace(newItr);				
					std::string varName;
					if (!ConsumeComplexName(newItr, varName))
					{
						throw ParserException(newItr.GetSource(),
							Messages::Get("CppParser.Constructor.VariableInitializer"));
					}

					NodePtr varNode = currentScope->FindOrCreate(varName);

					ConsumeWhitespace(newItr);

					if (!newItr.ConsumeChar('('))
					{
						throw ParserException(newItr.GetSource(),
							Messages::Get("CppParser.Constructor.NoInitializerExpr"));
					}
					
					std::string exprCode;
					ConsumeExpression(newItr, ")", exprCode);
					newItr.Advance(1);

					VariableAssignment va;
					va.Expression = exprCode;
					va.Variable = varNode;
					ConstructorOverloadPtr ctorOl =
						boost::dynamic_pointer_cast<ConstructorOverload>(fOlPtr);
					ctorOl->AddAssignment(va);

					ConsumeWhitespace(newItr);
				} while(newItr.ConsumeChar(','));
			}
		} // end !tilda
		///*else
		//{
		//	DestructorPtr dtor = Destructor::Create(ctorNode,  isInline, access,
		//		Reason::Create(CppAxioms::DtorCreation(), itr.GetSource()));
		//	fOlPtr = dtor->GetFunctionOverload();
		//}*/

		std::string codeBlock;
		bool codeAttached = false;
		Iterator startOfCodeBlock = newItr;
		codeAttached = CodeBlock(newItr, codeBlock);
		if (!codeAttached)
		{
			ConsumeWhitespace(newItr);
			if (!newItr.ConsumeChar(';'))
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Function.SemicolonExpected")); 
			}
		}
		
		if (codeAttached)
		{
			fOlPtr->SetCodeBlock(codeBlock, startOfCodeBlock.GetSource());
		}
		itr = newItr;
		return true;
	}

	bool ConsumeComment(Iterator & itr)
	{
		itr.ConsumeWhitespace();
		if (itr.Finished() || itr.Current() != '/')
		{
			return false;
		}
		Iterator cmtItr = itr;
		cmtItr.Advance(1);
		if (cmtItr.Finished())
		{
			return false;
		}
		if (cmtItr.Current() == '/')
		{
			itr.Advance(1);
			while(!itr.Finished() && itr.Current() != '\n')
			{
				itr.Advance(1);
			}
			return true;
		}
		else if (cmtItr.Current() == '*')
		{
			itr.Advance(1);			
			bool seenStar = false;
			bool commentFinished = false;
			while(!itr.Finished() && !commentFinished)
			{
				if (seenStar && itr.Current() == '/')
				{
					commentFinished = true;
				}
				if (itr.Current() == '*')
				{
					seenStar = true;
				}
				else
				{
					seenStar = false;
				}
				itr.Advance(1);
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	void ConsumeWhitespace(Iterator & itr)
	{
		while(!itr.Finished() && ConsumeComment(itr)){}
	}

	/** If no complex name found, nothing happens. 
	 * If one is found, it is consumed and the name itself is placed in result. 
	 */
	static bool ConsumeComplexName(Iterator & itr, std::string & result)
	{
		int length = ComplexName(itr);
		if (length < 1)
		{
			return false;
		}
		std::stringstream ss;
		for(int i = 0; i < length; i ++)
		{
			// operator overloads may put spaces here...
			// For the purposes of Macaroni generators, the canonical version
			// should be used.
			if (!itr.IsWhiteSpace()) 
			{
				ss << itr.Current();
			}			
			itr.Advance(1);
		}
		result = ss.str();
		return true;
	}

	/** Reads through an arbitrary block of code, stopping at one of the provided
	 *  chars.  StopAtChars must be terminated with \0.
	 *  Does NOT return on ) or } if these close a block; for example, you
	 *  must push the iterator to past the first { if you plan on stopping at
	 *  }, or else the parser will simply think the { is new and increase
	 *  its depth.
	 **/
	void ConsumeExpression(Iterator & itr, char * stopAtChars, std::string & code)
	{
		Iterator exceptionItr = itr;

		std::vector<char> blocks;
		std::stringstream ss;

		while(!itr.Finished())
		{			
			char c = itr.Current();
			if (c == '(')
			{
				blocks.push_back('(');
			} 
			else if (c == '{')
			{
				blocks.push_back('{');
			}
			else if (	blocks.size() > 0 &&
						(
							(blocks.back() == '(' && c == ')') 
							||
							(blocks.back() == '{' && c == '}') 
						)
					)
			{
				blocks.pop_back();
			}
			else 
			{
				for (int stopAtI = 0; stopAtChars[stopAtI] != '\0'; stopAtI ++)
				{
					if (c == stopAtChars[stopAtI])
					{
						code = ss.str();
						//itr.Advance(1); //2010-01-03 Changed to not advance past char.
						return;
					}
				}
			}
			
			ss << c;
			itr.Advance(1);	
		}

		throw ParserException(exceptionItr.GetSource(), 
			Messages::Get("CppParser.Expression.CouldNotMatch")); 
	}

	bool ConsumeFilePath(Iterator & itr, std::string & filePath)
	{
		std::stringstream ss;
		bool ankles = false;
		if (itr.Current() == '<')
		{
			ankles = true;
		}
		else if (!(itr.Current() == '"'))
		{
			return false;
		}
		ss << itr.Current();
		itr.Advance(1);
		
		while(!itr.Finished() && itr.Current() !='\"' && itr.Current() !='>')
		{
			ss << itr.Current();
			itr.Advance(1);
		}
		
		if (itr.Finished())
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.FileNameExpected"));
		}
		ss << itr.Current();
	
		//TO-DO: Check for > if ankels is true and " if not true, and make sure
		// there's no mismatch.

		itr.Advance(1);
		filePath = ss.str();
		return true;
	}

	/** Attempts to consume a name, and then stores the Node by that name.
	 * Returns false if either no complex name was there, and sets node to nullptr
	 * if it can't find a node with that name.
	 * IOW, be aware this may return true but not have found a node. */
	bool ConsumeNodeName(Iterator & itr, NodePtr & node)
	{
		node = NodePtr();
		Iterator newItr = itr;
		std::string name;	
		if (!ConsumeComplexName(newItr, name))
		{
			return false;
		}
		if (name == "std::string")
		{
			int five = 5;
		}
		node = FindNode(name);
		if (!!node)
		{ 
			// Advance only if Node was found.
			itr = newItr;
		}
		return true;
	}

	/** If no simple name found, nothing happens. 
	 * If one is found, it is consumed and the name itself is placed in result. 
	 * Btw, if this finds a Complex name it stops right at :: and returns
	 * true, so be prepared for that sucka.
	 */
	static bool ConsumeSimpleName(Iterator & itr, std::string & result)
	{
		int length = SimpleName(itr);
		if (length < 1)
		{
			return false;
		}
		std::stringstream ss;
		for(int i = 0; i < length; i ++)
		{
			ss << itr.Current();
			itr.Advance(1);
		}
		result = ss.str();
		return true;
	}

	/** Searches for "anything". Will move itr, and places result in rtnString. */
	bool ConsumeStringLiteral(Iterator & itr, std::string & rtnString)
	{
		std::stringstream ss;
		if (!(itr.Current() == '"'))
		{
			return false;
		}		
		itr.Advance(1); // skip first "
		
		while(!itr.Finished() && itr.Current() !='\"')
		{
			ss << itr.Current();
			itr.Advance(1);
		}
		
		if (itr.Finished())
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.StringLiteralEndingExpected"));
		}				
		itr.Advance(1); // skip last "
		rtnString = ss.str();
		return true;
	}

	/** Looks for a node name with type arguments either at the end of in the middle.
	 * Example: a::b<c>, a::b::d, a<b,c>::b<a::b::c>::e<a *,const e::g> 
	 * Returns false if it does not find a name.
	 */
	bool ConsumeTypeMainNodeAndArguments(Iterator & itr, NodePtr & mainNode, 
										 TypeArgumentListPtr & typeArgumentList)
	{
		typeArgumentList.reset(new TypeArgumentList());

		NodePtr lastNode;
		
		while(!mainNode)
		{
			ConsumeWhitespace(itr);
			std::string complexName;
			if (!ConsumeComplexName(itr, complexName))
			{
				return false;
			}
			ConsumeWhitespace(itr);

			NodePtr node;
			if (!lastNode) // The first iteration of the loop?
			{				
				node = FindNode(complexName);
				if (!node)
				{
					return true;
				}
			}
			else
			{
				// If we've already seen a node (and a type arg list) then the
				// main node has to come off of it.
				node = lastNode->FindOrCreate(complexName);
			}

			if (itr.Current() == '<')
			{
				itr.ConsumeChar('<');
				TypeListPtr list = ConsumeTypeDefinitionList(itr);
				TypeArgumentPtr arg(new TypeArgument(node, list));
				typeArgumentList->push_back(arg);
				
				itr.ConsumeWhitespace();
				if (itr.ConsumeWord("::")) // More to come, looks like.
				{
					lastNode = node;
					continue; // SO tempted to just use a GOTO here... too lazy to decompose method.
				}
			}	

			mainNode = node;
		}

		return true;
	}

	
	
	/** This is called to find lists of type defs, as can be seen in ankle
	 * brackets.  Throws if it can't find type argument.   Ends when it
	 * sees ">". */
	TypeListPtr ConsumeTypeDefinitionList(Iterator & itr)
	{
		TypeListPtr typeList(new TypeList());
		
		while(true) // Remember, its not as bad as GOTO because its WHILE. :p
		{
			Iterator newItr = itr;
			TypePtr type;
			if (!Type(newItr, type))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Type.TypeDefinitionArgumentExpected"));
			}
			typeList->push_back(type);
			itr = newItr;
			ConsumeWhitespace(itr);
			if (itr.Current() == '>')
			{
				itr.ConsumeChar('>');
				return typeList;
			}
			else if (itr.Current() == ',')
			{
				itr.ConsumeChar(',');
				// Continue loop.
			}
			else
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Type.TypeDefinitionCommaOrClosingBracketExpected"));
			}
		}			
	}

	NodePtr createNextBlockNode(NodePtr node, Iterator & itr)
	{
		const int maxBlocks = 1024;
		if (!node)
		{
			throw Macaroni::Exception("This function does not accept a null node.");
		}
		for(int index = 0; index < maxBlocks; index ++)
		{
			std::stringstream ss;
			ss << "~#block" << index;
			std::string name = ss.str();
			NodePtr next = node->Find(name);
			if (!next)
			{
				return node->FindOrCreate(name);
			}
		}
		std::stringstream ss;
		ss << "The node " << node->GetFullName() 
			<< " has exceded the maximum number of blocks allowed by the "
			<< "Parser (" << maxBlocks << ").";
		throw ParserException(itr.GetSource(), ss.str());		
	}

	bool Directives(Iterator & itr)
	{
		Iterator newItr = itr;
		ConsumeWhitespace(newItr);
		if (newItr.Finished() || !(newItr.Current() == '~'))
		{
			return false;
		}
		newItr.Advance(1);
		if (!Import(newItr) && !HFileDirective(newItr))
		{
			return false;
			// For awhile now, ~ does not necessarily indicate a directive,
			// but a keyword unique to Macaroni.
			// So we certainly don't want to throw...
			//throw ParserException(itr.GetSource(),
			//	Messages::Get("CppParser.Directive.Unknown"));
		}
		itr = newItr;
		return true;
	}

	/** The start of a doc.  Either it reads stuff in or throws an exception. */
	void Document(Iterator itr)
	{
		ScopeFiller(itr);
		ConsumeWhitespace(itr);
		if (!itr.Finished())
		{
			throw ParserException(itr.GetSource(), Messages::Get("CppParser.Document.SyntaxError"));
		}
	}

	/** Given a Node, crawls up the Node tree to find the first
	 * parent (or the given node itself) which is a Class. */
	NodePtr FindClassAncestorOfNode(NodePtr node)
	{
		if (!node)
		{
			return NodePtr();
		}
		if (Class::IsInstance(node->GetMember()))
		{
			return node;
		}
		return FindClassAncestorOfNode(node->GetNode());
	}

	/** Given a Node, crawls up the Node tree to find the first
	 * parent (or the given node itself) which is a Namespace. */
	NodePtr FindNamespaceAncestorOfNode(NodePtr node)
	{
		if (!node)
		{
			return NodePtr();
		}
		if (Namespace::IsInstance(node->GetMember()))
		{
			return node;
		}
		return FindNamespaceAncestorOfNode(node->GetNode());
	}

	/** Takes a complex name and finds its node. */
	NodePtr FindNode(std::string & complexName)
	{
		/*
		// The Node class really needs a routine to discover a name given the
		// current scope.  The visibility rules are too complex to
		// lay out here.

		// TO-DO: Create method in Node class called "FindNodeUsingVisibilityRules"
		// For now, just do this:
		NodePtr result;
		result = currentScope->Find(complexName);
		if (!result)
		{
			result = FindNodeFromImports(complexName);
		}		
		if (!result)
		{
			if (complexName == currentScope->GetFullName() ||
				complexName == currentScope->GetName())
			{
				return currentScope;
			}
		}
		return result;*/
		return FindNodeFromScope(complexName);
	}

	NodePtr FindNodeFromImports(std::string & complexName)
	{
		std::string firstPart, lastPart;
		Node::SplitFirstNameOffComplexName(complexName, firstPart, lastPart);
		for (unsigned int i = 0 ; i < importedNodes->size(); i ++)
		{
			NodePtr & imp = (*importedNodes)[i];
			if (firstPart == imp->GetName())
			{
				if (lastPart.size() < 1)
				{
					return imp;
				}
				return imp->Find(lastPart);
			}
		}		
		return NodePtr();
	}

	NodePtr FindNodeFromImportsOrScope(std::string & complexName)
	{
		NodePtr rtnValue = FindNodeFromImports(complexName);
		NodePtr scopeItr = currentScope;
		if (complexName == currentScope->GetName() || 
			complexName == currentScope->GetFullName())
		{
			return currentScope;
		}
		while (!rtnValue && !!scopeItr)
		{
			rtnValue = scopeItr->Find(complexName);
			scopeItr = scopeItr->GetNode();
		}
		return rtnValue;
	}
	
	/** Finds a node using visibility rules.
	 * First checks the Class, then the namespace, and finally imports.
	 */
	NodePtr FindNodeFromScope(std::string & complexName)
	{
		NodePtr scope = currentScope;
		NodePtr classNode = FindClassAncestorOfNode(scope);
		NodePtr namespaceNode = FindNamespaceAncestorOfNode(scope);

		NodePtr rtn;
		// Check class first,
		rtn = FindNodeFromNode(classNode, complexName);
		// then namespace,
		if (!rtn) 
		{
			rtn = FindNodeFromNode(namespaceNode, complexName);
			if (!rtn)
			{
				rtn = FindNodeFromImports(complexName);
			}
		}
		if (!rtn)
		{
			// 2010-03-28 - Adding this code may break something as I haven't
			// been thinking about this section lately but I think this will
			// fix it so you can use long names for Nodes...
			rtn = this->context->GetRoot()->Find(complexName);
		}
		return rtn;
	}	

	/** Searchs a node for the given complex name. */
	NodePtr FindNodeFromNode(NodePtr scope, std::string & complexName)
	{				
		if (!scope)
		{
			return NodePtr();
		}
		NodePtr scopeItr = scope;
		if (complexName == scope->GetName() || 
			complexName == scope->GetFullName())
		{
			return scope;
		}
		if (!!scope->GetAdoptedHome())
		{
			if (complexName == scope->GetAdoptedHome()->GetName() &&
				complexName == scope->GetAdoptedHome()->GetFullName())
			{
				return scope->GetAdoptedHome();
			}
		}
		NodePtr rtnValue;
		while (!rtnValue && !!scopeItr)
		{
			rtnValue = scopeItr->Find(complexName);
			scopeItr = scopeItr->GetNode();
		}
		return rtnValue;
	}	



	static void FindNodeTest()
	{
		ContextPtr c(new Context("{ROOT}"));
		LibraryPtr l = c->FindOrCreateLibrary("Tests", "FindNodeTest", "1");
		CppContext::CreateCppNodes(c);
		ParserFunctions funcs(c, l);

		// It is found.
		NodePtr nodeInt = funcs.FindNode(std::string("signed int"));
		Assert(nodeInt->GetFullName() == CppContext::GetPrimitives(c)->Find("signed int")->GetFullName());
		
		// int is found, but nothing is found beyond that.
		NodePtr nodeInt2 = funcs.FindNode(std::string("signed int::something"));
		Assert(nodeInt2 == NodePtr());

		NodePtr seed = c->GetRoot()->FindOrCreate(std::string("Fruit::Orange::Seed"));
		NodePtr orange = seed->GetNode();
		NodePtr fruit = orange->GetNode();
		funcs.AddImport(orange);
		NodePtr foundSeed = funcs.FindNode(std::string("Orange::Seed"));
		Assert(foundSeed->GetFullName() == seed->GetFullName());
		
		NodePtr foundFruit = funcs.FindNode(std::string("Fruit"));
		Assert(foundFruit->GetFullName() == fruit->GetFullName());
	}
		
	static void FindNodeFromImportsTest()
	{
		ContextPtr c(new Context("{ROOT}"));
		CppContext::CreateCppNodes(c);
		LibraryPtr l = c->FindOrCreateLibrary("Tests", "FindNodeFromImportsTest", "1");
		ParserFunctions funcs(c, l);

		// It is found.
		NodePtr nodeInt = funcs.FindNodeFromImports(std::string("signed int"));
		Assert(nodeInt->GetFullName() == CppContext::GetPrimitives(c)->Find("signed int")->GetFullName());
		
		// int is found, but nothing is found beyond that.
		NodePtr nodeInt2 = funcs.FindNodeFromImports(std::string("signed int::something"));
		Assert(nodeInt2 == NodePtr());

		NodePtr orange = c->GetRoot()->FindOrCreate(std::string("Fruit::Orange"));
		NodePtr fruit = orange->GetNode();
		funcs.AddImport(fruit);
		NodePtr foundOrange = funcs.FindNodeFromImports(std::string("Fruit::Orange"));
		Assert(foundOrange->GetFullName() == orange->GetFullName());
	}

	bool FriendDeclaration(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		if (!itr.ConsumeWord("friend"))
		{
			return false;
		}

		ClassPtr classPtr;
		if (!!currentScope->GetMember())
		{
			classPtr = boost::dynamic_pointer_cast<Macaroni::Model::Cpp::Class>(currentScope->GetMember());
		}
		if (!classPtr)
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Friend.FriendKeywordOnlyForClasses"));
		}

		ConsumeWhitespace(itr);
		std::string friendName;
		ConsumeComplexName(itr, friendName);
		NodePtr friendNode = FindNodeFromImportsOrScope(friendName);
		if (!friendNode)
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Friend.CouldNotFindNode"));
		}
		ConsumeWhitespace(itr);
		if (!itr.ConsumeChar(';'))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Friend.MissingSemicolon"));
		}
		classPtr->AddFriend(friendNode);
		return true;
	}

	/** Attempts to consume "~friend". Moves itr. */
	bool FriendModifierKeyword(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		return itr.ConsumeWord("~friend");		
	}

	/** This function expects us to be committed to finding a function and to
	 * have seen '('. We parse until we see ')'. */
	void FunctionArgumentList(Iterator & itr)//, TypeInfo & rtnTypeInfo, 
							  //std::string & name)
	{
		ConsumeWhitespace(itr);
		bool seenArg = false;
		while(!itr.ConsumeChar(')'))
		{	
			Iterator oldItr = itr;
			Access access = Access::NotSpecified();
			bool _friend;
			bool global;
			bool isInline;
			std::string initializer;
			bool isStatic;
			TypePtr type;
			std::string argName;
			if ((seenArg && !itr.ConsumeChar(','))
				||
				!Variable(itr, access, _friend, global, isInline, isStatic, type, argName))
			{
				throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Function.ExpectedEndingParenthesis"));
			}
			itr.ConsumeWhitespace();
			if (itr.ConsumeChar('='))
			{
				ConsumeExpression(itr, ",)", initializer);
			}
			if (_friend)
			{
				throw ParserException(oldItr.GetSource(),
					"CppParser.Variable.FriendNotAllowedForArg");
			}
			if (global)
			{
				throw ParserException(oldItr.GetSource(),
					"CppParser.Variable.GlobalNotAllowedForArg");
			}
			if (isInline)
			{
				throw ParserException(oldItr.GetSource(),
					"CppParser.Variable.InlineNotAllowedForVariable");
			}
			if (isStatic)
			{
				throw ParserException(oldItr.GetSource(),
					"CppParser.Variable.StaticNotAllowedForArg");
			}

			NodePtr node = currentScope->FindOrCreate(argName);
			Variable::Create(node, access, isStatic, type, initializer,
				Reason::Create(CppAxioms::VariableScopeCreation(), oldItr.GetSource()));
			seenArg = true;
		}
	}

	bool GlobalKeyword(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		return itr.ConsumeWord("~global");
	}

	bool HFileDirective(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		if (!itr.ConsumeWord("hfile"))
		{
			return false;
		}
		ConsumeWhitespace(itr);
		if (itr.ConsumeWord("reset"))
		{
			hFilesForNewNodes = "";
			return true;
		}
		if (!itr.ConsumeChar('='))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Directive.HFileBadArgument"));
		}
		ConsumeWhitespace(itr);
		std::string filePath;
		if (!ConsumeFilePath(itr, filePath))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Directive.HFileFilePathExpected"));
		}
		FileNamePtr fileName = FileName::Create(filePath);
		ReasonPtr reason = Reason::Create(CppAxioms::SetExistingHFilePath(), itr.GetSource());
		this->currentScope->SetHFilePath(reason, fileName);
		return true;
	}

	// looks for "import [complexName];" 
	// Modifies itr argument if match is found.
	bool Import(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		if (!itr.ConsumeWord("import"))
		{
			return false;
		}

		// Now we're committed!
		ConsumeWhitespace(itr);
		
		std::string name;
		if (!ConsumeComplexName(itr, name))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Import.NameNotFound"));
		}
		
		ConsumeWhitespace(itr);
		if (!itr.ConsumeChar(';'))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Import.SemicolonExpected"));
		}

		NodePtr importNode = context->GetRoot()->FindOrCreate(name);
		AddImport(importNode);
		return true;
	}

	bool InlineKeyword(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		return itr.ConsumeWord("inline");
	}

	// looks for "namespace [complexName]{}" Ignores whitespace.
	// Modifies itr argument if match is found.
	bool Namespace(Iterator & itr)
	{
		Iterator newItr = itr; 
		ConsumeWhitespace(newItr);
		if (!newItr.ConsumeWord("namespace"))
		{
			return false;
		}
		
		// Once the parser has seen "namespace" it must see the correct
		// grammar or there will be hell to pay.

		ConsumeWhitespace(newItr);
		
		std::string name;
		if (!ConsumeComplexName(newItr, name))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Namespace.NoID1"));
		}		

		NodePtr oldScope = currentScope;
		currentScope = currentScope->FindOrCreate(name);
		
		// Create any parent namespaces that may have been introduced;
		// for example, if we parse "A::B", A is also a namespace
		// because only Namespaces can contain Namespaces.
		NodePtr ns = currentScope;
		while(ns != oldScope)
		{
			if (ns->GetMember() == nullptr)
			{
				Namespace::Create(library, ns, 
					Reason::Create(CppAxioms::NamespaceCreation(), newItr.GetSource()));
			}
			ns = ns->GetNode();
		}

		ConsumeWhitespace(newItr);

		SourcePtr firstBraceSrc = itr.GetSource();

		if (!newItr.ConsumeChar('{'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Namespace.NoOpeningBrace"));
			//EXCEPTION MSG: Expectedd { after namespace identifier.")
			return false;
		}		

		ScopeFiller(newItr);

		ConsumeWhitespace(newItr);
		
		//Model::Cpp::Namespace::Create(currentScope,
		//	
	///		);
		
		ConsumeWhitespace(newItr);
		if (!newItr.ConsumeChar('}'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Namespace.NoEndingBrace", firstBraceSrc->GetLine()));			
		}
		
		itr = newItr; // Success! :)
		currentScope = oldScope;
		return true;
	}

	// Parses "operator blah", where blah is the operator being overloaded.
	// Its necessary to do it this way because "operatorFunctions" is a valid
	// name but "operator+" refers to "operator +".  So we have to actually
	// check the operator and the keyword here.
	static int OperatorName(Iterator & itr)
	{		
		Iterator myItr = itr;
		if (!myItr.ConsumeWord("operator"))
		{
			return 0;			
		}
		int consumed = 8;
		bool committed = myItr.IsWhiteSpace();
		consumed += myItr.ConsumeWhitespace();
		if (OverloadableOperator(myItr, consumed))
		{
			return consumed;
		}
		else if (committed)
		{
			throw ParserException(myItr.GetSource(),
				Messages::Get("CppParser.Operator.InvalidOperator")); 
		}
		else
		{
			return 0;
		}
	}

	// If the itr lies at an overloadable operator, parses through it and
	// returns the consumed count, then returns true.  Otherwise returns false.
	static bool OverloadableOperator(Iterator & itr, int & consumed)
	{
		int consume = 0;
		if (itr.ConsumeWord("new[]"))
		{
			consume = 5;
		}
		else if (itr.ConsumeWord("new"))
		{
			consume = 3;
		}		
		else if (itr.ConsumeWord("delete[]"))
		{
			consume = 8;
		}
		else if (itr.ConsumeWord("delete"))
		{
			consume = 6;
		}	
		else if (itr.ConsumeWord("[]")
			|| itr.ConsumeWord("->")
			|| itr.ConsumeWord("==")
			|| itr.ConsumeWord("!=")
			|| itr.ConsumeWord(">=")
			|| itr.ConsumeWord("<=")
			|| itr.ConsumeWord("&&")
			|| itr.ConsumeWord("||")
			|| itr.ConsumeWord("++")
			|| itr.ConsumeWord("--")
			|| itr.ConsumeWord("+=")
			|| itr.ConsumeWord("-=")
			|| itr.ConsumeWord("*=")
			|| itr.ConsumeWord("/=")
			|| itr.ConsumeWord("<<")
			|| itr.ConsumeWord(">>"))
		{
			consume = 2;
		}
		else if (itr.ConsumeChar('+')
			|| itr.ConsumeChar('-')
			|| itr.ConsumeChar('*')
			|| itr.ConsumeChar('/')
			|| itr.ConsumeChar('%')
			|| itr.ConsumeChar('^')
			|| itr.ConsumeChar('|')
			|| itr.ConsumeChar('&')
			|| itr.ConsumeChar('&')
			|| itr.ConsumeChar('~')			
			|| itr.ConsumeChar('<')
			|| itr.ConsumeChar('>')
			|| itr.ConsumeChar('!')
			|| itr.ConsumeChar('=')
			)
		{
			consume = 1;
		}		
		else
		{
			return false;
		}
		consumed += consume;
		return true;
	}

	// Looks for a number beginning at the iterator exactly.
	// If it sees ANYTHING in the form "1" or "." etc it will advance
	// the iterator.  It returns IMMEDIETELY following the number.
	bool RealNumber(Iterator & itr, double & number)
	{		
		Iterator newItr = itr;
		std::stringstream ss;
		bool seenDot = false;
		bool seenDigit = false;
		bool isDigit;
		while(true)
		{
			isDigit = false;
			char c = newItr.Current();
			if (c >= '0' && c <= '9')
			{
				isDigit = true;
				seenDigit = true;
			}
			else if (!seenDot && c == '.')
			{
				seenDot = true;
				isDigit = true;
			}
			else
			{
				break;
			}
			ss << c;
			newItr.Advance(1);
		}
		if (seenDigit) 
		{
			
			ss.seekg(std::ios_base::beg);
			ss >> number;
			itr = newItr;
			return true;
		}
		return false;
	}

	// Very destructive - takes the itr and attempts to consume
	// anything that could be fit in a Scope.
	void ScopeFiller(Iterator & itr)
	{	
		// Take whatever you can get, replace the iter
		while (Directives(itr) 
				|| Block(itr)
				|| ConstructorOrDestructor(itr)				
				|| FriendDeclaration(itr)
				|| Namespace(itr) 
				|| Class(itr)
				|| Typedef(itr)
				|| VariableOrFunction(itr)
				|| Attribute(itr)
				)
		{
		}
	}

	static int SimpleName(Iterator itr)
	{		
		if (itr.Finished() || (!itr.IsAlpha() && !itr.Is('_')))
		{
			return 0;
		}
		int consumed = 1;
		itr.Advance(1);
		while(!itr.Finished() && 
			  (itr.IsAlpha() || itr.IsDigit() || itr.Is("_"))
			  )
		{
			consumed ++;
			itr.Advance(1);
		}
		return consumed;
	}

	static void SimpleNameTests()
	{
		Assert(SimpleName(createTestItr("1")) == 0);
		Assert(SimpleName(createTestItr("_")) == 1);
		Assert(SimpleName(createTestItr("a")) == 1);
		Assert(SimpleName(createTestItr("1a")) == 0);
		Assert(SimpleName(createTestItr("a1")) == 2);
		Assert(SimpleName(createTestItr("_1")) == 2);
		Assert(SimpleName(createTestItr("a_")) == 2);
		Assert(SimpleName(createTestItr("{1")) == 0);
		Assert(SimpleName(createTestItr("a/")) == 1);
		Assert(SimpleName(createTestItr("a/")) == 1);
		Assert(SimpleName(createTestItr("_1:")) == 2);
		Assert(SimpleName(createTestItr("AVeryLongName2_3a:")) == 17);
	}

	bool StaticKeyword(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		return itr.ConsumeWord("static");
	}

	/** Consumes type info into the form of Type, in the format of
	 * [const] [TypeNodeAndArgs] [*] [const] [&]
	 * and sets the new (or, potentially later, found) Type object in "info."
	 * This includes even nested template instances.
	 * Moves itr.
	 */
	bool Type(Iterator & itr, TypePtr & info)
	{
		Iterator newItr = itr;

		NodePtr mainNode;
		TypeModifiers modifiers;
		TypeArgumentListPtr typeArgumentList;

		ConsumeWhitespace(newItr);		
		if (newItr.ConsumeWord("const"))
		{
			modifiers.Const = true;
			ConsumeWhitespace(newItr);
		}

		Iterator beforeTypeInfoItr = newItr;
		if (!ConsumeTypeMainNodeAndArguments(newItr, mainNode, typeArgumentList))
		{
			if (modifiers.Const)
			{
				// If we saw const, they're committed.
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Variable.ConstMaybeBeforeVar")); 
			}
			else
			{
				// Didn't see false and that didn't look like a typename, so
				// we're fine.
				return false;
			}
		}
				
		// At this point, we're committed, so start using the actual itr passed 
		// in.
		if (!mainNode) // The function could not find the node.
 		{
			throw ParserException(beforeTypeInfoItr.GetSource(),
				Messages::Get("CppParser.Variable.UnknownTypeName")); 
		}

		itr = newItr;

		ConsumeWhitespace(itr);
		if (itr.ConsumeWord("const"))
		{
			if (modifiers.Const)
			{
				// Appeared twice!? How dare it!
				throw ParserException(itr.GetSource(0, -5),
					Messages::Get("CppParser.Variable.ConstSeenTwice")); 
			}
			modifiers.Const = true;
			ConsumeWhitespace(itr);
		}

		// Now, we check for reference.
		if (itr.ConsumeChar('*'))
		{
			modifiers.Pointer = true;
			ConsumeWhitespace(itr);
			if (itr.ConsumeWord("const"))
			{
				modifiers.ConstPointer = true;				
				ConsumeWhitespace(itr);
			}
		}

		// Now, we check for reference.
		if (itr.ConsumeChar('&'))
		{
			modifiers.Reference = true;
			ConsumeWhitespace(itr);
		}

		// At this point we can be sure we've seen everything.
		info.reset(new Model::Type(mainNode, modifiers, typeArgumentList));
		return true;
	}

	// looks for "typedef [type]{}" Ignores whitespace.
	// Modifies itr argument if match is found.
	bool Typedef(Iterator & itr)
	{
		Iterator newItr = itr; 
		ConsumeWhitespace(newItr);
		if (!newItr.ConsumeWord("typedef"))
		{
			return false;
		}		

		ConsumeWhitespace(newItr);
		
		TypePtr type;
		if (!Type(newItr, type))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Typedef.NoTypeDefinitionFound"));
		}

		std::string name;
		if (!ConsumeComplexName(newItr, name))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Typedef.NoName"));
		}		

		NodePtr typedefNode = currentScope->FindOrCreate(name);
		
		Typedef::Create(typedefNode, 
			Reason::Create(CppAxioms::TypedefCreation(), newItr.GetSource()),
			type);

		ConsumeWhitespace(newItr);
		
		NodePtr oldScope = currentScope;
		currentScope = typedefNode;
		while(Attribute(newItr)) {
			ConsumeWhitespace(newItr);
		}
		currentScope = oldScope;		

		if (!newItr.ConsumeChar(';'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Typedef.NoSemicolon"));			
		}		
		
		itr = newItr; // Success! :)		
		return true;
	}

	/** Attempts to parse a variable. Returns false if it can't.  Will move
	 * itr.  If it finds variable type info followed by a complex name it
	 * will return the typeInfo and name.  Otherwise an exception gets thrown.
	 * Does not parse the semicolon though, just stops after the name. 
	 * [ WARNING: This name is misleading.  It doesn't fully parse the var,
	 *   just the first part. ]
	 */
	bool Variable(Iterator & itr, Access & access, bool & _friend, 
				  bool & global, bool & isInline, bool & isStatic, 
				  TypePtr & type, std::string & varName)
	{
		using namespace Macaroni::Model::Cpp;

		access = Access::NotSpecified();
		_friend = false;
		global = false;
		isInline = false;		
		isStatic = false;
		while(
				(access == Access::NotSpecified()
					&& (access = AccessKeyword(itr)) != Access::NotSpecified())
				|| (!_friend && (_friend = FriendModifierKeyword(itr)))
				|| (!global && (global = GlobalKeyword(itr)))
				|| (!isInline && (isInline = InlineKeyword(itr)))
				|| (!isStatic && (isStatic = StaticKeyword(itr)))
			  )
		{
			//       ~Monkey
			//  ^..^
			// ~."".
		}
		

		if (access == Access::Public() && global && isStatic) 
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.PublicGlobalStaticMakesNoSense")); 			
		}

		if (!Type(itr, type))
		{
			return false;
		}
		//if (!VariableType(itr, typeInfo))
		//{
		//	return false;
		//}

		if (itr.ConsumeWord(""))
		// Now we need to see a name.
		if (!ConsumeComplexName(itr, varName))
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.NameExpected")); 
		}		

		itr.ConsumeWhitespace();
		
		return true;
	}

	/** Will look at and consume variables with type info, a name, and ending
	 * with a semicolon.  Returns false if nothing is found. 
	 * If it finds something it creates the variable within currentScope, and
	 * may define additional nodes if the variable's name is complex. */
	bool VariableOrFunction(Iterator & itr)
	{
		using namespace Macaroni::Model::Cpp;


		Access access = Access::NotSpecified();
		bool _friend;
		bool global;
		bool isInline;
		bool isStatic;
		TypePtr type;
		std::string varName;
		Iterator oldItr = itr; // Save it in case we need to define where the
							   // var definition began.
		if (!Variable(itr, access, _friend, global, isInline, isStatic, type, varName))
		{
			if (global)
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.MustFollowGlobalKeyword")); 
			}
			return false;
		}
		
		if (_friend && !global) 
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.MustFollowGlobalKeyword")); 
		}

		if (access == Access::NotSpecified())
		{
			access = Access::Private();
		}

		NodePtr node;

		if (global)
		{
			node = currentScope->GetNode()->FindOrCreate(varName);
			node->SetAdoptedHome(currentScope);			
		} 
		else
		{
			node = currentScope->FindOrCreate(varName);
		}		

		ConsumeWhitespace(itr);
		
		std::string initializer;

		if (itr.ConsumeChar('='))
		{
			// Initializer expression following var
			ConsumeExpression(itr, ";", initializer);
		}

		if (itr.ConsumeChar(';'))
		{			
			Variable::Create(node, access, isStatic, type, initializer,
				Reason::Create(CppAxioms::VariableScopeCreation(), oldItr.GetSource()));
			if (global)
			{
				ClassPtr classPtr;
				if (!!currentScope->GetMember())
				{
					classPtr = boost::dynamic_pointer_cast<Macaroni::Model::Cpp::Class>(currentScope->GetMember());
				}
				if (!classPtr)
				{
					throw ParserException(itr.GetSource(),
						Messages::Get("CppParser.Global.AllowedForClassesOnly"));
				}
				classPtr->AddGlobal(node);
				if (_friend)
				{
					classPtr->AddFriend(node);
				}
			}	
		} 
		else if (itr.ConsumeChar('('))
		{
			// Was going to throw exception if initializer found, but really 
			// it will parse to ; anyway...
			///*if (!initializer.empty())
			//{
			//	throw ParserException(itr.GetSource(),
			//			Messages::Get("CppParser.Variable.InitializerNotAllowedemicolonExpected")); 
			//}*/

			NodePtr oldScope = currentScope;
			NodePtr foNode = node->CreateNextInSequence("Overload#");
			currentScope = foNode;
				FunctionArgumentList(itr);			

			bool constMember = false;
			ConsumeWhitespace(itr);
			if (itr.ConsumeWord("const"))
			{
				constMember = true;
				ConsumeWhitespace(itr);
			}
			
			while(Attribute(itr));

			currentScope = oldScope;

			std::string codeBlock;
			bool codeAttached = false;
			Iterator startOfCodeBlock = itr;
			codeAttached = CodeBlock(itr, codeBlock);
			if (!codeAttached)
			{
				ConsumeWhitespace(itr);
				if (!itr.ConsumeChar(';'))
				{
					throw ParserException(itr.GetSource(),
						Messages::Get("CppParser.Function.SemicolonExpected")); 
				}
			}
			ReasonPtr fReason = Reason::Create(CppAxioms::FunctionCreation(), 
											   oldItr.GetSource());
			FunctionPtr function = Function::Create(node, fReason);
			FunctionOverloadPtr fOl = 
				FunctionOverload::Create(foNode, isInline, access, isStatic, 
										 type, constMember, fReason);
			//isInline, access, isStatic, type, constMember,
			if (codeAttached)
			{
				fOl->SetCodeBlock(codeBlock, startOfCodeBlock.GetSource());
			}
			if (global)
			{
				ClassPtr classPtr;
				if (!!currentScope->GetMember())
				{
					classPtr = boost::dynamic_pointer_cast<Macaroni::Model::Cpp::Class>(currentScope->GetMember());
				}
				if (!classPtr)
				{
					throw ParserException(itr.GetSource(),
						Messages::Get("CppParser.Global.AllowedForClassesOnly"));
				}
				classPtr->AddGlobal(foNode);
				if (_friend)
				{
					classPtr->AddFriend(foNode);
				}
			}	

		}
		else
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Variable.SemicolonExpected")); 
		}
		
		return true;
	}

	/////** Attempts to parse a variable's type information.
	//// * expects: [const] [typeComplexName] [const] [*] [const] [&]
 ////    */
	////bool VariableType(Iterator & itr, TypeInfo & info)
	////{
	////	info = TypeInfo();

	////	ConsumeWhitespace(itr);		
	////	if (itr.ConsumeWord("const"))
	////	{
	////		info.IsConst = true;
	////		ConsumeWhitespace(itr);
	////	}

	////	if (!ConsumeNodeName(itr, info.Node))
	////	{
	////		if (info.IsConst)
	////		{
	////			// If we saw const, they're committed.
	////			throw ParserException(itr.GetSource(),
	////			Messages::Get("CppParser.Variable.ConstMaybeBeforeVar")); 
	////		}
	////		else
	////		{
	////			// Didn't see false and that didn't look like a typename, so
	////			// we're fine.
	////			return false;
	////		}
	////	}
	////	
	////	// At this point, we're committed.

	////	if (!info.Node)
	////	{
	////		throw ParserException(itr.GetSource(),
	////			Messages::Get("CppParser.Variable.UnknownTypeName")); 
	////	}

	////	ConsumeWhitespace(itr);
	////	if (itr.ConsumeWord("const"))
	////	{
	////		if (info.IsConst)
	////		{
	////			// Appeared twice!? How dare it!
	////			throw ParserException(itr.GetSource(0, -5),
	////				Messages::Get("CppParser.Variable.ConstSeenTwice")); 
	////		}
	////		info.IsConst = true;
	////		ConsumeWhitespace(itr);
	////	}

	////	// Now, we check for reference.
	////	if (itr.ConsumeChar('*'))
	////	{
	////		info.IsPointer = true;
	////		ConsumeWhitespace(itr);
	////		if (itr.ConsumeWord("const"))
	////		{
	////			info.IsConstPointer = true;				
	////			ConsumeWhitespace(itr);
	////		}
	////	}

	////	// Now, we check for reference.
	////	if (itr.ConsumeChar('&'))
	////	{
	////		info.IsReference = true;
	////		ConsumeWhitespace(itr);
	////	}

	////	// At this point, we've seen all we can of the type info.
	////	return true;

	////}


	static void RunTests()
	{
		ComplexNameTests();
		FindNodeTest();
		FindNodeFromImportsTest();
		SimpleNameTests();		
	}
};

PippyParser::PippyParser()
{
}

PippyParser::~PippyParser()
{
}

PippyParserPtr PippyParser::Create()
{
	return PippyParserPtr(new PippyParser());
}

int PippyParser::Read(Model::LibraryPtr l, Model::SourcePtr source, const std::string & text)
{
	CppContext::CreateCppNodes(l->GetContext());
	ParserFunctions funcs(l->GetContext(), l);		
	Iterator itr(text.begin(), 
			   text.end(), 
			   source);
	try
	{
		funcs.Document(itr);
	}
	catch(ModelInconsistencyException & mie)
	{
		throw ParserException(mie.GetSource(), mie.GetMessage());
	}

	/*typedef std::string::const_iterator iteratorType;
    typedef complexDecls<iteratorType> complexDecls;
	complexDecls grammar;

	std::string::const_iterator iter = text.begin();
    std::string::const_iterator end = text.end();
    bool r = phrase_parse(iter, end, grammar, space);

	if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "-------------------------\n";
    }
    else
    {
        std::string rest(iter, end);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \": " << rest << "\"\n";
        std::cout << "-------------------------\n";
    }*/
	/*
	DocumentGrammar cppGrammar;

	const char * c_str = text.c_str();
	typedef position_iterator<char const*> iterator_t;
	iterator_t begin(c_str, 
					 c_str + text.length(), 
					 source->GetFileName()->GetName().c_str());
	iterator_t end;

	parse_info<iterator_t> info = parse(
		//text.c_str(),
		begin, end,
		cppGrammar,
		space_p);
	*/
	return 1;
}

void PippyParser::RunTests()
{
	Iterator::RunTests();
	ParserFunctions::RunTests();
}

END_NAMESPACE

#endif