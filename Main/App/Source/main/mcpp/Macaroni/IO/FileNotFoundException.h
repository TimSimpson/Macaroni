/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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

