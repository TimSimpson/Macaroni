#ifndef MACARONI_EXCEPTION_H
#define MACARONI_EXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

namespace Macaroni{

/** A very simple Exception class which stores the source of the exception
 * as well as the exception message as std::strings. */
class Exception : public std::exception
{
public:

	Exception(const char * message)
		:source("Unspecified source."), message(message)
	{
		#ifndef _DEBUG
			std::cout << "THROWN EXCEPTION:" << message << std::endl;
		#endif
	}

	//Exception(std::string & mergggg)
	//	:source("Unspecified source.", message(mergggg.c_str()))
	//{
	//	#ifndef _DEBUG
	//		std::cout << "THROWN EXCEPTION:" << message << std::endl;
	//	#endif
	//}

	Exception(const char * source, const char * message)
		: source(source), message(message)
	{
		#ifndef _DEBUG
			std::cout << "THROWN EXCEPTION at " << source << ":" << message << std::endl;
		#endif
	}

	Exception(std::string const & source, const char * message)
		: source(source), message(message)
	{
		#ifndef _DEBUG
			std::cout << "THROWN EXCEPTION at " << source << ":" << message << std::endl;
		#endif
	}

	Exception(const char * source, const std::string & message)
		: source(source), message(message)
	{
		#ifndef _DEBUG
			std::cout << "THROWN EXCEPTION at " << source << ":" << message << std::endl;
		#endif
	}

	Exception(std::string const & source, const std::string & message)
		: source(source), message(message)
	{
		#ifndef _DEBUG
			std::cout << "THROWN EXCEPTION at " << source << ":" << message << std::endl;
		#endif
	}
	
	Exception(Exception const & other) // Copy ctor
		: source(other.source), message(other.message)
	{
		#ifndef _DEBUG
			std::cout << "THROWN EXCEPTION at " << source << ":" << message << std::endl;
		#endif
	}

	Exception & operator =(const Exception & other) // Copy assignment.
	{
		this->source = other.source;
		this->message = other.message;
		return *this;
	}
	
#ifdef MACARONI_COMPILE_TARGET_WINDOWS
	virtual ~Exception()
	{
	}
#endif
#ifdef MACARONI_COMPILE_TARGET_LINUX
	virtual ~Exception() throw()
	{
	}
#endif

	const char * GetSource() const
	{
		return source.c_str();
	}

	const char * GetMessage() const
	{
		return message.c_str();
	}

	virtual const char* what() const throw()
    {
		return GetMessage();
    } 

private:
	std::string source;
	std::string message;
};

} // end of namespace


#endif