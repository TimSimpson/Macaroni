#ifndef MACARONI_EXCEPTION_H
#define MACARONI_EXCEPTION_H

#include <exception>
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
	}

	Exception(const char * source, const char * message)
		: source(source), message(message)
	{
	}

	Exception(std::string const & source, const char * message)
		: source(source), message(message)
	{
	}

	Exception(const char * source, std::string const & message)
		: source(source), message(message)
	{
	}

	Exception(std::string const & source, std::string const & message)
		: source(source), message(message)
	{
	}
	
	Exception(Exception const & other) // Copy ctor
		: source(other.source), message(other.message)
	{
	}

	Exception & operator =(const Exception & other) // Copy assignment.
	{
		this->source = other.source;
		this->message = other.message;
	}
	
	~Exception()
	{
		// No need to do anything! Thank you, std::string.
	}

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