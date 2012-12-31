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
/* This file defines multiple macros used by Macaroni.
 * It piggybacks off Boost Config for much of the heavy lifting.
 */
#ifndef ME_H
#define ME_H

#include <boost/config.hpp>

#ifdef BOOST_NO_NULLPTR
	#define nullptr 0
#endif

#ifdef BOOST_WINDOWS
	#ifndef MACARONI_COMPILE_TARGET_WINDOWS
		#define MACARONI_COMPILE_TARGET_WINDOWS
	#endif
#else

	#ifndef MACARONI_COMPILE_TARGET_LINUX
		#define MACARONI_COMPILE_TARGET_LINUX
	#endif
#endif

// In MSVC and C++0x you can have a vector with const elements.

// It was a shock to me this isn't really allowed.  I'm not sure if VC++
// actually gets away with this or if it causes hard drives to get formatted but
// for now I want to keep it in.
#ifdef MACARONI_COMPILE_TARGET_WINDOWS
//TODO: This may be overkill. I think boost::filesystem defines this somewhere.
#define MACARONI_DIRECTORY_SEPERATOR "\\"
#endif
#ifdef MACARONI_COMPILE_TARGET_LINUX
#define MACARONI_DIRECTORY_SEPERATOR "/"
#endif

#ifdef BOOST_MSVC // MACARONI_VE_CONST_IS_ALLOWED
#define MACARONI_VE_CONST const
#define MACARONI_VE_CONST_IS_ALLOWED
#else
#define MACARONI_VE_CONST /**/
#endif

#define BEGIN_NAMESPACE(a, b, c) namespace a{ namespace b{ namespace c{
#define END_NAMESPACE } } }

#define BEGIN_NAMESPACE2(a, b) namespace a{ namespace b{
#define END_NAMESPACE2 } }

#define BEGIN_NAMESPACE4(a, b, c, d) namespace a{ namespace b{ namespace c{ namespace d{
#define END_NAMESPACE4 } } } }

namespace Macaroni
{
void ThrowMacaroniException(const char * file, int line, const char * message);
}

/**
 * Crashes hard core.  An alternative to the classic exit command, except this
 * can be caught at the absolute lowest level to give some kind of error screen.
 */
//TODO: Rename or get rid of this. It makes it seem like its intended to end the
//app, but thats not what it'll do.
#define MACARONI_FAIL(m) { Macaroni::ThrowMacaroniException(__FILE__, __LINE__, (m)); }

#define MACARONI_THROW(m) { Macaroni::ThrowMacaroniException(__FILE__, __LINE__, (m)); }

#define MACARONI_CHECK(a, b) {if (!(a)){ Macaroni::ThrowMacaroniException(__FILE__, __LINE__, (b)); } }

#ifndef DEBUGGING_OFF
#define MACARONI_ASSERT(a, b) {if (!(a)){ Macaroni::ThrowMacaroniException(__FILE__, __LINE__, (b)); } }
#else
#define MACARONI_ASSERT(a, b) { /* a, b */ }
#endif

#endif
