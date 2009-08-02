#ifndef MACARONI_MODEL_INSTANCE_H
#define MACARONI_MODEL_INSTANCE_H

class Instance
{
public:
	enum Type
	{
		ConstPointer,
		ConstReference,
		Direct,
		Pointer,
		Reference,
	};
	Instance(NamespacePtr startingNs, std::string name);
private:
	// True if the reference is const.
	ClassPtr classPtr;
	bool constant;
	std::string name;
	Type type;

}

#endif

