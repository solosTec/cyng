/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/factory/chrono_factory.h>
#include <cyng/chrono.h>
#if CYNG_ODBC_INSTALLED
#include <sql.h> 
#endif

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
	
	object make_time_point(std::time_t tt)
	{
		return make_object(std::chrono::system_clock::from_time_t(tt));
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
	
#if CYNG_ODBC_INSTALLED
	object make_odbc_ts(std::chrono::system_clock::time_point tp)
	{

		chrono::dbl_time_point dtp = chrono::convert(tp);
		std::tm t = chrono::convert_utc(dtp.first);

		SQL_TIMESTAMP_STRUCT ts;
		ts.year = chrono::year(t);
		ts.month = chrono::month(t);
		ts.day = chrono::day(t);
		ts.hour = chrono::hour(t);
		ts.minute = chrono::minute(t);
		ts.second = chrono::second(t);
		//	billionths of a second: 0 .. 999,999,999 
		ts.fraction = static_cast<SQLUINTEGER>(dtp.second * 1000u * 1000u * 1000u);
		return make_object(ts);
	}
#endif

}

