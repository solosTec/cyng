/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_CHRONO_FACTORY_H
#define CYNG_CHRONO_FACTORY_H


#include <cyng/factory/factory.hpp>

namespace cyng 
{	
	object make_now();
	object make_time_point_min();
	object make_time_point_max();
	object make_time_point(std::time_t);
	object make_nanoseconds(std::uint64_t);
	object make_microseconds(std::uint64_t);
	object make_milliseconds(std::uint64_t);
	object make_seconds(std::uint64_t);
	object make_minutes(std::uint64_t);
	object make_hours(std::uint64_t);
	object make_days(std::uint64_t);
#if CYNG_ODBC_INSTALLED
	object make_odbc_ts(std::chrono::system_clock::time_point);
#endif

}

#endif
