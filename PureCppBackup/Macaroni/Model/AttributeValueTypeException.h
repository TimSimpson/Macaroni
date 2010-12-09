#ifndef MACARONI_MODEL_ATTRIBUTEVALUETYPEEXCEPTION_H
#define MACARONI_MODEL_ATTRIBUTEVALUETYPEEXCEPTION_H

#include <Macaroni/ME.h>
#include <Macaroni/Exception.h>

BEGIN_NAMESPACE2(Macaroni, Model)

class AttributeValueTypeException : public Macaroni::Exception 
{
public:
	AttributeValueTypeException(std::string message)
		: Macaroni::Exception(message.c_str()) {}
};

END_NAMESPACE2

#endif
