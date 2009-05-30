#ifndef MACARONI_PARSER_RULE_H
#define MACARONI_PARSER_RULE_H

#include "../ME.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

class Rule
{
public:
	Rule(const std::string & exp)
		: explanation(exp)
	{

	}
	
	~Rule();
	
	const std::string & GetExplanation() { return explanation; }

private:
	std::string explanation;
};

END_NAMESPACE2

#endif

