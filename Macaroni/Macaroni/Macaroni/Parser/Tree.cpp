#ifndef MACARONI_PARSER_TREE_CPP
#define MACARONI_PARSER_TREE_CPP

#include "Tree.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

Tree::Tree()
:refCount(0), root(new Node(this))
{
}

Tree::~Tree()
{
	delete root;
}

Node * Tree::GetRoot()
{
	return root;
}

END_NAMESPACE2

#endif