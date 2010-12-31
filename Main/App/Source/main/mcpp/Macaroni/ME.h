#ifndef ME_H
#define ME_H

// In C++0x (C++ 2009) nullptr will become officially adopted by C++.
#ifndef nullptr
#define nullptr 0
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
