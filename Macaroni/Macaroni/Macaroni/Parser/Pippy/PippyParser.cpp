#ifndef MACARONI_PARSER_PIPPY_PIPPYPARSER_CPP
#define MACARONI_PARSER_PIPPY_PIPPYPARSER_CPP

#include "../../ME.h"
#include "../../Model/Context.h"
#include "../../Exception.h"
#include "../../Model/FileName.h"
#include "../../Environment/Messages.h"
#include "../../Model/Cpp/Namespace.h"
#include "../../Model/Node.h"
#include "PippyParser.h"
#include "../Parser.h"
#include "../ParserException.h"
#include "../../Model/Source.h"

//#include "PippyParser.spirit"
#include <sstream>
#include <string>

using Macaroni::Environment::Messages;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::FileName;
using Macaroni::Model::FileNamePtr;
using Macaroni::Model::Cpp::Namespace;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;
using Macaroni::Parser::ParserException;

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
public:

	ParserFunctions(ContextPtr context)
		:context(context), currentScope(context->GetRoot())
	{
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

	/** The start of a doc.  Either it reads stuff in or throws an exception. */
	void Document(Iterator itr)
	{
		NamespaceFiller(itr);
		itr.ConsumeWhiteSpace();
		if (!itr.Finished())
		{
			throw ParserException(itr.GetSource(), Messages::Get("CppParser.Document.SyntaxError"));
		}
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

		newItr.ConsumeWhiteSpace();

		SourcePtr firstBraceSrc = itr.GetSource();

		if (!newItr.ConsumeChar('{'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Namespace.NoOpeningBrace"));
			//EXCEPTION MSG: Expectedd { after namespace identifier.")
			return false;
		}

		NamespaceFiller(newItr);

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
	// anything that could be a namespace filler.
	void NamespaceFiller(Iterator & itr)
	{	
		// Take whatever you can get, replace the iter
		while (Namespace(itr))
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

	static void RunTests()
	{
		ComplexNameTests();
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
	ParserFunctions funcs(c);		
	Iterator itr(text.begin(), 
			   text.end(), 
			   source);
	funcs.Document(itr);

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