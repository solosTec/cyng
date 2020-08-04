/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/io_chrono.hpp>
#include <sstream>
#include <boost/core/ignore_unused.hpp>

namespace cyng 
{	
	std::ostream& operator<<(std::ostream& os, std::chrono::system_clock::time_point const& tp)
	{
		const auto r = chrono::to_dbl_time_point(tp);
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

	std::string to_str(std::chrono::system_clock::time_point const& tp)
	{
		const std::tm tm = chrono::convert_utc(std::chrono::system_clock::to_time_t(tp));

		//	store and reset stream state
		std::stringstream ss;

		ss
			<< std::setfill('0')
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
			<< std::setw(2)
			<< chrono::second(tm)
			;

		return ss.str();
	}

	std::string to_str_iso(std::chrono::system_clock::time_point const& tp)
	{
		const std::tm tm = chrono::convert_utc(std::chrono::system_clock::to_time_t(tp));

		//	store and reset stream state
		std::stringstream ss;

		//	yyyy-mm-ddThh:mm:ssZ 
		ss
			<< std::setfill('0')
			<< chrono::year(tm)
			<< '-'
			<< std::setw(2)
			<< chrono::month(tm)
			<< '-'
			<< std::setw(2)
			<< chrono::day(tm)
			<< 'T'
			<< std::setw(2)
			<< chrono::hour(tm)
			<< ':'
			<< std::setw(2)
			<< chrono::minute(tm)
			<< ':'
			<< std::setw(2)
			<< chrono::second(tm)
			<< 'Z'
			;

		return ss.str();
	}

	std::ostream& date_to_str(std::ostream& os, std::chrono::system_clock::time_point tp, char sep)
	{
		const std::tm tm = chrono::convert_utc(std::chrono::system_clock::to_time_t(tp));

		//	store and reset stream state
		boost::io::ios_flags_saver  ifs(os);

		os
			<< std::setfill('0')
			<< chrono::year(tm)
			<< sep
			<< std::setw(2)
			<< chrono::month(tm)
			<< sep
			<< std::setw(2)
			<< chrono::day(tm)
			;

		return os;
	}

	std::string date_to_str(std::chrono::system_clock::time_point tp)
	{
		const std::tm tm = chrono::convert_utc(std::chrono::system_clock::to_time_t(tp));
        boost::ignore_unused(tm);

		//	store and reset stream state
		std::stringstream ss;
		date_to_str(ss, tp, '-');
		return ss.str();

	}

}

