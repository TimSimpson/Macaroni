#ifndef MACARONI_MODEL_NODE_H
#define MACARONI_MODEL_NODE_H


#include "../ME.h"
#include <vector>
#include "Context.lh"
#include "ScopeMember.h"

BEGIN_NAMESPACE2(Macaroni, Model)

class Node
{
public:	
	Node * Find(const std::string & name);

	Node * FindOrCreate(const std::string & name);

	inline Node * GetChild(int index) const
	{
		return children[index];
	}

	inline size_t GetChildCount() const
	{
		return children.size();
	}

	static bool IsComplexName(const std::string & name);

	static bool IsSimpleName(const std::string & name);

	static void ParseComplexName(ScopePtr searchRoot, const std::string & complexName,
						         ScopePtr & resultScope,
						         std::string & resultSimpleName);

	static void SplitFirstNameOffComplexName(const std::string & complexName,
											 std::string & firstPart,
											 std::string & lastPart);
																				 
	static void SplitScopeAndMemberName(const std::string & complexName,
										std::string & scopeName, 
										std::string & memberName);

	static void SplitComplexName(const std::string & complexName,
		   						 std::vector<std::string> & subNames);

protected:
	Node();
	
	~Node();

	Node * find(const std::string & name) const;

	void AddData(NodeChildData data);

private:
	Context * context;

	std::vector<Node *> children;
	
	std::string name;
	
};

END_NAMESPACE2

#endif

