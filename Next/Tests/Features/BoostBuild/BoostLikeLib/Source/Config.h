#ifndef TURTLE_CONFIG_HPP
#define TURTLE_CONFIG_HPP

/*
 * The name of the library is "Macaroni", "Macaroni_Turtle", "1.0.45"
 * 
 * So the magic Macro name prefix then becomes
 * "MACARONI_LIB_Macaroni__Turtle"
 * turning this into
 * MACARONI_LIB_DECL_Macaroni___Macaroni__Turtle
 
 * The config file has to unfortunately be something like this:
 * /Config_Macaroni___Macaroni__Turtle.h
 * 
 * The option is
 * MACARONI_LIB_DYN_LINK_Macaroni___Macaroni__Turtle
 
 */
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
	//
	#ifndef TURTLE_DECL
		#define TURTLE_DECL
	#endif

#endif


#endif

