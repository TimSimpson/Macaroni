#ifndef MACARONI_IO_FILENOTFOUNDEXCEPTION_H
#define MACARONI_IO_FILENOTFOUNDEXCEPTION_H

#include "../ME.h"
#include "../Exception.h"

BEGIN_NAMESPACE2(Macaroni, IO)


class FileNotFoundException : public Macaroni::Exception
{
public:
	template<class _Elem, class _Traits, class _Ax>
	FileNotFoundException(const std::basic_string<_Elem, _Traits, _Ax> & msg)
		: Macaroni::Exception(msg.c_str())
	{
	}
};

END_NAMESPACE2

#endif

