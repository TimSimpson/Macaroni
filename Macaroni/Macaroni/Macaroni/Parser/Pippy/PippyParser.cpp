#ifndef MACARONI_PARSER_PIPPY_PIPPYPARSER_CPP
#define MACARONI_PARSER_PIPPY_PIPPYPARSER_CPP

#include "../../ME.h"
#include "../../Model/Cpp/Class.h"
#include "../../Model/Context.h"
#include "../Cpp/CppAxioms.h"
#include "../../Model/Cpp/CppContext.h"
#include "../../Exception.h"
#include "../../Model/FileName.h"
#include "../../Model/Cpp/Function.h"
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
#include "../../Model/Cpp/Variable.h"
#include "../../Model/Cpp/TypeInfo.h"

//#include "PippyParser.spirit"
#include <sstream>
#include <string>

using Macaroni::Model::Cpp::Class;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using namespace Macaroni::Parser::Cpp;
using Macaroni::Model::Cpp::CppContext;
using Macaroni::Model::Cpp::CppContextPtr;
using Macaroni::Model::FileName;
using Macaroni::Model::FileNamePtr;
using Macaroni::Model::Cpp::Function;
using Macaroni::Model::Cpp::FunctionPtr;
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
using Macaroni::Model::Cpp::Variable;
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

	inline void ConsumeWhiteSpace()
	{
		while(!Finished() && *itr <= 32)
		{
			Advance(1);
		}
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
		catch(Macaroni::Exception *)
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
public:

	ParserFunctions(ContextPtr context)
		:context(context), currentScope(context->GetRoot()), importedNodes(new NodeList())
	{			
		MACARONI_ASSERT(!!CppContext::GetPrimitives(context),
			"Cpp nodes must be found to parse successfully.");
		NodePtr primitiveRoot = CppContext::GetPrimitives(context);
		for(unsigned int i = 0; i < primitiveRoot->GetChildCount(); i ++)
		{
			importedNodes->push_back(primitiveRoot->GetChild(i));
		}
	}

	void AddImport(NodePtr node)
	{
		importedNodes->push_back(node);
	}

	// looks for "class [complexName]{}" Ignores whitespace.
	// Modifies itr argument if match is found.
	bool Class(Iterator & itr)
	{
		Iterator newItr = itr; 
		newItr.ConsumeWhiteSpace();
		if (!newItr.ConsumeWord("class"))
		{
			return false;
		}
		
		// Once the parser has seen "class" it must see the correct
		// grammar or there will be hell to pay.

		newItr.ConsumeWhiteSpace();
		
		std::string name;
		if (!ConsumeComplexName(newItr, name))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Namespace.NoID1"));
		}		

		NodePtr oldScope = currentScope;
		currentScope = currentScope->FindOrCreate(name, hFilesForNewNodes);
		Class::Create(currentScope, importedNodes,  
			Reason::Create(CppAxioms::ClassCreation(), newItr.GetSource()));

		newItr.ConsumeWhiteSpace();

		SourcePtr firstBraceSrc = itr.GetSource();

		if (!newItr.ConsumeChar('{'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Class.NoOpeningBrace"));
			//EXCEPTION MSG: Expectedd { after namespace identifier.")
		}		

		ScopeFiller(newItr);

		newItr.ConsumeWhiteSpace();
		
		//Model::Cpp::Namespace::Create(currentScope,
		//	
	///		);
		
		newItr.ConsumeWhiteSpace();
		if (!newItr.ConsumeChar('}'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Class.NoEndingBrace", firstBraceSrc->GetLine()));			
		}

		// The ';' following a class def is optional.
		newItr.ConsumeWhiteSpace();
		newItr.ConsumeChar(';');
		
		itr = newItr; // Success! :)
		currentScope = oldScope;
		return true;
	}

	/** Consumes white space, then returns false if '{' not seen.
	 * If it is, consumes all text until next '}'. */
	bool CodeBlock(Iterator & itr, std::string & codeBlock)
	{
		itr.ConsumeWhiteSpace();
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
		itr.ConsumeWhiteSpace();
		bool tilda = false;
		if (itr.ConsumeChar('~'))
		{
			tilda = true;
			itr.ConsumeWhiteSpace();
		}
		if (!itr.ConsumeWord(currentScope->GetName().c_str()))
		{
			if (!tilda)
			{
				return false;
			}
			throw new ParserException(itr.GetSource(),
				Messages::Get("CppParser.Constructor.ClassNameExpected")); 
		}
		itr.ConsumeWhiteSpace();
		if (!itr.ConsumeChar('('))
		{
			throw new ParserException(itr.GetSource(),
				Messages::Get("CppParser::Constructor::ArgumentListExpected"));
		}
		 CONSTRUCTOR MUST BE A SPECIAL SUBCLASS OF FUNCTION
			 YOU CREATE IT AND SET IT TO THE CURRENT SCOPE THEN CALL THIS:
		FunctionArgumentList(itr, why?, name?);
			TO GET THE ARGUMENT LIST.
				THEN YOU LOOK FOR VARIABLE INITIALIZERS AND ADD THEM TO 
				A LIST THE CONSTRUCTOR KEEPS.
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
			ss << itr.Current();
			itr.Advance(1);
		}
		result = ss.str();
		return true;
	}

	bool ConsumeFilePath(Iterator & itr, std::string & filePath)
	{
		std::stringstream ss;
		bool ankles = false;
		if (itr.Current() == '<')
		{
			ankles = true;
		}
		else if (!itr.Current() == '"')
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
			throw new ParserException(itr.GetSource(),
				Messages::Get("CppParser::FileNameExpected"));
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
		node = FindNode(name);
		if (!!node)
		{ 
			// Advance only if Node was found.
			itr = newItr;
		}
		return true;
	}

	bool Directives(Iterator & itr)
	{
		itr.ConsumeWhiteSpace();
		if (itr.Finished() || !(itr.Current() == '#'))
		{
			return false;
		}
		itr.Advance(1);
		if (!Import(itr) && !HFileDirective(itr))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Directive.Unknown"));
		}
		return true;
	}

	/** The start of a doc.  Either it reads stuff in or throws an exception. */
	void Document(Iterator itr)
	{
		ScopeFiller(itr);
		itr.ConsumeWhiteSpace();
		if (!itr.Finished())
		{
			throw ParserException(itr.GetSource(), Messages::Get("CppParser.Document.SyntaxError"));
		}
	}

	/** Takes a complex name and finds its node. */
	NodePtr FindNode(std::string & complexName)
	{
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
		return result;
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

	static void FindNodeTest()
	{
		ContextPtr c(new Context("{ROOT}"));
		CppContext::CreateCppNodes(c);
		ParserFunctions funcs(c);

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
		ParserFunctions funcs(c);

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


	/** This function expects us to be committed to finding a function and to
	 * have seen '('. We parse until we see ')'. */
	void FunctionArgumentList(Iterator & itr)//, TypeInfo & rtnTypeInfo, 
							  //std::string & name)
	{
		itr.ConsumeWhiteSpace();
		bool seenArg = false;
		while(!itr.ConsumeChar(')'))
		{	
			Iterator oldItr = itr;
			TypeInfo typeInfo;
			std::string argName;
			if ((seenArg && !itr.ConsumeChar(','))
				||
				!Variable(itr, typeInfo, argName))
			{
				throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Function.ExpectedEndingParenthesis"));
			}
			NodePtr node = currentScope->FindOrCreate(argName);
			Variable::Create(node, typeInfo,
				Reason::Create(CppAxioms::VariableScopeCreation(), oldItr.GetSource()));
			seenArg = true;
		}
	}

	

	bool HFileDirective(Iterator & itr)
	{
		itr.ConsumeWhiteSpace();
		if (!itr.ConsumeWord("hfile"))
		{
			return false;
		}
		itr.ConsumeWhiteSpace();
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
		itr.ConsumeWhiteSpace();
		std::string filePath;
		if (!ConsumeFilePath(itr, filePath))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Directive.HFileFilePathExpected"));
		}
		hFilesForNewNodes = filePath;
		return true;
	}

	// looks for "import [complexName];" 
	// Modifies itr argument if match is found.
	bool Import(Iterator & itr)
	{
		itr.ConsumeWhiteSpace();
		if (!itr.ConsumeWord("import"))
		{
			return false;
		}

		// Now we're committed!
		itr.ConsumeWhiteSpace();
		
		std::string name;
		if (!ConsumeComplexName(itr, name))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Import.NameNotFound"));
		}
		
		itr.ConsumeWhiteSpace();
		if (!itr.ConsumeChar(';'))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Import.SemicolonExpected"));
		}

		NodePtr importNode = context->GetRoot()->FindOrCreate(name);
		AddImport(importNode);
		return true;
	}

	// looks for "namespace [complexName]{}" Ignores whitespace.
	// Modifies itr argument if match is found.
	bool Namespace(Iterator & itr)
	{
		Iterator newItr = itr; 
		newItr.ConsumeWhiteSpace();
		if (!newItr.ConsumeWord("namespace"))
		{
			return false;
		}
		
		// Once the parser has seen "namespace" it must see the correct
		// grammar or there will be hell to pay.

		newItr.ConsumeWhiteSpace();
		
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
				Namespace::Create(ns, 
					Reason::Create(CppAxioms::NamespaceCreation(), newItr.GetSource()));
			}
			ns = ns->GetNode();
		}

		newItr.ConsumeWhiteSpace();

		SourcePtr firstBraceSrc = itr.GetSource();

		if (!newItr.ConsumeChar('{'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Namespace.NoOpeningBrace"));
			//EXCEPTION MSG: Expectedd { after namespace identifier.")
			return false;
		}		

		ScopeFiller(newItr);

		newItr.ConsumeWhiteSpace();
		
		//Model::Cpp::Namespace::Create(currentScope,
		//	
	///		);
		
		newItr.ConsumeWhiteSpace();
		if (!newItr.ConsumeChar('}'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Namespace.NoEndingBrace", firstBraceSrc->GetLine()));			
		}
		
		itr = newItr; // Success! :)
		currentScope = oldScope;
		return true;
	}

	// Very destructive - takes the itr and attempts to consume
	// anything that could be fit in a Scope.
	void ScopeFiller(Iterator & itr)
	{	
		// Take whatever you can get, replace the iter
		while (Directives(itr) 
				|| Namespace(itr) 
				|| Class(itr)
				|| VariableOrFunction(itr))
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

	
	/** Attempts to parse a variable. Returns false if it can't.  Will move
	 * itr.  If it finds variable type info followed by a complex name it
	 * will return the typeInfo and name.  Otherwise an exception gets thrown.
	 * Does not parse the semicolon though, just stops after the name. */
	bool Variable(Iterator & itr, TypeInfo & typeInfo, std::string & varName)
	{
		if (!VariableType(itr, typeInfo))
		{
			return false;
		}

		// Now we need to see a name.
		if (!ConsumeComplexName(itr, varName))
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.NameExpected")); 
		}		
		
		return true;
	}

	/** Will look at and consume variables with type info, a name, and ending
	 * with a semicolon.  Returns false if nothing is found. 
	 * If it finds something it creates the variable within currentScope, and
	 * may define additional nodes if the variable's name is complex. */
	bool VariableOrFunction(Iterator & itr)
	{
		TypeInfo typeInfo;
		std::string varName;
		Iterator oldItr = itr; // Save it in case we need to define where the
							   // var definition began.
		if (!Variable(itr, typeInfo, varName))
		{
			return false;
		}
		
		NodePtr node = currentScope->FindOrCreate(varName);

		itr.ConsumeWhiteSpace();
		if (itr.ConsumeChar(';'))
		{			
			Variable::Create(node, typeInfo,
				Reason::Create(CppAxioms::VariableScopeCreation(), oldItr.GetSource()));
		
		} 
		else if (itr.ConsumeChar('('))
		{
			NodePtr oldScope = currentScope;
			currentScope = node;
				FunctionArgumentList(itr, typeInfo, varName);
			currentScope = oldScope;

			std::string codeBlock;
			bool codeAttached = false;
			Iterator startOfCodeBlock = itr;
			codeAttached = CodeBlock(itr, codeBlock);
			if (!codeAttached)
			{
				itr.ConsumeWhiteSpace();
				if (!itr.ConsumeChar(';'))
				{
					throw ParserException(itr.GetSource(),
						Messages::Get("CppParser.Function.SemicolonExpected")); 
				}
			}
			FunctionPtr function = Function::Create(node, typeInfo, 
				Reason::Create(CppAxioms::FunctionCreation(), oldItr.GetSource()));
			if (codeAttached)
			{
				function->SetCodeBlock(codeBlock, startOfCodeBlock.GetSource());
			}

		}
		else
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Variable.SemicolonExpected")); 
		}
		
		return true;
	}

	/** Attempts to parse a variable's type information.
	 * expects: [const] [typeComplexName] [const] [*] [const] [&]
     */
	bool VariableType(Iterator & itr, TypeInfo & info)
	{
		info = TypeInfo();

		itr.ConsumeWhiteSpace();		
		if (itr.ConsumeWord("const"))
		{
			info.IsConst = true;
			itr.ConsumeWhiteSpace();
		}

		if (!ConsumeNodeName(itr, info.Node))
		{
			if (info.IsConst)
			{
				// If we saw const, they're committed.
				throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Variable.ConstMaybeBeforeVar")); 
			}
			else
			{
				// Didn't see false and that didn't look like a typename, so
				// we're fine.
				return false;
			}
		}
		
		// At this point, we're committed.

		if (!info.Node)
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Variable.UnknownTypeName")); 
		}

		itr.ConsumeWhiteSpace();
		if (itr.ConsumeWord("const"))
		{
			if (info.IsConst)
			{
				// Appeared twice!? How dare it!
				throw ParserException(itr.GetSource(0, -5),
					Messages::Get("CppParser.Variable.ConstSeenTwice")); 
			}
			info.IsConst = true;
			itr.ConsumeWhiteSpace();
		}

		// Now, we check for reference.
		if (itr.ConsumeChar('*'))
		{
			info.IsPointer = true;
			itr.ConsumeWhiteSpace();
			if (itr.ConsumeWord("const"))
			{
				info.IsConstPointer = true;				
				itr.ConsumeWhiteSpace();
			}
		}

		// Now, we check for reference.
		if (itr.ConsumeChar('&'))
		{
			info.IsReference = true;
			itr.ConsumeWhiteSpace();
		}

		// At this point, we've seen all we can of the type info.
		return true;

	}


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

int PippyParser::Read(Model::ContextPtr c, Model::SourcePtr source, const std::string & text)
{
	CppContext::CreateCppNodes(c);
	ParserFunctions funcs(c);		
	Iterator itr(text.begin(), 
			   text.end(), 
			   source);
	try
	{
		funcs.Document(itr);
	}
	catch(ModelInconsistencyException mie)
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