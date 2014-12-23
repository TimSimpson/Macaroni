#ifndef MACARONI_COMPILE_GUARD_Macaroni_Environment_MacaroniEnvironment_H
#define MACARONI_COMPILE_GUARD_Macaroni_Environment_MacaroniEnvironment_H

// This exists soley to work around a Macaroni issue with classes that have
// the same name as their namespace.

namespace Macaroni { namespace Environment {

class Environment;

typedef Environment MacaroniEnvironment;

} }

#endif
