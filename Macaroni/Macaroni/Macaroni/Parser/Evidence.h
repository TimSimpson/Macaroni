#ifndef MACARONI_PARSER_EVIDENCE_H
#define MACARONI_PARSER_EVIDENCE_H

#include "../ME.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

class Evidence
{
public:
	Evidence(const Rule & rule, const Source & source)
		: rule(rule), source(source)
	{
	}

	~Evidence();

	const Rule & GetRule() { return rule; }

	const Source & GetSource() { return source; }

private;
	const Rule & rule;
	Source source;
};

END_NAMESPACE2

#endif