#ifndef MACARONI_PARSER_NODECHILDDATA_H
#define MACARONI_PARSER_NODECHILDDATA_H

#include "../ME.h"
#include "Evidence.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

/** Stores some kind of data for a node.
 * When all instances of this are deleted, the data pointed to is destroyed. 
 */
class NodeChildData
{
public:
	enum Type
	{
		Type_Argument,
		Type_BaseClass,
		Type_CodeBody,
		Type_IsType
		Type_Member,
		Type_ReturnValue,		
	};
	
	NodeChildData(Type type, void * data, Evidence evidence);
	
	NodeChildData(const NodeChildData & other);

	void operator=(const NodeChildData & other);

	~NodeChildData();

	Type GetType();

	template<typename T> T GetData();

	template<Node> Node GetData();

	template<std::string> 

private:	
	void * data;
	Evidence evidence;
	refCount;
	Type type;
};

END_NAMESPACE2

#endif

