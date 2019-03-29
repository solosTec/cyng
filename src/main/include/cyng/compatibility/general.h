/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_COMPATIBILITY_H
#define CYNG_COMPATIBILITY_H

#include <CYNG_project_info.h>

//	string_view
#if defined(__CPP_SUPPORT_N3921)	

#include <string_view>
#include <string>
#if defined(__CPP_SUPPORT_N3642)
using namespace std::string_literals; // enables s-suffix for std::string literals
#endif

#else

#include <boost/utility/string_view.hpp>
#endif

namespace cyng 
{
#if defined(__CPP_SUPPORT_N3921)
    
	using string_view = std::string_view;

#define CYNG_STRING_LITERAL(x)    \
    x ## s 

#else

	using string_view = boost::string_view;

#define CYNG_STRING_LITERAL(x)    \
    std::string(x) 
    
#endif
    
}

#endif 	//	CYNG_COMPATIBILITY_H







