/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/io_chrono.hpp>


namespace cyng 
{	
	std::ostream& operator<<(std::ostream& os, std::chrono::system_clock::time_point const& tp)
	{
		const auto r = chrono::convert(tp);
		return os << r;
	}
	
	std::ostream& operator<<(std::ostream& os, chrono::dbl_time_point const& r)
	{
		const std::tm tm = chrono::convert_utc(r.first);
		const double sec = chrono::second(tm) + r.second;
		
		//	store and reset stream state
		boost::io::ios_flags_saver  ifs(os);

		os 
		<< std::dec
		<< std::setfill('0');
		
		//	write to stream
		os
		<< chrono::year(tm)
		<< '-'
		<< std::setw(2)
		<< chrono::month(tm)
		<< '-'
		<< std::setw(2)
		<< chrono::day(tm)
		<< ' '
		<< std::setw(2)
		<< chrono::hour(tm)
		<< ':'
		<< std::setw(2)
		<< chrono::minute(tm)
		<< ':'
		//	So, the value for setw is: 8 for the precision desired + 2 for the integer desired + 1 for the floating point.
		<< std::setfill('0') 
		<< std::setw(11)
		<< std::fixed
		<< std::setprecision(8)
		<< sec
		;
		
		return os;
	}
	
}

