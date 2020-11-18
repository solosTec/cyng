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

	revision::revision(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d)
	: this_type(version(a, b).full(), version(c, d).full())
	{}

	revision::revision(std::uint32_t maj, std::uint32_t min)
	: this_type(maj, min)
	{}
	
	revision::revision(version maj, version min)
		: this_type(maj.full(), min.full())
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

	//
	//	comparison
	//
	bool operator==(version const& lhs, version const& rhs)
	{
		return lhs.full() == rhs.full();
	}
	bool operator<(version const& lhs, version const& rhs)
	{
		return lhs.is_less(rhs);
	}
	bool operator!=(version const& lhs, version const& rhs)
	{
		return !(lhs == rhs);
	}
	bool operator>(version const& lhs, version const& rhs)
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(version const& lhs, version const& rhs)
	{
		return !(lhs > rhs);
	}
	bool operator>=(version const& lhs, version const& rhs)
	{
		return !(lhs < rhs);
	}

	bool operator==(revision const& lhs, revision const& rhs)
	{
		return lhs.full() == rhs.full();
	}
	bool operator<(revision const& lhs, revision const& rhs)
	{
		return lhs.is_less(rhs);
	}
	bool operator!=(revision const& lhs, revision const& rhs)
	{
		return !(lhs == rhs);
	}
	bool operator>(revision const& lhs, revision const& rhs)
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(revision const& lhs, revision const& rhs)
	{
		return !(lhs > rhs);
	}
	bool operator>=(revision const& lhs, revision const& rhs)
	{
		return !(lhs < rhs);
	}
}




