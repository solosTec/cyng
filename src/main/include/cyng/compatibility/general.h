/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_COMPATIBILITY_H
#define CYNG_COMPATIBILITY_H

#include <CYNG_project_info.h>

#if defined(CYNG_STD_STRING_VIEW_OFF)
#include <boost/utility/string_view.hpp>
#else
#include <string_view>
#endif

namespace cyng 
{
#if defined(CYNG_STD_STRING_VIEW_OFF)
    using string_view = boost::string_view;
    
#define CYNG_STRING_LITERAL(x)    \
    std::string(x) 
    
    
#else
    using string_view = std::string_view;
    
#define CYNG_STRING_LITERAL(x)    \
    x##s 
    
#endif
    
}

#endif 	//	CYNG_COMPATIBILITY_H







