/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/intrinsics/version.h>
#include <iostream>

namespace cyng 
{
	version::version(std::uint16_t maj, std::uint16_t min)
	: this_type(maj, min)
	{}
	
	version::version(std::uint32_t v)
	: this_type(v)
	{}
	
	version::version(version const& v)
	: this_type(v.first, v.second)
	{}
	
	version::version()
	: this_type(0, 0)
	{}

	revision::revision(std::uint32_t maj, std::uint32_t min)
	: this_type(maj, min)
	{}
	
	revision::revision(std::uint64_t v)
	: this_type(v)
	{}
	
	revision::revision(revision const& v)
	: this_type(v.first, v.second)
	{}
	
	revision::revision()
	: this_type(0, 0)
	{}
}




