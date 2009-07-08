#ifndef MACARONI_PARSER_CPP_CPPPARSER_CPP
#define MACARONI_PARSER_CPP_CPPPARSER_CPP

#include "CppParser.h"
#include "../../Model/Context.h"
#include "CppParserState.h"
#include "../../Model/Source.h"
#include "../../Model/FileName.h"
#include "CppParser.spirit"
#include <sstream>
#include <boost/algorithm/string.hpp>

#include <boost/spirit/include/classic_position_iterator.hpp>


using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

struct custom_space_parser : public char_parser<custom_space_parser>
{
    typedef space_parser self_t;

	custom_space_parser(CppParserState & s):state(s) {}
	
	CppParserState & state;

    template <typename CharT>
    bool test(CharT ch) const
    {
		std::cerr << ch;
		if (ch == '\n')
		{
			state.IncrementLineNumber();
		}		
        return impl::isspace_(ch);
    }
};

space_parser const space_p = space_parser();

CppParser::CppParser()
{
	//p = nullptr;
}

CppParser::~CppParser()
{
}

CppParserPtr CppParser::Create()
{
	return CppParserPtr(new CppParser());
}

int CppParser::Read(ContextPtr c, SourcePtr source, const std::string & text)
{
	CppParserState state;
	state.SetContext(c);
	state.SetCurrentSource(source);
	
	DocumentGrammar cppGrammar;
	ParserActions actors(state);
	cppGrammar.actors = &actors;
	
	/*vector<double> v;
	DocumentGrammar cppGrammar;
	cppGrammar.v = &v;*/

	custom_space_parser skipSpaces(state);
	//ipSpaces.state = state;

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
		space_p);//skipSpaces);
	
	if (!false)//info.full)
    {
		//file_position const & pos = info.stop.get_position();
		SourcePtr errSrc = state.GetCurrentSource()->JumpToLine(1);//pos.line);
		
		//std::string remainder(info.stop);
		//boost::trim(remainder);
		//if (remainder.size() > 0)
		//{
			std::stringstream ss;
			ss << "Syntax or parser error.";
			//ss << info.stop;
			throw ParserException(errSrc, ss.str());
		//}
    } 

	//if (!info.hit)
	//{
	//	// Couldn't parse anything.
	//	throw ParserException(source, std::string(
	//		"Syntax or parser error."
	//		));
	//}
	///*else
	//{
	//	if (!info.full)
	//	{			
	//		throw ParserException(state.GetCurrentSource(),
	//			std::string("An unknown error occured while parsing source."));
	//	}
	//}*/
	return 1;
} 

END_NAMESPACE

#endif