#ifndef MACARONI_PARSER_PROTOTYPE_H
#define MACARONI_PARSER_PROTOTYPE_H

#include "../ME.h"
#include <vector>

class Prototype;

class Prototype
{
public:
	Prototype(Prototype * parent, const std::string & name);

private:
	Prototype(const Prototype & other);
	void operator=(const Prototype & other);
	~Prototype();

	std::vector<Prototype *> members;
	
	std::string name;
};

#endif

