// Must be the first thing on every C file.
#define TURTLE_SOURCE

#include <boost/filesystem/convenience.hpp>
#include "Turtle.h"


using boost::filesystem::path;
using Macaroni::Reptile::Turtle;

namespace Macaroni 
{
	namespace Reptile
	{
		Turtle::Turtle(const path & p)
			: myPath(p)
		{
				
		}
		
		int Turtle::getNumber()
		{
			return 50;	
		}
	}	
}