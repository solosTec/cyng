/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/factory/chrono_factory.h>
#include <cyng/chrono.h>

namespace cyng 
{	

	object make_now()
	{
		return make_object(std::chrono::system_clock::now());
	}
	
	object make_time_point_min()
	{
		return make_object(std::chrono::system_clock::time_point::min());
	}
	
	object make_time_point_max()
	{
		return make_object(std::chrono::system_clock::time_point::max());
	}
	
	object make_nanoseconds(std::uint64_t n)
	{
		return make_object(std::chrono::nanoseconds(n));
	}
	
	object make_microseconds(std::uint64_t n)
	{
		return make_object(std::chrono::microseconds(n));
	}
	
	object make_milliseconds(std::uint64_t n)
	{
		return make_object(std::chrono::milliseconds(n));
	}
	
	object make_seconds(std::uint64_t n)
	{
		return make_object(std::chrono::seconds(n));
	}
	
	object make_minutes(std::uint64_t n)
	{
		return make_object(std::chrono::minutes(n));
	}
	
	object make_hours(std::uint64_t n)
	{
		return make_object(std::chrono::hours(n));
	}
	
	object make_days(std::uint64_t n)
	{
		return make_object(chrono::days(n));
	}
	
}

