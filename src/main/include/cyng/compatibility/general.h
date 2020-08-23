/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_GENERAL_COMPATIBILITY_H
#define CYNG_GENERAL_COMPATIBILITY_H

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

#if defined(__CPP_SUPPORT_P0218R1) && defined(__CPP_SUPPORT_P0156R0)
#include <system_error>
#else
#include <boost/system/system_error.hpp>
#endif

namespace cyng 
{
#if defined(__CPP_SUPPORT_P0218R1) && defined(__CPP_SUPPORT_P0156R0)
    using system_error = std::system_error;
#else
    using system_error = boost::system::system_error;
#endif

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

#endif 	//	CYNG_GENERAL_H







