#ifndef MACARONI_PARSER_H
#define MACARONI_PARSER_H

#include <boost/spirit/core.hpp>
#include <boost/spirit/actor/push_back_actor.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/chset.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

namespace Macaroni {

namespace bs = boost::spirit;

using namespace boost::spirit;


void print_braceSandwhich(char const * str, char const * end) 
{ 
	std::string s(str, end);
	std::cout << "BRACE SANDWHICH==" << s << "\n"; 
}

void print_notBrace(char const * str, char const * end) 
{ 
	std::string s(str, end);
	std::cout << "NOT A BRACE=" << s << "\n"; 
}

void print_identifier(char const * str, char const * end) 
{ 
	std::string s(str, end);
	std::cout << "Identifier(" << s << ")\n"; 
}

void print_classBody(char const * str, char const * end) 
{ 
	std::string s(str, end);
	std::cout << "Class Body:" << s << "\n"; 
}


struct CppGramar : public grammar<CppGramar>
{
    template <typename ScannerT>
    struct definition
    {
        definition(CppGramar const& /*self*/)
        {
			/** The character {. */
			braceL
				=	ch_p('{');
			/** The character }. */
			braceR
				=	ch_p('}');
			/** A delicious sandhwhich, with braces forming the bread. {??} */
			braceSandwhich
				= braceL 
				  >> 
				  *(
						braceSandwhich 
						| 
						~ch_p('}')
					) 
				  >> 
				  braceR;			
			/*classBody
				=	braceL 
					>> 
					*(
						(braceSandwhich)
						|
						~ch_p("}")						
					 ) 
					>> 
					braceR;*/
			/** The keyword "class". */
			classKW
				=	str_p("class");		
			commentBlock
				=	commentBlockNormal;
			commentBlockNormal
				=	comment_p("//") 
					| 
					comment_p("/*", "*/");
			/** Any digit, 0 through 9. */
			digit
				=	range_p('0','9');
			/** A C++ identifier- can be a nondigit or digit. */
			cppIdentifier
				= 	(+nondigit) | *(digit | nondigit)  ;			

			/** Any letter, or the underscore. */
			nondigit
				=	range_p('a','z') | range_p('A','Z') | ch_p('_');
			/** The definition of a Macaroni program. */
			program
				=	*commentBlock
					>>
					classKW 
					>> 
					(cppIdentifier)[&print_identifier] 
					>> 
					braceSandwhich[&print_classBody];
        }

        rule<ScannerT> braceL, braceR, braceSandwhich, braceSandwhichEnd,
			//classBody, 
			classKW, //comment1L, comment1R, comment2L, comment2R,
			commentBlock, commentBlockNormal,// commentBlock1,
			digit, cppIdentifier, nondigit, program;

        rule<ScannerT> const&
        start() const { return program; }
    };
};



//boost::spirit::parser parser = boost::spirit::real_p;

void parseStream(std::istream & input);

int CmdLine(std::vector<std::string> args)
{
	std::string & fileName = args[0];
	std::ifstream input;
	input.open(fileName.c_str());
	if (!input)
	{
		std::cout << "Could not open file!";
	}
	else
	{
		parseStream(input);
	}

	return 0;
}


void parseStream(std::istream & input)
{	
	std::stringstream fullFile;
	char ch;	
	while(!input.eof())
	{
		input.get(ch);
		fullFile << ch;
	}		
	CppGramar cppG;

	bs::parse_info<> info = bs::parse(
		fullFile.str().c_str(),
		cppG,
		bs::space_p);

	std::cout << "Full  :" << info.full << "\n";	
	std::cout << "Hit   : " << info.hit << "\n";
	std::cout << "Length: " << info.length << "\n";
	std::cout << "Stop  : " << info.stop << "\n";
	
	//char c;
	//while(!input.eof())
	//{
	//	fullFile << input;//.get(c);
	//	std::cout << c;
	//}	
}

} // end namespace

#endif
