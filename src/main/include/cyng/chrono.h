/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_CHRONO_H
#define CYNG_CHRONO_H


#include <chrono>
#include <utility>
#include <string>
#include <boost/predef.h>
#if BOOST_OS_WINDOWS
#include <ctime>
#endif

namespace cyng 
{
	namespace chrono 
	{
		/**
		 * define additional duration types
		 */
		using days = std::chrono::duration<int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
		
		/**
		 * define additional duration types
		 * Second type with double resolution
		 */
		using dbl_seconds = std::chrono::duration<double>;
		
		/**
		 * timestamp with explicit seconds
		 */
		using dbl_time_point = std::pair<std::time_t, double>;
		
		/**
		 * @return gregorian year
		 */
		int year(std::tm const& t);
		
		/**
		 * @return gregorian month
		 */
		int month(std::tm const& t);
		
		/**
		 * @return day of month
		 */
		int day(std::tm const& t);

		/**
		 * @return day of year
		 */
		int day_of_year(std::tm const& t);
		
		/**
		 * @return hour of day
		 */
		int hour(std::tm const& t);

		/**
		 * @return minute of hour
		 */
		int minute(std::tm const& t);

		/**
		 * @return second of minute
		 */
		int second(std::tm const& t);

		/**
		 *	@return seconds since midnight
		 */
		int time_of_day(std::tm const& t);
        
		/**
		 * helps to initialize a tm struct
		 */
		std::tm init_tm(int year, int month, int day, int hour, int min, int sec);
        
		/**
		 * Create a time stamp with fractional seconds
		 */
		std::chrono::system_clock::time_point init_tp(std::uint16_t year, std::uint8_t month, std::uint8_t day, std::uint8_t hour, std::uint8_t min, double sec);
        
		/**
		 * Convert a tm struct into a std::time format.
		 */
		std::time_t tm_to_tt(std::tm const& t);
     
		std::tm convert_utc(std::time_t);
		std::tm convert_local(std::time_t);
		
		/**
		 * DST is not considered in this calculation.
		 * 
		 * @return difference between UTC and localtime in minutes.
		 */
		std::chrono::minutes delta();
		
		/**
		 * Converts a time point into std::time_t and the fractional seconds as double
		 * The fraction seconds value is always lower than 1.0. To get the full second
		 * value you have to add the integer part. Example:
		 * @code
		 const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		 const dbl_time_point ts = chrono::convert(now);
		 std::tm r = chrono::convert(ts.first);
		 const double seconds = ts.second + chrono::second(r);
		 * @endcode
		 */
		dbl_time_point to_dbl_time_point(std::chrono::system_clock::time_point const&);
		std::chrono::system_clock::time_point to_time_point(dbl_time_point const&);

		/**
		 * Calculate duration of day.
		 * 
		 * @return timespane since midnight (0 o'clock)
		 */
		std::chrono::system_clock::duration duration_of_day(std::chrono::system_clock::time_point const&);

		/**
		 * Create a new timestamp by setting all non-date data (minutes,
		 * seconds, ...) to zero.
		 * Example: Sat Mar 28 00:48:09 2017 => Sat Mar 28 00:00:00 2017
		 */
		std::chrono::system_clock::time_point strip_time(std::chrono::system_clock::time_point const&);
		
		/**
		 * Convert to string using the ctime() function.
		 * Format is "Day Mon dd hh:mm:ss yyyy" as local time.
		 * Example: Sat Mar 28 00:48:09 2015
		 */
		std::string to_string(std::chrono::system_clock::time_point const&);
	
		std::tm make_utc_tm(std::chrono::system_clock::time_point tp);
	}
}

#endif 	//	CYNG_CHRONO_H

