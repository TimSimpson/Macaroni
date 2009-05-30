#ifndef MACARONI_PARSER_TREE_H
#define MACARONI_PARSER_TREE_H

#include "../ME.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

/** Start of the Node structure.  All managed pointers use this to keep count 
 * of references. */
class Tree
{
public:
	Tree();

	~Tree();

	Node & GetRoot();

protected:
	void addReference();

	void removeReference();

private;	
	Tree(const Tree & other);

	void operator=(const Tree & other);

	int refCount;

	Node * root;
};

END_NAMESPACE2

#endif