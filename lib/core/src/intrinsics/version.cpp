/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/intrinsics/version.h>
#include <iostream>
#include <cmath>

namespace cyng 
{
	version::version(double d)
		: this_type(static_cast<std::uint16_t>(d), static_cast<std::uint16_t>(std::round(std::fabs(d - std::trunc(d)) * 1e4)))
	{}

}




