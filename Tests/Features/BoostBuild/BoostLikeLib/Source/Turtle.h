#ifndef MACARONI_TURTLE_H
#define MACARONI_TURTLE_H

#include "Config.h"

#include <boost/filesystem/convenience.hpp>

#include <boost/config/abi_prefix.hpp>

namespace Macaroni 
{	
	namespace Reptile
	{
		class Turtle
		{			
			/** Constructs a new instance of Turtle. */
			public: TURTLE_DECL Turtle(const boost::filesystem::path & p);
			/** Gets a number. */
			public: TURTLE_DECL int getNumber();	
			
			private: boost::filesystem::path myPath;	
		};
	}	
	
}

#include <boost/config/abi_suffix.hpp>

#endif