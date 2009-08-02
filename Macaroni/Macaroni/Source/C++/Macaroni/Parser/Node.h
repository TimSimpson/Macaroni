#ifndef MACARONI_PARSER_NODE_H
#define MACARONI_PARSER_NODE_H

#include "../ME.h"
#include "NodeChildData.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

class Tree;

class Node
{
friend class Tree;
public:	
	Node * FindOrCreate(const std::string & complexName);

	Node * GetNode(const std::string & name) const;

	Node * GetNode(int index) const;

protected:
	Node();
	
	~Node();

	void AddData(NodeChildData data);

private:
	typedef std::vector<NodeChildData> NodeChildList;
	std::vector<NodeChildData> items;
	std::string name;
	Node * parent;
	Tree * tree;
};

END_NAMESPACE2

#endif

