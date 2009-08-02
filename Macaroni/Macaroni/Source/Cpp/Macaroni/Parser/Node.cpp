#ifndef MACARONI_PARSER_NODE_CPP
#define MACARONI_PARSER_NODE_CPP

#include "Node.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

Node::Node()
:items(), name(), parent(nullptr), tree(nullptr)
{
}

Node::~Node()
{
	while(!items.empty())
	{
		items.removeFirst();
	}	
}
	
Node * Node::FindOrCreate(const std::string & complexName)
{

}

Node * Node::GetNode(const std::string & name) const
{
	for(int i = 0; i < items.size(); i ++)
	{
		switch(items[i]->GetType())
		{

		}
	}
}

Node * Node::GetNode(int index) const
{
}

protected:
	
	void AddData(NodeChildData data);

private:
	std::vector<NodeChildData> items;
	std::string name;
	Node * parent;
	Tree * tree;
};

END_NAMESPACE2

#endif

