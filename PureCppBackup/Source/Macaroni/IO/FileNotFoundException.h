#ifndef MACARONI_IO_FILENOTFOUNDEXCEPTION_H
#define MACARONI_IO_FILENOTFOUNDEXCEPTION_H

#include "../ME.h"
#include "../Exception.h"

BEGIN_NAMESPACE2(Macaroni, IO)


class FileNotFoundException : public Macaroni::Exception
{
public:
	FileNotFoundException(std::string & msg)
		: Macaroni::Exception(msg.c_str())
	{
	}
};

END_NAMESPACE2

#endif

