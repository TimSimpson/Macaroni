#ifndef MACARONI_MODEL_ATTRIBUTEVALUETYPEEXCEPTION_H
#define MACARONI_MODEL_ATTRIBUTEVALUETYPEEXCEPTION_H

#include <Macaroni/ME.h>
#include <Macaroni/Exception.h>

BEGIN_NAMESPACE2(Macaroni, Model)

class AnnotationValueTypeException : public Macaroni::Exception 
{
public:
	AnnotationValueTypeException(std::string message)
		: Macaroni::Exception(message.c_str()) {}
};

END_NAMESPACE2

#endif
