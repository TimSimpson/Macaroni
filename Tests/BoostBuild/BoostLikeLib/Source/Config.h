#ifndef TURTLE_CONFIG_HPP
#define TURTLE_CONFIG_HPP

#include <boost/config.hpp>

#ifndef BOOST_WORKS_THE_WAY_THE_DOCUMENTATION_STATES_IT_SHOULD

	#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_TURTLE_DYN_LINK)
	# if defined(TURTLE_SOURCE)
	#  define TURTLE_DECL BOOST_SYMBOL_EXPORT
	# else
	#  define TURTLE_DECL BOOST_SYMBOL_IMPORT
	# endif
	#else
	# define TURTLE_DECL 
	#endif

#else

	// Get it, because the docs are wrong.

	#ifdef BOOST_HAS_DECLSPEC
		#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_TURTLE_DYN_LINK)
			#ifdef TURTLE_SOURCE
				# define TURTLE_DECL __declspec(dllexport)
			#else
				# define TURTLE_DECL __declspec(dllimport)
			#endif
		#endif
	#endif  // BOOST_HAS_DECLSPEC
	
	#ifndef TURTLE_DECL
		#define TURTLE_DECL
	#endif

#endif


#endif

