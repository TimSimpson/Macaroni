#ifndef MACARONI_PLATFORM_WINDOWS_FILETIME_H
#define MACARONI_PLATFORM_WINDOWS_FILETIME_H

#include "../../ME.h"
#include <boost/filesystem/operations.hpp>

BEGIN_NAMESPACE(Macaroni, Platform, Windows)

class FileTime
{
public:
	static bool File1IsOlderThanFile2(boost::filesystem::path & one,
									  boost::filesystem::path & two);
};

END_NAMESPACE

#endif
