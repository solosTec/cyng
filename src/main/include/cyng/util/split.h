/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_SPLIT_H
#define CYNG_SPLIT_H

#include <vector>
#include <cyng/compatibility/general.h>
#include <boost/utility/string_view.hpp>

namespace cyng
{
#if !defined(CYNG_STD_STRING_VIEW_OFF)
    std::vector<std::string_view> split(std::string_view str, std::string_view delims);
#endif
	std::vector<boost::string_view> split(boost::string_view str, boost::string_view delims);
	std::vector<std::string> split(std::string str, std::string delims);
	
}

#endif

